
function(setup_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE "/W4" "/permissive-" "/WX")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${target} PRIVATE "-Wall" "-Wextra" "-Wshadow"
                "-Wnon-virtual-dtor" "-Wdouble-promotion" "-pedantic" "-Werror" "-Woverloaded-virtual"
                "-Wold-style-cast" "-Wconversion" "-Wsign-conversion" "-Wimplicit-fallthrough")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(FATAL_ERROR "GCC is not supported, please use clang with export CC=clang; export CXX=clang++ for example")
    endif()
endfunction()