# Repository Overview

This repository is a C99/raylib game prototype built around a custom ECS, a global game process (`GP`), a global ECS world (`world`), JSON-driven definitions, generated scene data, and a raygui UI layer.

Major systems/modules found:

- App/screen loop: `app.c`, `game_world.c`, `game.c`, `screen_*.c`, `screens.h`, `game_process.h`
- ECS runtime: `game_register.h`, `component_registry.c`, `game_entity.c`, `game_system.c`, `game_registry.c`, `components.h`
- ECS gameplay definitions: `game_define.h`, `game_data.c`, `game_parse.c`, `resources/data/definitions.json`
- Gameplay systems: `system_anim.c`, `system_sprite.c`, `system_camera.c`, `system_physics.c`, `system_input.c`, `system_render.c`, `system_level.c`, `system_behavior.c`, `system_expire.c`, `system_combat.c`
- Gameplay component domains: `component_define.h`, `game_assets.h`, `game_physics.h`, `game_control.h`, `game_views.h`, `game_stats.h`, `game_behaviors.h`, `game_types.h`
- Assets/resources/sprites: `game_resources.h`, `game_assets.h`, `game_sprites.c`, `game_load.c`
- UI: `game_ui.h`, `game_menu.c`, `game_layout.c`, `game_print.c`, `game_ui_data.c`, `raygui.h`
- Events/timers/interactions: `game_utils.h`, `game_events.c`, event helpers exposed through `game_process.h`
- Scenes: `scene.h`, `scene_loader.c`, `scene_data.h`, `scene_data_wrapper.c`

God files / central files:

- `src/game_menu.c`: 1368 lines, about 118 function definitions. It contains UI construction, menu construction, layout recursion, rendering, state transitions, callbacks, context/value binding, and custom raygui controls.
- `src/game_parse.c`: 781 lines, about 46 function definitions. It parses generic JSON, Aseprite sheets, scene files, system definitions, component definitions, prefabs, and relations.
- `src/game_tools.h`: 775 lines. It is an umbrella utility header with geometry macros, hash map declarations, hash helpers, grid/cell algorithms, random helpers, rectangle helpers, and many static functions.
- `src/game_ui.h`: 428 lines. It exports UI data structs, UI runtime structs, callbacks, menu APIs, custom raygui controls, state transition data, and static state requirements.
- `src/game_systems.h`: central gameplay-system registry. It declares all system callbacks and owns `FUNCTION_LOOKUP`, so every new system callback currently modifies this header.
- `src/component_define.h`: includes asset, input, physics, view, stat, and behavior headers, then defines components from all domains in one place.
- `src/game_define.h`: mixes game-definition parsing data, relation tables, game-state/update lookup tables, constants, component function lookup API, and `game_t`.
- `src/game_world.c`: owns global world/process initialization, screen transition tables, event wrappers, game state, ECS startup, scene setup, and event bus access.

Include fan-in hotspots from the current tree:

- `game_process.h`: included by 15 files.
- `game_systems.h`: included by 15 files.
- `game_define.h`: included by 14 files.
- `game_utils.h`: included by 11 files.
- `game_tools.h`: included by 10 files.
- `game_assets.h`: included by 9 files.
- `game_ui.h`, `game_strings.h`, `component_define.h`: included by 8 files each.

Git history also points at central conflict files. In local history, the most-touched source/header files under `src/` are `game_systems.h` (25 touches), `game_registry.c` (21), `game_data.c` (21), `game_define.h` (21), `game_world.c` (20), `game_sprites.c` (19), `game_assets.h` (18), `game_register.h` (17), `system_anim.c` (17), `game_physics.h` (14), `system_physics.c` (14), `game_entity.c` (14), and `game_parse.c` (13).

Translation units likely to generate broad rebuilds:

- Any change to `game_systems.h` rebuilds nearly all system `.c` files plus parser/data files because it includes `game_define.h` and `component_define.h`.
- Any change to `component_define.h` rebuilds rendering, physics, input, animation, stats, and behavior users because it includes every component domain header.
- Any change to `game_process.h` rebuilds app, events, UI, world, collision, input, sprites, tools, and screen code because it exposes `GP`, screen state, process state, events, and world helper functions.
- Any change to `game_tools.h` rebuilds a large cross-section because it is pulled through `game_types.h`, `game_utils.h`, `game_assets.h`, `screens.h`, and `components.h`.
- `game_ui.h` pulls in `game_assets.h` and `game_utils.h`; a UI struct tweak can rebuild asset and gameplay-facing code.

# Major Conflict Hotspots

## File: src/game_menu.c
- Risk: HIGH
- Why: This file combines too many UI responsibilities: raygui setup (`InitUI`), descriptor-to-runtime construction (`InitElementByName`), tree ownership (`ElementAddChild`), layout recursion (`ElementResize`), rendering (`ElementRender`), sync/layer queues (`UISync`, `UIRender`), menu transitions, element state machine, data/context binding, text/value sizing, callbacks, and custom controls. Any UI feature naturally edits this file.
- Developers/features likely to collide: UI/UX developers adding widgets, gameplay programmers wiring HUD data, engine programmers changing input/state behavior, graphics programmers changing 9-slice/raygui drawing, and designers adding menu behavior.
- Recommended split: `ui_manager.c`, `ui_element_factory.c`, `ui_element_tree.c`, `ui_element_state.c`, `ui_render.c`, `ui_context.c`, `ui_controls_raygui.c`, and keep layout in `game_layout.c` or rename to `ui_layout.c`.
- Expected improvement: A button visual change stops colliding with HUD data binding, menu transition work, and layout work. Designers changing menu behavior touch state/callback files, while rendering work stays in render/control files.

## File: src/game_ui.h
- Risk: HIGH
- Why: It exposes UI internals to every UI user: runtime element structs, descriptor structs, callback typedefs, menu manager internals, `ELEM_DATA`, `MENU_DATA`, custom control prototypes, and static `ELEM_STATE_REQ`. It also includes `game_assets.h` and `game_utils.h`, dragging assets, ECS-ish params, and event/choice utility types into UI consumers.
- Developers/features likely to collide: UI data authors, UI runtime programmers, HUD gameplay integration, and rendering/style work.
- Recommended split: `ui_public.h` for `InitUI`, `UISync`, `UIRender`, high-level menu APIs; `ui_types.h` for small enums and opaque `ui_element_t`/`ui_menu_t`; `ui_element_internal.h` for struct definitions; `ui_data.h` for descriptor definitions; `ui_render.h`; `ui_context.h`; `ui_controls.h`.
- Expected improvement: Most screens only need `ui_public.h`. Runtime internals can change without forcing every file that includes `game_ui.h` to rebuild or merge.

## File: src/game_ui_data.c
- Risk: HIGH
- Why: `ELEM_DATA[ELE_COUNT]` and `MENU_DATA[MENU_DONE]` are central UI registries. New menus, HUD widgets, title screen copy, callbacks, textures, and child hierarchies all edit the same arrays. `ELE_COUNT` in `game_ui.h` also has to be edited when the table grows.
- Developers/features likely to collide: HUD work, title/end screen work, pause/options menu work, UI styling, and gameplay data display.
- Recommended split: `ui_data_main_menu.c`, `ui_data_hud.c`, `ui_data_ending.c`, `ui_data_debug.c`, plus a small `ui_data_registry.c` that concatenates per-menu descriptor lists at startup. Replace `ELE_COUNT` with `ARRAY_COUNT(local_array)` per file and an aggregate count computed by registration.
- Expected improvement: A HUD programmer can add restart/score widgets without touching title/ending data. Menu copy edits no longer conflict with gameplay HUD feature work.

## File: src/game_parse.c
- Risk: HIGH
- Why: It mixes unrelated parsers and registries: JSON helpers, string-to-enum lookups, component parsers, system parser, game-state/update lookup, Aseprite parsing, scene parsing, prefab parsing, and relation parsing. It also includes `game_systems.h`, so parser changes are coupled to all system declarations and the system function registry.
- Developers/features likely to collide: tools/import developers, gameplay component authors, asset pipeline work, scene loader work, ECS system authors, and content/prefab work.
- Recommended split: `json_read.c`, `parse_component_defs.c`, `parse_system_defs.c`, `parse_prefabs.c`, `parse_relations.c`, `parse_scene.c`, `parse_aseprite.c`, `lookup_names.c`.
- Expected improvement: Aseprite import changes stop colliding with prefab/component definition changes. A new system callback no longer requires editing the same parser file that scene importers touch.

## File: src/game_systems.h
- Risk: HIGH
- Why: It is the central system callback registry. It declares callbacks for position, animation, input, force, physics, level, render, camera, sprite, behavior, combat, and expiration in one header, and it defines `FUNCTION_LOOKUP[NUM_FUNCTIONS]` directly in the header. Adding a new system function changes a widely included header and risks duplicate static copies in each translation unit.
- Developers/features likely to collide: AI, combat, physics, rendering, camera, input, and gameplay programmers adding systems or callbacks.
- Recommended split: `systems/system_registry.c` owns the lookup table; `systems/system_registry.h` exports `SystemFunctionLookup`. Per-domain headers such as `systems/physics_system.h`, `systems/anim_system.h`, `systems/render_system.h`, `systems/camera_system.h`, and `systems/input_system.h` declare only their own callbacks.
- Expected improvement: Physics and render system authors stop editing one shared header. Most `.c` files include only their domain system header plus a minimal ECS API.

## File: src/game_data.c
- Risk: HIGH
- Why: It owns `CORE_COMPONENTS`, `COMPFUNC_LOOKUP`, component definition lookup, and prefab initialization. Every new core component changes this file and often `components.h`, `component_define.h`, `game_define.h`, and `definitions.json` at the same time.
- Developers/features likely to collide: gameplay programmers adding components, AI/combat programmers adding state, physics programmers adding bodies/forces, rendering programmers adding sprite/view components, and content authors adding prefab component data.
- Recommended split: `component_types_registry.c` for component sizes, `component_init_registry.c` for init functions, `prefab_build.c` for prefab instantiation from parsed data, and optional per-domain registration files such as `physics_components.c`, `render_components.c`, `input_components.c`.
- Expected improvement: Adding an AI component can be contained to AI component registration rather than forcing edits in the central data file used by sprite/view/input/physics.

## File: src/game_define.h
- Risk: HIGH
- Why: This header mixes constants, scene instance structs, prefab definition structs, component-entry parser structs, `system_define_t`, `game_t`, relation lookup data, component index helpers, game-state lookup data, and update-step lookup data. It includes `game_register.h`, which includes `game_process.h`, which includes screen and utility headers. The dependency direction is inverted: data-definition types pull in runtime process/world APIs.
- Developers/features likely to collide: ECS runtime work, content schema work, component/parser work, game state work, and relation logic.
- Recommended split: `game_config_limits.h`, `game_definition_types.h`, `component_definition_types.h`, `system_definition_types.h`, `relation_types.h`, `state_lookup.c/.h`, `update_lookup.c/.h`.
- Expected improvement: Changing a parser schema or a relation table no longer rebuilds runtime world/process code or collides with system registration work.

## File: src/component_define.h
- Risk: HIGH
- Why: It is the shared component mega-header. It includes assets, controls, physics, views, stats, and behaviors, then defines all component structs. Adding or editing any component touches this one file and can pull many unrelated headers into systems that only need one component.
- Developers/features likely to collide: physics, animation, rendering, input, camera, stats, behavior, level, and future AI/combat component authors.
- Recommended split: `components/position_component.h`, `components/physics_component.h`, `components/force_component.h`, `components/animation_component.h`, `components/sprite_component.h`, `components/view_component.h`, `components/input_component.h`, `components/camera_component.h`, `components/state_component.h`, `components/lifetime_component.h`, and a generated or hand-maintained `components/all_components_private.h` only for registries.
- Expected improvement: `system_camera.c` should not rebuild because a physics force field changed. AI component work gets isolated from rendering and physics definitions.

## File: src/game_world.c
- Risk: HIGH
- Why: It owns global `world` and `GP`, ECS initialization, game definition loading, scene setup, event wrapper APIs, state transitions, screen phase/update table registration, process child setup, and per-frame sync. Screen additions, game-state changes, event-bus changes, and ECS startup changes all converge here.
- Developers/features likely to collide: engine loop work, screen/menu flow work, ECS initialization work, scene loading work, and event system work.
- Recommended split: `game_process.c` for `GP` lifecycle and update loop, `screen_flow.c` for screen transition tables, `game_bus.c` for `Subscribe/GameEvent/ScheduleEvent`, `world_bootstrap.c` for ECS definition loading and scene setup, `game_state.c` for `GameSetState`.
- Expected improvement: Adding a screen no longer conflicts with event-bus changes or ECS bootstrap changes.

## File: resources/data/definitions.json
- Risk: HIGH
- Why: It is one JSON database for active systems, inactive system definitions, component definitions, prefab definitions, and relations. A feature that adds a component, a prefab, a system, or content data edits the same file. This is also the live bridge between content and runtime registries.
- Developers/features likely to collide: content designers, gameplay programmers, renderer/sprite authors, physics/input developers, and level/scene designers.
- Recommended split: `resources/data/systems/*.json`, `resources/data/components/*.json`, `resources/data/prefabs/*.json`, `resources/data/relations/*.json`, with a manifest file such as `game_definition.json` that lists files to load. Support directory or manifest loading in `parse_game_definition`.
- Expected improvement: A designer adding tile prefabs does not collide with a programmer registering a physics system or input component.

## File: src/game_assets.h and src/game_sprites.c
- Risk: MEDIUM/HIGH
- Why: `game_assets.h` combines render layers, animation events, sprite sheet data, sprite structs, animation structs, global sheet storage, and sprite draw/animation APIs. `game_sprites.c` combines splash loading, UI texture loading, sprite construction, collision data extraction, drawing, subtexture loading, transparent preprocessing, animation playback, animation state registration, Aseprite slice helpers, and component init functions.
- Developers/features likely to collide: asset pipeline programmers, animation programmers, rendering programmers, UI texture work, and gameplay component initialization work.
- Recommended split: `resources/sprite_sheet.h/.c`, `render/sprite_draw.c`, `animation/anim_player.c`, `animation/anim_registry.c`, `assets/splash.c`, `assets/texture_preprocess.c`, `components/sprite_init.c`, `components/anim_init.c`.
- Expected improvement: Animation state changes no longer collide with sprite rendering or splash/resource-loading work.

## File: src/system_physics.c
- Risk: MEDIUM/HIGH
- Why: It combines physics event handlers, force event handling, collision detection response, hitbox spawning, relation handling, rigid body movement, debug rendering, force load/system/cleanup, and combat-event interaction. It also reaches directly into global `world` from callbacks.
- Developers/features likely to collide: physics programmers, combat programmers, animation/hurtbox programmers, debug rendering work, and gameplay force/impulse work.
- Recommended split: `physics/physics_system.c`, `physics/collision_system.c`, `physics/force_system.c`, `physics/physics_events.c`, `combat/hitbox_spawn.c`, `physics/physics_debug_draw.c`.
- Expected improvement: Combat hitbox work can move independently from force integration and collision resolution.

## File: src/system_anim.c
- Risk: MEDIUM
- Why: Animation playback is coupled to input events, combat hit events, physics hitbox spawning, interaction cooldowns, sprite index mutation, and animation behavior dispatch. A future AI/combat animation feature will edit this same file as input animation work.
- Developers/features likely to collide: animation programmers, combat programmers, input programmers, and sprite/render programmers.
- Recommended split: `animation/anim_system.c`, `animation/anim_input_events.c`, `animation/anim_combat_events.c`, `animation/anim_behavior.c`.
- Expected improvement: Adding combat animation reactions does not conflict with movement animation input handling.

## File: src/game_events.c and src/game_utils.h
- Risk: MEDIUM
- Why: `game_events.c` combines payloads, cooldown timers, event bus, notification pool, and interaction cooldown map. `game_utils.h` exposes all of those types plus behavior choice structs, string/file helpers, and state requirement structs. It is included by 11 files.
- Developers/features likely to collide: engine/event work, gameplay interaction cooldowns, behavior/AI choice work, and UI state-machine work.
- Recommended split: `events/event_bus.c/.h`, `events/notifications.c/.h`, `events/timers.c/.h`, `gameplay/interactions.c/.h`, `ai/choice_pool.c/.h`, `state/state_requirements.h`.
- Expected improvement: AI choice tuning and event bus changes no longer share one header/source surface.

## File: src/game_register.h
- Risk: MEDIUM
- Why: It exposes ECS world internals (`world_t` layout), prefab registry internals, relation storage, query structs, system structs, component APIs, global `world`, and query global `EQ`. Systems include this transitively through `game_systems.h`, so they can reach all internals.
- Developers/features likely to collide: ECS runtime maintainers, system authors, prefab/relations feature work, and component storage changes.
- Recommended split: `ecs/world.h` opaque public API, `ecs/world_internal.h` for storage layout, `ecs/component_api.h`, `ecs/system_api.h`, `ecs/query.h`, `ecs/prefab.h`, `ecs/relation.h`.
- Expected improvement: System authors consume stable API headers, while storage refactors stay private to ECS implementation files.

## File: src/game_enum.h
- Risk: MEDIUM
- Why: This is a shared enum/constants table for event bases, game events, input events, combat events, entity types, UI/game params, levels, tile status, actions, turns, behavior status, and interaction results. Adding unrelated enum values in one file is a classic merge-conflict pattern.
- Developers/features likely to collide: input, combat, level, action/turn, AI behavior, tile/gameplay status, and entity-type work.
- Recommended split: `events/event_ids.h`, `gameplay/entity_types.h`, `input/input_events.h`, `combat/combat_events.h`, `level/level_ids.h`, `actions/action_types.h`, `ui/game_params.h`.
- Expected improvement: Combat event additions stop colliding with input, tile, turn, or UI param changes.

# Suggested Module Refactors

## Current:
UI is concentrated in `game_menu.c`, `game_ui.h`, `game_ui_data.c`, `game_layout.c`, and `game_print.c`.

## Proposed:
- `ui/ui_public.h`: opaque `ui_element_t`, `ui_menu_t`, `InitUI`, `UISync`, `UIRender`, menu open/close API.
- `ui/ui_types.h`: UI enums that external callers genuinely need.
- `ui/ui_element_internal.h`: full `ui_element_t` and value structs, private to UI implementation.
- `ui/ui_menu_internal.h`: full `ui_menu_t` and manager internals.
- `ui/ui_data.h`: descriptor types and registration API.
- `ui/ui_data_title.c`, `ui/ui_data_hud.c`, `ui/ui_data_ending.c`: per-menu data tables.
- `ui/ui_data_registry.c`: registers descriptor arrays and replaces global `ELEM_DATA[ELE_COUNT]`.
- `ui/ui_element_factory.c`: `InitElementByName`, child descriptor expansion.
- `ui/ui_element_state.c`: `ElementCanChangeState`, `ElementSetState`, callbacks/state transitions.
- `ui/ui_element_tree.c`: `ElementAddChild`, free/hide/show/toggle child operations.
- `ui/ui_layout.c`: size and layout calculations.
- `ui/ui_render.c`: `UIRender`, `ElementRender`.
- `ui/ui_controls_raygui.c`: `GuiPanel`, `GuiLabel`, `GuiHeader`, `GuiTitle`, textured button, tooltip.
- `ui/ui_context.c`: `ElementSetContext`, owner/context param functions, value sync.
- `ui/ui_text.c`: `ElementSetText`, value text sizing.

## Benefits:
This isolates UI roles. A visual-control developer works mostly in `ui_render.c` and `ui_controls_raygui.c`; a HUD/data developer works in `ui_data_hud.c`; a gameplay programmer wiring values works in `ui_context.c`; a menu-flow programmer works in `ui_element_state.c` and `ui_menu.c`. `screen_title.c` and `screen_ending.c` should only include `ui_public.h`.

## Current:
System registration is centered in `game_systems.h` plus `game_parse.c` string lookup and `resources/data/definitions.json`.

## Proposed:
- `systems/system_registry.h/.c`: owns `SystemFunctionLookup`.
- `systems/anim_system.h`, `systems/sprite_system.h`, `systems/camera_system.h`, `systems/render_system.h`, `systems/physics_system.h`, `systems/input_system.h`, etc.
- A registration macro or function per domain: `RegisterAnimationSystemFns(registry)`, `RegisterPhysicsSystemFns(registry)`.
- `parse_system_defs.c`: parses JSON system definitions but depends only on `system_registry.h`, not every system header.

## Benefits:
New systems can be added by touching the domain system file and the domain registration file, not a global umbrella header. AI developers rarely touch rendering declarations; rendering developers do not edit the physics/input callback registry.

## Current:
Components are centralized in `component_define.h`, `components.h`, `game_data.c`, `game_define.h`, and `definitions.json`.

## Proposed:
- `components/component_ids.h`: stable IDs/macros or string-to-ID public API.
- `components/component_pool.h`: `component_pool_t`, registration/add/get APIs.
- `components/position_component.h`
- `components/physics_component.h`
- `components/force_component.h`
- `components/animation_component.h`
- `components/sprite_component.h`
- `components/view_component.h`
- `components/input_component.h`
- `components/camera_component.h`
- `components/lifetime_component.h`
- `components/component_registry_data.c`: core component size registration.
- `components/component_init_registry.c`: component init function lookup.

## Benefits:
Component authors can work in their domain. A new AI state component does not require including sprite sheet and physics types everywhere. Systems include exactly the components they read/write.

## Current:
JSON parsing is monolithic in `game_parse.c`.

## Proposed:
- `parse/json_read.h/.c`: `Json_GetString`, `Json_GetInt`, `Json_GetFloat`, `Json_GetBool`, `ParseRoot`.
- `parse/name_lookup.h/.c`: `StringToSheetID`, `StringToAnimState`, `GetGameState`, `GetUpdateStep`, `RelationTypeLookup`.
- `parse/parse_component_defs.c`
- `parse/parse_system_defs.c`
- `parse/parse_prefabs.c`
- `parse/parse_relations.c`
- `parse/parse_scene.c`
- `parse/parse_aseprite.c`

## Benefits:
Tools/import work stops colliding with gameplay schema work. Scene importers and Aseprite importers can work in parallel.

## Current:
`game_world.c` owns process, screens, event bus facade, and ECS bootstrap.

## Proposed:
- `process/game_process.c`: `InitGameProcess`, `GameProcessSync`, `GameProcessStep`, `GameProcessEnd`.
- `process/screen_flow.c`: screen phase/update/next registration and `GameTransitionScreen`.
- `process/game_state.c`: `GameSetState`, `GameStepState`.
- `events/game_bus.c`: `Subscribe`, `SubscribeEntity`, `TargetSubscribe`, `GameEvent`, `ScheduleEvent`, `GameBus`.
- `world/world_bootstrap.c`: `InitEntityComponentSystem`, definition loading, prefab init, scene setup.
- `world/world_globals.c`: `world` definition if globals remain.

## Benefits:
Screen-flow changes stop colliding with ECS startup. Event bus implementation can evolve without touching screen registration tables.

## Current:
Assets, sprites, and animation are merged through `game_assets.h` and `game_sprites.c`.

## Proposed:
- `assets/resources.h/.c`: resource loading, `ResourceRef`, resource pools.
- `assets/sprite_sheet.h/.c`: `sprite_sheet_d`, `SHEETS`, texture/sheet loading.
- `render/sprite_draw.h/.c`: `DrawSprite`, `DrawSlice`, render-layer concerns.
- `animation/anim_types.h`, `animation/anim_player.c`, `animation/anim_registry.c`.
- `assets/aseprite_types.h`; parser lives in `parse/parse_aseprite.c`.
- `components/sprite_component_init.c`, `components/animation_component_init.c`.

## Benefits:
Animation work becomes independent from texture loading and sprite rendering. UI texture work stops touching animation and collision slice APIs.

## Current:
Physics, forces, combat hit events, and debug draw are in `system_physics.c`.

## Proposed:
- `physics/rigid_body_system.c`: movement application.
- `physics/collision_system.c`: collision iteration and collision events.
- `physics/force_system.c`: force lifecycle, force application, cleanup.
- `physics/physics_events.c`: `OnPhysEvent`, `OnForceEvent`.
- `combat/hitbox_system.c`: hitbox spawn/destroy behavior now inside `OnPhysEvent`.
- `physics/physics_debug_draw.c`: debug visualization.

## Benefits:
Combat programmers can change hitbox behavior without editing force cleanup or broad collision movement logic. Physics debug work becomes isolated from gameplay collision response.

## Current:
`resources/data/definitions.json` contains all systems, components, prefabs, and relations.

## Proposed:
- `resources/data/game_definition.json`: manifest with arrays of files.
- `resources/data/systems/animation.json`, `systems/sprites.json`, `systems/render.json`, `systems/camera.json`, `systems/physics.json`.
- `resources/data/components/sprite.json`, `animation.json`, `position.json`, `camera.json`, `input.json`, `physics.json`.
- `resources/data/prefabs/player.json`, `prefabs/tiles.json`, `prefabs/views.json`.
- `resources/data/relations/player_forces.json`.

## Benefits:
Content authors can add prefabs while engine programmers add systems without fighting over one large JSON file. Code review becomes domain-specific.

# Header and Dependency Issues

`game_process.h` is too broad. It exposes screen includes, game state/process structs, event wrapper APIs, global `GP`, notification/interactions, and `WorldGetTime`. Many users only need `GameEvent`, `Subscribe`, or `WorldGetTime`.

Recommendation:
- `process/game_state.h`: `GameState`, `GameSetState`, `WorldGetTime`.
- `process/game_process.h`: high-level process lifecycle only.
- `events/game_bus.h`: `Subscribe`, `GameEvent`, `ScheduleEvent`.
- `process/game_process_internal.h`: full `game_process_t` and `extern GP`, private to process/event implementation.

`game_systems.h` is an umbrella header with a lookup table defined in the header.

Recommendation:
- Move `FUNCTION_LOOKUP` to `system_registry.c`.
- Replace global `NUM_FUNCTIONS` fixed array with a registration table that can grow or be composed from per-domain arrays.
- Systems should include `ecs/system_api.h` plus only their component headers.

`component_define.h` overexposes all components and imports every component domain.

Recommendation:
- Split per component domain.
- Keep an internal `components/all_components_private.h` only for registries that genuinely need every type size.
- Component parser/init functions should live beside the component they initialize.

`game_define.h` depends on runtime ECS/process headers for data-schema definitions.

Recommendation:
- Data schema headers should not include `game_register.h`.
- Use forward declarations or primitive IDs in definition structs where possible.
- Move lookup arrays out of headers and into `.c` files.

`game_tools.h` is a rebuild amplifier and category mixer.

Recommendation:
- Split into `base/memory.h`, `base/hash.h`, `math/rect_utils.h`, `math/cell_grid.h`, `math/random.h`, `math/vector_utils.h`.
- Move non-trivial static functions out of headers unless they are truly hot inline helpers.
- Keep macros like `VEC_NEW` only in a minimal math utility header.

`game_utils.h` mixes events, timers, interactions, choice/AI, state machine helpers, file/string utilities, and Emscripten upload declaration.

Recommendation:
- Split `events/event_bus.h`, `events/timers.h`, `gameplay/interactions.h`, `ai/choice_pool.h`, `state/state_requirements.h`, `platform/web_upload.h`, `base/file_utils.h`.

`game_common.h` and `game_types.h` include each other indirectly. `game_common.h` includes `game_types.h`, while `game_types.h` includes `game_tools.h`, and `game_tools.h` pulls in `game_math.h`; meanwhile `game_utils.h` includes both `game_types.h` and `game_common.h`. This creates fragile include order and makes `Entity` unavailable as a clean primitive type.

Recommendation:
- Create `ecs/entity.h` containing only `Entity`, `INVALID_ENTITY`, `EntityManager`, and entity APIs.
- Create `base/types.h` or `math/cell.h` for `Cell`, `ShapeType`, and small math primitives.
- Keep ECS entity types independent from generic gameplay param/event types.

`game_ui.h` pulls asset and gameplay utility dependencies into UI consumers.

Recommendation:
- Forward declare `sprite_t`, `line_item_t`, `progress_item_t`, `ui_element_t`, `ui_menu_t` where possible.
- Keep `Rectangle`, `Vector2`, `Color`, and `KeyboardKey` in implementation/private headers unless public constructors require them.
- Move descriptor/runtime struct definitions out of the public UI header.

`game_assets.h` is both asset API and animation/sprite type API.

Recommendation:
- Split sprite, animation, and resource headers.
- Rendering systems should not include animation parser/resource declarations unless they use them.

`screens.h` includes `game_utils.h` but mostly declares screen functions and `GameScreen`.

Recommendation:
- `screens.h` should include only minimal raylib/math types needed for screen declarations. If `mouse_controller_t` is not broadly used, move it to input/UI code.

Circular dependency risks:

- `game_define.h` -> `game_register.h` -> `game_process.h` -> `game_types.h`/`game_utils.h` while `game_define.h` itself declares parser structs and lookup tables. Parser/data types depend on runtime process types.
- `game_common.h` -> `game_types.h`; `game_types.h` contains payload/param helpers that rely on memory/hash helpers from `game_tools.h`, which is used by many headers that also include `game_common.h`.
- `component_define.h` includes `game_physics.h`; `game_physics.h` includes `game_define.h`; `game_define.h` includes `game_register.h`; this means a component type header reaches back into definition/parser/runtime world headers.

Compile firewall opportunities:

- Make `world_t`, `game_process_t`, `ui_manager_t`, `ui_element_t`, and `sprite_sheet_d` opaque outside their modules.
- Put global arrays (`SHEETS`, `FUNCTION_LOOKUP`, `CORE_COMPONENTS`, `COMPFUNC_LOOKUP`, `MENU_DATA`, `ELEM_DATA`) behind registration functions or module-specific accessors.
- Move static lookup tables out of headers into `.c` files with query functions.
- Replace umbrella includes in systems with domain component headers and `ecs/system_api.h`.

# Highest Priority Refactors

1. Split `game_systems.h` and move `FUNCTION_LOOKUP` to `system_registry.c`.

This is the highest-impact team scalability fix. It removes a 15-file include hotspot and prevents every system addition from editing one shared header.

2. Split `component_define.h` into per-domain component headers.

This reduces rebuild blast radius and separates physics/render/input/animation ownership. It also makes it easier to give teams ownership of component domains.

3. Split `game_menu.c` and `game_ui.h`.

The UI layer is the largest source file and largest exported UI header. It will become painful as soon as HUD, menus, styling, and gameplay value binding are worked on in parallel.

4. Split `resources/data/definitions.json`.

This file is a content and system registry bottleneck. Moving to per-domain JSON files gives content designers and gameplay programmers separate edit surfaces.

5. Split `game_parse.c`.

Parser work currently crosses assets, scenes, components, systems, prefabs, and relations. Splitting it lets asset pipeline, scene, and gameplay schema work proceed independently.

6. Split `game_world.c` into process, screen flow, event bus facade, game state, and world bootstrap.

This removes a central collision point between engine-loop work, screen transitions, ECS startup, and event bus changes.

7. Split `game_tools.h` and `game_utils.h`.

These headers are hidden rebuild amplifiers. Their contents should become small, purpose-specific headers.

8. Split `game_assets.h` / `game_sprites.c` into resource, sprite draw, sprite sheet, animation, and component-init modules.

This separates asset pipeline, animation, rendering, and component initialization.

# Quick Wins

- Move `FUNCTION_LOOKUP` out of `game_systems.h` into a `.c` file immediately. Keep the API shape if needed; this alone reduces header churn.
- Replace `ELE_COUNT` with `ARRAY_COUNT(ELEM_DATA)` locally or a registration count so adding UI elements does not require editing `game_ui.h`.
- Move `ELEM_STATE_REQ` out of `game_ui.h` into `ui_element_state.c`; it is implementation data, not public API.
- Create `events/game_bus.h` and stop including `game_process.h` in files that only need `GameEvent` or `Subscribe`.
- Create `ecs/entity.h` and move `Entity`, `INVALID_ENTITY`, and `EntityManager` out of `game_common.h`.
- Make `world_t` private by introducing minimal ECS APIs for systems: `EcsGet`, `EcsAdd`, `EcsEntityValid`, `EcsRelationTarget`, `EcsEmit`.
- Split `game_enum.h` by domain before adding more events or gameplay enums.
- Move custom raygui controls (`GuiPanel`, `GuiLabel`, `GuiHeader`, `GuiTitle`, `GuiTooltipControl`) from `game_menu.c` into `ui_controls_raygui.c`.
- Move Aseprite parsing from `game_parse.c` into `parse_aseprite.c`.
- Move scene parsing from `game_parse.c` into `parse_scene.c`.
- Split `definitions.json` first by prefabs/components/systems even if the loader initially concatenates them into the existing `game_t` shape.
- Replace direct global `world` usage in event callbacks with callback user data or a `GameWorld()` accessor. `system_physics.c` currently receives `world_t* w` for normal system functions but uses global `world` in `OnForceEvent` and `OnPhysEvent`.
- Add capacity guards to shared registries (`SystemRegister`, `SystemCreate`, `SPRITE_LAYERS`, UI render layers) before multiple developers expand content. This prevents merge-driven table growth from becoming memory corruption.
- Remove unused or stub declarations from public headers (`ElementDynamicValue`, empty parser functions, unimplemented UI callbacks) or move them private until implemented. Public stubs invite unrelated edits.

Team scalability:

- 2 developers: workable, but they should avoid both touching UI or ECS registries in the same branch. The current architecture is tolerable for one gameplay programmer plus one content/UI person if changes are coordinated.
- 5 developers: conflict-prone. UI, components, system registration, and `definitions.json` become regular merge points. Parallel work across animation/physics/render/input still funnels through `game_systems.h`, `component_define.h`, `game_data.c`, and `definitions.json`.
- 10+ developers: not scalable without module boundaries. Ownership cannot be cleanly assigned because headers and registries cross domains. A 10-person team would need domain-owned folders, private/public headers, per-domain registries, and data-file sharding before feature branches can be merged predictably.
