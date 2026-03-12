file(GLOB_RECURSE PHOTON_SRC *.h LoadBalancing-cpp/src/*.cpp)
add_library(PhotonLib STATIC ${PHOTON_SRC})
target_include_directories(PhotonLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC)
    target_link_libraries(PhotonLib PUBLIC
            optimized "${CMAKE_CURRENT_SOURCE_DIR}/Common-cpp/lib/windows/Common-cpp_vc17_release_windows_md_x64.lib"
            debug "${CMAKE_CURRENT_SOURCE_DIR}/Common-cpp/lib/windows/Common-cpp_vc17_debug_windows_md_x64.lib"
            optimized "${CMAKE_CURRENT_SOURCE_DIR}/Photon-cpp/lib/windows/Photon-cpp_vc17_release_windows_md_x64.lib"
            debug "${CMAKE_CURRENT_SOURCE_DIR}/Photon-cpp/lib/windows/Photon-cpp_vc17_debug_windows_md_x64.lib"
    )
elseif(APPLE)
    target_compile_definitions(PhotonLib PUBLIC _EG_IMAC_PLATFORM=1)
    find_library(corefoundation_lib CoreFoundation)
    find_library(cfnetwork_lib CFNetwork)
    target_link_libraries(PhotonLib PUBLIC
            optimized "${CMAKE_CURRENT_SOURCE_DIR}/Common-cpp/lib/macosx/libCommon-cpp_release_macosx.a"
            debug "${CMAKE_CURRENT_SOURCE_DIR}/Common-cpp/lib/macosx/libCommon-cpp_debug_macosx.a"
            optimized "${CMAKE_CURRENT_SOURCE_DIR}/Photon-cpp/lib/macosx/libPhoton-cpp_release_macosx.a"
            debug "${CMAKE_CURRENT_SOURCE_DIR}/Photon-cpp/lib/macosx/libPhoton-cpp_debug_macosx.a"
            optimized "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libcrypto_release_macosx.a"
            debug "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libcrypto_debug_macosx.a"
            "-framework Foundation" "-framework CoreFoundation" "-framework Security" "-framework CFNetwork"
    )
elseif(UNIX)
    set_property(TARGET PhotonLib PROPERTY POSITION_INDEPENDENT_CODE ON)
    target_compile_definitions(PhotonLib PUBLIC _EG_LINUX_PLATFORM=1)
    target_link_libraries(PhotonLib PUBLIC
            optimized "${CMAKE_CURRENT_SOURCE_DIR}/Photon-cpp/lib/linux/libPhotonRelease64.a"
            debug "${CMAKE_CURRENT_SOURCE_DIR}/Photon-cpp/lib/linux/libPhotonDebug64.a"
            optimized "${CMAKE_CURRENT_SOURCE_DIR}/Common-cpp/lib/linux/libCommonRelease64.a"
            debug "${CMAKE_CURRENT_SOURCE_DIR}/Common-cpp/lib/linux/libCommonDebug64.a"
    )
endif()


# --- Read Photon AppId from photon.cfg ---
set(PHOTON_CFG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/photon.cfg")
if(NOT EXISTS "${PHOTON_CFG_FILE}")
    message(FATAL_ERROR
            "Missing ${PHOTON_CFG_FILE}\n"
            "Copy photon.cfg.example to photon.cfg and set your Photon AppId.")
endif()
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${PHOTON_CFG_FILE}")

file(STRINGS "${PHOTON_CFG_FILE}" PHOTON_CFG_LINES)
foreach(LINE IN LISTS PHOTON_CFG_LINES)
    if(LINE MATCHES "^AppId=(.+)$")
        set(PHOTON_APP_ID "${CMAKE_MATCH_1}")
    endif()
endforeach()
string(STRIP "${PHOTON_APP_ID}" PHOTON_APP_ID)

if(NOT DEFINED PHOTON_APP_ID OR PHOTON_APP_ID STREQUAL "")
    message(FATAL_ERROR "AppId not found in ${PHOTON_CFG_FILE}")
endif()
message(STATUS "Photon AppId: ${PHOTON_APP_ID}")

target_compile_definitions(PhotonLib PUBLIC PHOTON_APP_ID="${PHOTON_APP_ID}")