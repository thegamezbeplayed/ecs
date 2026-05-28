# Project Feature Notes

This document consolidates durable project explanations that previously lived in dated session notes. It avoids resolved investigations and implementation advice that no longer matches the current source.

## Project Shape

The project is a C99/raylib game prototype with:

- custom ECS runtime and component pools
- screen/state process table
- pooled event bus
- observer/subject notification layer
- JSON-driven gameplay definitions
- generated scene registry with JSON scene files
- retained raygui UI layer
- desktop and Emscripten/web build targets

Primary third-party libraries:

- `raylib` 5.5 through CMake `FetchContent`
- `cJSON` through CMake `FetchContent`
- vendored `raygui` in [raygui.h](../src/raygui.h)
- raylib companion headers such as `raymath.h` and `rlgl.h`

## Runtime Flow

Desktop startup begins in [platform_app.c](../src/platform_app.c). Web startup begins in [platform_web.c](../src/platform_web.c). Both entrypoints delegate shared lifecycle work to [app_core.c](../src/app_core.c).

The shared startup path is:

1. [AppInit()](../src/app_core.c) initializes the window, stores the initial `"WINDOW"` subject, sets `SCREEN_LOGO`, initializes audio, splash, process tables, scene loading, resources, and UI.
2. [InitGameProcess()](../src/process_game.c) fills screen phase/update tables.
3. [SceneInit()](../src/scene_loader.c) queues generated resource references, then `PlatformLoaderStart()` begins platform-specific loading.
4. [AppFrame()](../src/app_core.c) advances resource loading and runs [GameProcessSync()](../src/process_game.c).

`GameProcessSync()` dispatches update steps for the active `GP.screen`. Screen transitions call [GameTransitionScreen()](../src/process_game.c), which finishes the current screen, switches `GP.screen`, initializes the next screen through `GameSetScreen()`, and starts that screen's music.

[GameSetState()](../src/process_game.c) stores state per active screen with `GP.state[GP.screen]`. Gameplay-specific state changes also emit ECS/gameplay events and call configured state callbacks.

The web path keeps the same high-level lifecycle but uses web-specific mechanics:

1. [platform_web.c](../src/platform_web.c) calls [AppInit()](../src/app_core.c) with web dimensions.
2. [platform_loader_web.c](../src/platform_loader_web.c) loads queued textures synchronously because Emscripten has already preloaded the `resources` package before C `main()` runs.
3. [AppUpdateLoading()](../src/app_core.c) calls [GameTransitionScreen()](../src/process_game.c) once web resources are ready.
4. [UpdateDrawFrame()](../src/platform_web.c) calls `AppFrame(false)` through `emscripten_set_main_loop()`.

The desktop path uses [platform_loader_desktop.c](../src/platform_loader_desktop.c) to load images on a background pthread and upload textures on the main thread during `PlatformLoaderProgress()`.

## Gameplay Bootstrap

Gameplay screen initialization currently runs through [InitGameplay()](../src/game.c):

1. [InitGameEvents()](../src/game_manage.c) creates `GM.bus` and notification storage.
2. [InitGameWorld()](../src/process_world.c) initializes the world and game definitions.
3. [LoadGameDefine()](../src/game_load.c) parses [component_def.json](../resources/data/component_def.json), [system_def.json](../resources/data/system_def.json), and [prefab_def.json](../resources/data/prefab_def.json).
4. Components from the definition file are registered through [ComponentRegisterCore()](../src/component_registry.c).
5. Systems from the definition file are created through [SystemCreate()](../src/gbm_system.c).
6. [GameInitPrefabs()](../src/game_data.c) initializes prefab data.
7. [SceneLoadByIndex()](../src/scene_loader.c) loads the selected scene from the generated scene registry and JSON scene file.
8. [SceneSetup()](../src/scene_loader.c) spawns scene entities and tiles from the parsed scene data.

[GameLoad()](../src/process_world.c) currently hardcodes scene index `2`, which maps to `Floor1` in [scene_data.h](../src/scene_data.h). Scene selection is still not data-driven at runtime.

## ECS

Entities are lightweight IDs plus generations. Component data lives outside the entity in typed pools.

Core ECS files:

- [game_common.h](../src/game_common.h): `Entity` and `EntityManager`
- [component_define.h](../src/component_define.h): gameplay component structs
- [game_register.h](../src/game_register.h): `world_t`, `system_t`, component pools, ECS macros
- [component_registry.c](../src/component_registry.c): component registration/add/get logic
- [gbm_system.c](../src/gbm_system.c): system registration and entity iteration
- [game_data.c](../src/game_data.c): component definitions and prefab initialization
- [gbm_parse.c](../src/gbm_parse.c): gameplay definition and scene parsing
- [parse_components.c](../src/parse_components.c), [parse_behaviors.c](../src/parse_behaviors.c), and [tool_strings.c](../src/tool_strings.c): component-specific parsing, behavior-tree definition parsing, and string-to-enum conversion

Components are registered from the `"components"` list in [component_def.json](../resources/data/component_def.json). Each component type gets a `component_pool_t`, which stores dense component data plus sparse entity-to-index lookup.

Systems are behavior plus a required component set. [SystemCreate()](../src/gbm_system.c) subscribes system callbacks to:

- `GAME_EVENT_STATE` for whole-system state callbacks
- `GAME_EVENT_SET` for entity-iterating state callbacks
- `GAME_EVENT_STEP` for whole-system update-step callbacks
- `GAME_EVENT_SYNC` for entity-iterating update-step callbacks

When sync events fire, [SystemTick()](../src/gbm_system.c) chooses a populated component pool, filters entities that have all required components, checks entity readiness, then invokes the system callback.

Systems communicate through:

- shared component data via `GET_COMPONENT()`
- gameplay events via [GameEvent()](../src/game_manage.c)
- targeted subscriptions via [SubscribeEntity()](../src/game_manage.c) and [TargetSubscribe()](../src/game_manage.c)
- entity relations such as `REL_AppliesTo`, `REL_ChildOf`, `REL_Owner`, and `REL_Target`

## Event System

The gameplay event bus is a single bus owned by `GM.bus` and exposed through [GameBus()](../src/game_manage.c). Current code does not use a screen-indexed bus array.

[InitEventBusEx()](../src/game_events.c) allocates both subscription capacity and event-pool capacity. `event_bus_t` owns:

- fixed subscription storage
- pooled event slots
- a free-list head for event-slot reuse
- scheduled-event hash storage
- due-event scratch storage
- event-pool statistics such as `active_peak` and `dropped_count`

Immediate events use [InitEvent()](../src/game_events.c), [EventEmit()](../src/game_events.c), and [EventRelease()](../src/game_events.c). Scheduled events use the same pool but keep their slots reserved until [EventBusStep()](../src/game_events.c) emits and releases them or [EventUnschedule()](../src/game_events.c) cancels them.

`TF_UPDATE` scheduling compares against `WorldGetTime()`. `TF_TURN` is present in the enum, but turn-based dispatch is not currently implemented.

## Observer / Subject Layer

The project also has an observer/subject notification layer in [gbm_observer.c](../src/gbm_observer.c) and [util_observe.h](../src/util_observe.h). This is separate from the gameplay event bus and is currently used for platform/render style notifications where a named subject broadcasts data to interested observers.

Core pieces:

- `SubjectStore()` records subject data before the subject pool is initialized.
- `InitSubjectPool()` initializes the runtime subject map.
- `SubjectRegister()` creates or returns a named subject.
- `SubjectAddObserver()` attaches a callback and user data to a named subject.
- `SubjectNotify()` runs observer callbacks for a named subject.
- `SubjectDestroy()` frees observers attached to a subject.

[InitScreenWindow()](../src/screen_window.c) stores the initial `"WINDOW"` subject data before gameplay initialization. [ObserveInit()](../src/system_observe.c) initializes the subject pool during `GAME_LOADING`, replays stored subject notifications, and moves stored subjects into the runtime map.

Current users include:

- [CameraLoad()](../src/system_camera.c), which observes `"WINDOW"` and adjusts camera offset when window data is notified.
- [SpritesInit()](../src/system_sprite.c), which observes render-layer subjects and draws sprite layers when those subjects notify.
- [ParticlesInit()](../src/system_particle.c), which uses the same render-layer subject pattern for particles.

## Web Shell And Resize

The web target uses a custom Emscripten shell at [web_shell.html](../web_shell.html), passed to Emscripten with `--shell-file` in [CMakeLists.txt](../CMakeLists.txt). The shell defines a full-page canvas, removes default Emscripten UI chrome, and uses `ResizeObserver` plus `window.resize` to detect browser or embed size changes.

The shell calls `Module._OnCanvasResize(width, height)`. [OnCanvasResize()](../src/platform_web.c) is exported from C with `EMSCRIPTEN_KEEPALIVE` and forwards to [AppOnResize()](../src/app_core.c), which stores the latest dimensions, ignores invalid zero-sized events, updates raylib's window size after the window exists, and notifies the `"WINDOW"` subject with `window_resize_t`.

The current resize behavior updates camera offset through the existing observer path. Render textures created by [InitView()](../src/view_camera.c) are not recreated or letterboxed on resize yet.

## UI Layer

The UI is currently separate from gameplay ECS. It is a retained UI layer built around:

- `ui_manager_t ui`
- `ui_menu_t`
- [ui_element_t](../src/game_ui.h)
- static descriptors in [MENU_DATA](../src/game_ui_data.c) and [ELEM_DATA](../src/game_ui_data.c)
- raygui/raylib rendering in [ElementRender()](../src/game_menu.c)

Startup flow:

1. [InitResources()](../src/game_sprites.c) loads resources.
2. [InitUI()](../src/game_menu.c) loads fonts, configures raygui style, and creates menus.
3. [InitMenuById()](../src/game_menu.c) recursively builds runtime elements from `ELEM_DATA`.
4. Screen update functions call [UISync()](../src/game_menu.c).
5. Screen draw functions call [UIRender()](../src/game_menu.c).

The 9-slice button implementation is data-driven rather than an element type. `ElementType` ends with `UI_BLANK`; 9-slice styling is represented by `UITexture` with `UI_TEXTURE_NONE` and `UI_TEXTURE_9SLICE`.

[DrawElementTexture()](../src/game_menu.c) maps `UI_TEXTURE_9SLICE` to `SHEETS[SHEET_UI].texture` and draws it with raylib `DrawTextureNPatch()`. Textured buttons use [GuiTextureButton()](../src/game_menu.c) so raygui's default button panel does not paint over the texture.

## Game Data And Scenes

Gameplay definitions are split across [component_def.json](../resources/data/component_def.json), [system_def.json](../resources/data/system_def.json), and [prefab_def.json](../resources/data/prefab_def.json). They configure components, systems, prefabs, and relations that are parsed by [gbm_parse.c](../src/gbm_parse.c) and loaded by [game_load.c](../src/game_load.c). Behavior-tree definitions live in [behavior_def.json](../resources/data/behavior_def.json) and are loaded by [BehaviorLoad()](../src/system_behavior.c) through [ParseBehaviorDefs()](../src/parse_behaviors.c).

Tile content is prefab-driven. The current data includes regular `Sprite` + `Position` prefabs for `autotile_0` through `autotile_47`, backed by `SHEET_AUTOTILE` and the generated [dungeon_autotile_47.json](../resources/dungeon_autotile_47.json) / [dungeon_autotile_47.png](../resources/dungeon_autotile_47.png) resource pair.

Scene loading now uses a generated registry plus runtime JSON scene data:

- [scene_data.h](../src/scene_data.h) is generated by the exporter and contains resource references plus `SCENE_HEADERS`.
- Resource references currently load tile, wizard, mob, and dungeon-autotile sheets into the sprite sheet table.
- Each `SceneHeader` maps a scene name to a JSON file path. The current registry includes [scene_01.json](../resources/data/scenes/scene_01.json), [autotile_test.json](../resources/data/scenes/autotile_test.json), and [floor1.json](../resources/data/scenes/floor1.json).
- [SceneLoadHeader()](../src/scene_data_wrapper.c) looks up the header, parses the scene JSON, and fills `Scene`.
- [ParseScene()](../src/gbm_parse.c) reads display/grid dimensions, tile instances, entity instances, and metadata from the JSON.
- [SceneSetup()](../src/scene_loader.c) spawns prefabs for parsed entities and tiles.

## CI Workflows

GitHub Actions are split into independent workflow files so branch protection can require each check separately:

- [linux-desktop.yml](../.github/workflows/linux-desktop.yml): native Linux build and smoke run
- [windows-desktop.yml](../.github/workflows/windows-desktop.yml): Visual Studio build, artifact check, and smoke run
- [web.yml](../.github/workflows/web.yml): Emscripten build and artifact checks

The `windows-desktop` workflow file remains in the repo, but the workflow is currently disabled in GitHub.
The checked-in workflow files define `push` and `pull_request` triggers for `main`; any GitHub UI disabled state is external to the repository.

The web workflow verifies build outputs (`index.html`, `index.js`, `index.wasm`, and `index.data`) but does not yet run a browser smoke test.
