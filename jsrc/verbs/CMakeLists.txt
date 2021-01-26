#if these were Header Only you don't need to include the filenames just
#add_library(lib_verbs INTERFACE)

# https://code.jsoftware.com/wiki/Vocabulary/Verbs
# Verbs are programs. Verbs operate on one or two noun operands, and produce a result which is always a noun.
# The simplest verbs are the primitives, such as +. Larger verbs are created by connecting other verbs together.

add_library(lib_verbs INTERFACE)
target_include_directories(lib_verbs INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lib_verbs INTERFACE lib_j_parent)