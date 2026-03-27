file(GLOB_RECURSE SAMPLES_SRC include/*.h src/*.cc)
add_library(samples STATIC ${SAMPLES_SRC})
target_include_directories(samples PUBLIC include/)
target_link_libraries(samples PUBLIC utils)