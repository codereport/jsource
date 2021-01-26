#if these were Header Only you don't need to include the filenames just
#add_library(j_parent INTERFACE)

# j.h is required by many files and it requires a few things. This library contains the things required.


add_library(lib_j_parent INTERFACE)
target_include_directories(lib_j_parent INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
#target_link_libraries(lib_j_parent)