option(USE_TRACY "Use Tracy Profiler" OFF)
if(USE_TRACY)
    list(APPEND VCPKG_MANIFEST_FEATURES "profiling")
endif (USE_TRACY)
option(ENABLE_TESTING "Use GoogleTest" OFF)
if(ENABLE_TESTING)
    list(APPEND VCPKG_MANIFEST_FEATURES "testing")
endif (ENABLE_TESTING)

cmake_minimum_required (VERSION 3.16)
project(GPR924-Engine)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/warnings.cmake)

option(ENABLE_NETWORK "Use Photon Realtime" OFF)

if(ENABLE_TESTING)

    enable_testing()
    find_package(GTest CONFIG REQUIRED)
    include(GoogleTest)
endif ()

add_subdirectory(core)
add_subdirectory(externals)
add_subdirectory(common)
add_subdirectory(physics)

add_executable(SampleBrowser physics/samples/main.cc)
target_link_libraries(SampleBrowser PRIVATE samples)
