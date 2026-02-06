set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(VCPKG_TARGET_TRIPLET "x64-windows-static" CACHE STRING "")

cmake_minimum_required(VERSION 3.20)

project(Atometa 
    VERSION 0.1.0
    DESCRIPTION "3D Chemistry Simulation Engine"
    LANGUAGES CXX
)

# ============================================================================
# Unicode/Internationalization Support
# ============================================================================

if(MSVC)
    # Full Unicode support
    add_compile_options(/utf-8)                    # Source files in UTF-8
    add_compile_definitions(UNICODE _UNICODE)      # Windows Unicode APIs
    
    # Windows 10+ features for UTF-8 locale
    add_compile_definitions(WINVER=0x0A00)         # Windows 10
    
    # Enable UTF-8 as ANSI codepage (Windows 10 build 1903+)
    add_compile_definitions(_WIN32_WINNT=0x0A00)
    
    # Disable Windows SBCS/MBCS legacy APIs
    add_compile_definitions(_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)
    add_compile_definitions(_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
    
    # Additional Unicode support
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    add_compile_definitions(_SCL_SECURE_NO_WARNINGS)
    
else()  # GCC/Clang
    # Full UTF-8 support
    add_compile_options(-finput-charset=UTF-8)     # Source file encoding
    add_compile_options(-fexec-charset=UTF-8)      # Runtime char encoding  
    add_compile_options(-fwide-exec-charset=UTF-8) # Runtime wchar_t encoding
    
    # Unicode locale support
    add_compile_options(-fvisibility=hidden)
    add_compile_definitions("_GLIBCXX_USE_CXX11_ABI=1")
endif()

# Enable ICU or libiconv if needed for advanced Unicode handling
option(ATOMETA_USE_ICU "Use ICU library for advanced Unicode support" OFF)
if(ATOMETA_USE_ICU)
    find_package(ICU COMPONENTS uc i18n REQUIRED)
    target_link_libraries(AtometaLib PUBLIC ICU::uc ICU::i18n)
endif()

# ============================================================================
# Build Configuration
# ============================================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# Build Speed Optimizations
# ============================================================================

# Use ccache if available (speeds up rebuilds significantly)
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    message(STATUS "ccache found and enabled: ${CCACHE_PROGRAM}")
endif()

# Enable parallel builds in Visual Studio
if(MSVC)
    add_compile_options(/MP)      # Multi-processor compilation
    add_compile_options(/bigobj)  # Support for large object files
    
    # Faster linking
    add_link_options(/INCREMENTAL)
endif()

# Precompiled headers option (enabled by default)
option(ATOMETA_USE_PCH "Use precompiled headers for faster compilation" ON)

# ============================================================================
# Output Directories
# ============================================================================

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ============================================================================
# Build Options
# ============================================================================

option(ATOMETA_BUILD_TESTS "Build unit tests" ON)
option(ATOMETA_BUILD_EXAMPLES "Build example simulations" ON)
option(ATOMETA_ENABLE_WARNINGS "Enable compiler warnings" ON)

# ============================================================================
# Compiler Settings
# ============================================================================

if(MSVC)
    if(ATOMETA_ENABLE_WARNINGS)
        add_compile_options(/W4)
    endif()
    
    # Optimization flags
    add_compile_options("$<$<CONFIG:Release>:/O2>")
    add_compile_options("$<$<CONFIG:Release>:/GL>")
    add_link_options("$<$<CONFIG:Release>:/LTCG>")  # Link-time code generation
    
    # Debug info
    add_compile_options($<$<CONFIG:Debug>:/Zi>)
else()
    if(ATOMETA_ENABLE_WARNINGS)
        add_compile_options(-Wall -Wextra -Wpedantic)
    endif()
    
    add_compile_options($<$<CONFIG:Release>:-O3>)
    add_compile_options($<$<CONFIG:Debug>:-g>)
endif()

# ============================================================================
# Dependencies
# ============================================================================

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Eigen3 CONFIG)

# ============================================================================
# Source Files
# ============================================================================

set(ATOMETA_CORE_SOURCES
    src/core/Application.cpp
    src/core/Window.cpp
    src/core/Input.cpp
    src/core/Logger.cpp
    src/core/Time.cpp
)

set(ATOMETA_CORE_HEADERS
    include/Atometa/Core/Application.h
    include/Atometa/Core/Window.h
    include/Atometa/Core/Input.h
    include/Atometa/Core/Logger.h
    include/Atometa/Core/Time.h
    include/Atometa/Core/Core.h
)

set(ATOMETA_CHEMISTRY_SOURCES
    src/chemistry/Atom.cpp
)

set(ATOMETA_CHEMISTRY_HEADERS
    include/Atometa/Chemistry/Atom.h
)

# ============================================================================
# Create Library
# ============================================================================

add_library(AtometaLib STATIC
    ${ATOMETA_CORE_SOURCES}
    ${ATOMETA_CORE_HEADERS}
    ${ATOMETA_CHEMISTRY_SOURCES}
    ${ATOMETA_CHEMISTRY_HEADERS}
)

target_include_directories(AtometaLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(AtometaLib PUBLIC
    glad::glad
    glfw
    glm::glm
    imgui::imgui
)

if(TARGET Eigen3::Eigen)
    target_link_libraries(AtometaLib PUBLIC Eigen3::Eigen)
endif()

target_compile_definitions(AtometaLib PRIVATE
    GLFW_INCLUDE_NONE  # This tells GLFW not to include OpenGL headers
)

# Precompiled headers for faster compilation
if(ATOMETA_USE_PCH)
    target_precompile_headers(AtometaLib PRIVATE
        <glad/glad.h>
        <GLFW/glfw3.h>
        <vector>
        <string>
        <memory>
        <unordered_map>
        <iostream>
        <glm/glm.hpp>
        <glm/gtc/matrix_transform.hpp>
        <glm/gtc/type_ptr.hpp>
    )
endif()

# ============================================================================
# Code Signing (Post-Build)
# ============================================================================

if(WIN32)
    # Option for code signing
    option(ATOMETA_CODE_SIGN "Sign the executable with digital certificate" ON)
    
    if(ATOMETA_CODE_SIGN)
        # Configure signing parameters
        set(CODE_SIGN_CERT "" CACHE FILEPATH "C:/atometa/atometa.pfx")
        set(CODE_SIGN_PASSWORD "" CACHE STRING "2423")
        set(CODE_SIGN_TIMESTAMP_URL "http://timestamp.digicert.com")
        
        if(CODE_SIGN_CERT AND EXISTS "${CODE_SIGN_CERT}")
            # Find signtool
            find_program(SIGNTOOL_EXECUTABLE
                NAMES signtool.exe
                PATHS
                "C:/Program Files (x86)/Windows Kits/10/bin"
                "C:/Program Files (x86)/Windows Kits/8.1/bin"
                "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Bin"
                DOC "C:/Program Files (x86)/Windows Kits/10/bin/10.0.26100.0/x64/signtool.exe"
            )
            
            if(SIGNTOOL_EXECUTABLE)
                # Add post-build signing
                add_custom_command(TARGET Atometa POST_BUILD
                    COMMAND "${SIGNTOOL_EXECUTABLE}" sign
                        /f "${CODE_SIGN_CERT}"
                        ${CODE_SIGN_PASSWORD}
                        /fd SHA256
                        /tr "${CODE_SIGN_TIMESTAMP_URL}"
                        /td SHA256
                        /v
                        "$<TARGET_FILE:Atometa>"
                    COMMENT "Digitally signing executable"
                    VERBATIM
                )
                message(STATUS "Code signing enabled")
            else()
                message(WARNING "signtool.exe not found - code signing disabled")
            endif()
        else()
            message(WARNING "Certificate not found - code signing disabled")
        endif()
    endif()
endif()

# ============================================================================
# Windows-specific Resources
# ============================================================================

if(WIN32)
    # Add Windows resource file
    set(WINDOWS_RESOURCES
        src/Atometa.rc
    )

    # UTF-8 for resource files (essential for Arabic/Chinese text in resources)
    add_compile_options("$<$<COMPILE_LANGUAGE:RC>:/c65001>")
    
    # Manifest will be embedded via .rc file
endif()

# ============================================================================
# Main Executable
# ============================================================================

if(WIN32)
    add_executable(Atometa 
        src/main.cpp
        ${WINDOWS_RESOURCES}
    )
else()
    add_executable(Atometa src/main.cpp)
endif()

if(WIN32)
    # For the main executable
    set_target_properties(Atometa PROPERTIES
        WIN32_EXECUTABLE TRUE
        LINK_FLAGS "/SUBSYSTEM:CONSOLE /MANIFEST:NO"
        VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Atometa>"
    )
    
    # Copy manifest file next to .rc so it can be embedded
    add_custom_command(TARGET Atometa PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/src/Atometa.manifest"
            "${CMAKE_CURRENT_BINARY_DIR}/Atometa.manifest"
        COMMENT "Copying manifest for embedding"
    )
endif()

target_link_libraries(Atometa PRIVATE AtometaLib)

# Set as startup project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Atometa)

# Copy assets to build directory
add_custom_command(TARGET Atometa POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Atometa>/assets
    COMMENT "Copying assets to build directory"
)

# ============================================================================
# Configuration Summary
# ============================================================================

message(STATUS "")
message(STATUS "==================== Atometa Configuration ====================")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "")
message(STATUS "Options:")
message(STATUS "  Build tests: ${ATOMETA_BUILD_TESTS}")
message(STATUS "  Build examples: ${ATOMETA_BUILD_EXAMPLES}")
message(STATUS "  Use PCH: ${ATOMETA_USE_PCH}")
message(STATUS "")
message(STATUS "Dependencies:")
message(STATUS "  GLFW: Found")
message(STATUS "  GLAD: Found")
message(STATUS "  GLM: Found")
message(STATUS "  ImGui: Found")
if(TARGET Eigen3::Eigen)
    message(STATUS "  Eigen3: Found")
else()
    message(STATUS "  Eigen3: Not found (optional)")
endif()
message(STATUS "")
message(STATUS "Build acceleration:")
if(CCACHE_PROGRAM)
    message(STATUS "  ccache: Enabled (faster rebuilds)")
else()
    message(STATUS "  ccache: Not found (install for faster rebuilds)")
endif()
if(MSVC)
    message(STATUS "  Multi-processor: Enabled (/MP)")
    message(STATUS "  Link-time optimization: Enabled (/LTCG for Release)")
endif()
message(STATUS "================================================================")
message(STATUS "")