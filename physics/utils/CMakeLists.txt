file(GLOB_RECURSE UTILS_SRC include/*.h src/*.cc src/*.cpp)
add_library(utils STATIC ${UTILS_SRC})
target_include_directories(utils PUBLIC include/)
target_link_libraries(utils PUBLIC physics common)