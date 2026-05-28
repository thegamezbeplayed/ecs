cmake_minimum_required(VERSION 3.21)
project(index LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ====================== Platform ======================
if (EMSCRIPTEN)
    set(PLATFORM Web CACHE STRING "Target platform" FORCE)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()
if (NOT EMSCRIPTEN)
  set(PLATFORM Desktop CACHE STRING "Target platform" FORCE)
endif()
# ====================== Emscripten Flags ======================
if (PLATFORM MATCHES "Web")
  # Compile-time flags only
  set(EM_COMPILE_FLAGS
    -O0
    -g
  )

  # Link-time flags (the -s ones)
  set(EM_LINK_FLAGS
    -sALLOW_MEMORY_GROWTH=1
    -sINITIAL_MEMORY=256MB
    -sSTACK_SIZE=2MB
    -sUSE_GLFW=3
    -sUSE_WEBGL2=1
    -sFETCH=1
    -sFORCE_FILESYSTEM=1
    -sASSERTIONS=2
    -gsource-map
    --preload-file ${CMAKE_SOURCE_DIR}/resources@resources
    -sEXPORTED_RUNTIME_METHODS=['ccall','cwrap','HEAPF32','HEAP32','HEAP8','HEAPU8']
    --shell-file ${CMAKE_SOURCE_DIR}/web_shell.html
  )

  add_compile_options(${EM_COMPILE_FLAGS})
  add_link_options(${EM_LINK_FLAGS})

endif()

# ====================== Dependencies ======================
include(FetchContent)

# cJSON
set(ENABLE_CJSON_UNINSTALL OFF CACHE BOOL "" FORCE)
set(ENABLE_CJSON_TEST     OFF CACHE BOOL "" FORCE)
set(ENABLE_CJSON_UTILS    OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS     OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    cJSON
    GIT_REPOSITORY https://github.com/DaveGamble/cJSON.git
    GIT_TAG        master
    GIT_SHALLOW    TRUE
)

FetchContent_Declare(
    xxHash
    GIT_REPOSITORY https://github.com/Cyan4973/xxHash.git
    GIT_TAG        v0.8.2          # or 'origin/dev' for latest
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(xxHash)
# raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.5
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(cJSON raylib xxHash)
add_library(xxhash INTERFACE)
target_include_directories(xxhash INTERFACE ${xxhash_SOURCE_DIR})
target_compile_definitions(xxhash INTERFACE XXH_INLINE_ALL)
# ====================== Main Target ======================
add_executable(${PROJECT_NAME})

add_subdirectory(src)
if (PLATFORM MATCHES "Desktop")
  add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
  )
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/includes
        ${cJSON_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib cjson xxhash)
