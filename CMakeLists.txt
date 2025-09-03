# =============================================================================
# Black Hole Raytracer - CMake Build System
# =============================================================================
# 
# This CMake configuration provides a modern, cross-platform build system
# for the black hole raytracing engine with multiple build types and
# comprehensive testing support.
#
# @author Maxwell Corwin
# @date 2025
# @version 2.0
# =============================================================================

cmake_minimum_required(VERSION 3.16)
project(BlackHoleRaytracer VERSION 2.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Project metadata
set(PROJECT_DESCRIPTION "A C++ raytracing engine that simulates gravitational lensing effects around black holes")
set(PROJECT_HOMEPAGE_URL "https://github.com/mcorwin17/Blackhole-Rendering")
set(PROJECT_LICENSE "MIT")

# Build options
option(BUILD_TESTS "Build test suite" ON)
option(BUILD_BENCHMARKS "Build performance benchmarks" OFF)
option(BUILD_DOCS "Build documentation" OFF)
option(ENABLE_OPENMP "Enable OpenMP parallelization" OFF)
option(ENABLE_CUDA "Enable CUDA acceleration" OFF)
option(ENABLE_OPENCL "Enable OpenCL acceleration" OFF)

# Compiler-specific flags
if(MSVC)
    add_compile_options(/W4 /WX)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wshadow -Wconversion)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-fdiagnostics-color=always)
    endif()
endif()

# Optimization flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -DDEBUG")

# Find required packages
find_package(Threads REQUIRED)

# Optional packages
if(ENABLE_OPENMP)
    find_package(OpenMP)
    if(OpenMP_CXX_FOUND)
        message(STATUS "OpenMP found: ${OpenMP_CXX_VERSION}")
    endif()
endif()

if(ENABLE_CUDA)
    find_package(CUDA)
    if(CUDA_FOUND)
        message(STATUS "CUDA found: ${CUDA_VERSION}")
        enable_language(CUDA)
        set(CMAKE_CUDA_STANDARD 17)
    endif()
endif()

if(ENABLE_OPENCL)
    find_package(OpenCL)
    if(OpenCL_FOUND)
        message(STATUS "OpenCL found")
    endif()
endif()

# Source files
set(SOURCES
    main.cc
)

set(HEADERS
    blackhole_renderer.h
    config.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Set target properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    DESCRIPTION ${PROJECT_DESCRIPTION}
    HOMEPAGE_URL ${PROJECT_HOMEPAGE_URL}
    OUTPUT_NAME "blackhole"
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Threads::Threads
)

if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
endif()

# Compiler definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
    $<$<CONFIG:RelWithDebInfo>:NDEBUG>
    $<$<CONFIG:MinSizeRel>:NDEBUG>
)

# Installation
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(FILES ${HEADERS}
    DESTINATION include/${PROJECT_NAME}
)

# Testing
if(BUILD_TESTS)
    enable_testing()
    
    # Add test executable
    add_executable(${PROJECT_NAME}_tests
        tests/test_main.cc
        tests/test_vec3.cc
        tests/test_color.cc
        tests/test_camera.cc
        tests/test_blackhole.cc
    )
    
    target_link_libraries(${PROJECT_NAME}_tests PRIVATE ${PROJECT_NAME})
    
    # Add tests
    add_test(NAME Vec3Tests COMMAND ${PROJECT_NAME}_tests --gtest_filter="Vec3*")
    add_test(NAME ColorTests COMMAND ${PROJECT_NAME}_tests --gtest_filter="Color*")
    add_test(NAME CameraTests COMMAND ${PROJECT_NAME}_tests --gtest_filter="Camera*")
    add_test(NAME BlackHoleTests COMMAND ${PROJECT_NAME}_tests --gtest_filter="BlackHole*")
endif()

# Benchmarks
if(BUILD_BENCHMARKS)
    find_package(benchmark)
    if(benchmark_FOUND)
        add_executable(${PROJECT_NAME}_benchmarks
            benchmarks/benchmark_main.cc
            benchmarks/benchmark_raytracing.cc
        )
        target_link_libraries(${PROJECT_NAME}_benchmarks PRIVATE benchmark::benchmark)
    endif()
endif()

# Documentation
if(BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
        
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        
        add_custom_target(docs ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
        )
    endif()
endif()

# Package configuration
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Export targets
export(EXPORT ${PROJECT_NAME}Targets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake"
    NAMESPACE ${PROJECT_NAME}::
)

# CPack configuration
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_VENDOR "Maxwell Corwin")
set(CPACK_PACKAGE_CONTACT "maxwellcorwin13@gmail.com")
set(CPACK_PACKAGE_HOMEPAGE_URL ${PROJECT_HOMEPAGE_URL})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

if(WIN32)
    set(CPACK_GENERATOR "NSIS;ZIP")
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop;TGZ")
else()
    set(CPACK_GENERATOR "DEB;RPM;TGZ")
endif()

include(CPack)

# Print configuration summary
message(STATUS "")
message(STATUS "Black Hole Raytracer Configuration Summary")
message(STATUS "=========================================")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build tests: ${BUILD_TESTS}")
message(STATUS "Build benchmarks: ${BUILD_BENCHMARKS}")
message(STATUS "Build docs: ${BUILD_DOCS}")
message(STATUS "OpenMP: ${OpenMP_CXX_FOUND}")
message(STATUS "CUDA: ${CUDA_FOUND}")
message(STATUS "OpenCL: ${OpenCL_FOUND}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
