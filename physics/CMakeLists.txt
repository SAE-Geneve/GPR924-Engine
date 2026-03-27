file(GLOB_RECURSE PHYSICS_SRC include/*.h src/*.cc)
add_library(physics STATIC ${PHYSICS_SRC})
target_include_directories(physics PUBLIC include/)
target_link_libraries(physics PUBLIC core)

add_subdirectory(utils)
add_subdirectory(samples)