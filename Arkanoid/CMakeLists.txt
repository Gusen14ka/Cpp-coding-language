cmake_minimum_required(VERSION 3.15)

# ── A) Включаем manifest-mode для vcpkg ───────────────────────────────────────
set(VCPKG_MANIFEST_MODE      ON  CACHE BOOL    "" FORCE)
set(VCPKG_TARGET_TRIPLET     "x64-windows" CACHE STRING "" FORCE)
set(CMAKE_TOOLCHAIN_FILE     "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "")

# ── B) Описание проекта ─────────────────────────────────────────────────────
project(Arkanoid LANGUAGES CXX)
set(CMAKE_CXX_STANDARD       20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── B.1) Для multi-config (VS) кладём exe/DLL в build/Debug и build/Release ──
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY     ${CMAKE_BINARY_DIR}/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY     ${CMAKE_BINARY_DIR}/$<CONFIG>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY     ${CMAKE_BINARY_DIR}/$<CONFIG>)

# ── C) Ищем SFML ─────────────────────────────────────────────────────────────
find_package(SFML COMPONENTS graphics window system CONFIG REQUIRED)

# ── D) Список исходников ────────────────────────────────────────────────────
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)
file(GLOB_RECURSE HEADERS
    "${CMAKE_SOURCE_DIR}/src/*.hpp"
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
)

# ── D.1) Include-папки ──────────────────────────────────────────────────────
target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
)

# ── E) Линкуем SFML ──────────────────────────────────────────────────────────
target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)

# ── F) Копируем ресурсы рядом с .exe ─────────────────────────────────────────
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_SOURCE_DIR}/assets"
      $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_SOURCE_DIR}/maps"
      $<TARGET_FILE_DIR:${PROJECT_NAME}>/maps
)

# ── G) Чтобы VS Debugger запускал exe из папки Runtime ──────────────────────
set_target_properties(${PROJECT_NAME} PROPERTIES
  VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)

# ── H) Оключаем внешние предупредения ───────────────────────────────────────
if (MSVC)
    # Отключаем предупреждение C26495 (type.6 — неинициализированный член Glyph)
    target_compile_options(${PROJECT_NAME} PRIVATE /wd26495)
endif()
