#if these were Header Only you don't need to include the filenames just
#add_library(lib_conjunction INTERFACE)

# https://code.jsoftware.com/wiki/Vocabulary/Glossary#Conjunction
# A conjunction modifies the words or phrases to its left and right to produce a derived entity which is usually a verb,
# though it can be any of the four primary parts of speech. More...

add_library(lib_conjunctions INTERFACE)
target_include_directories(lib_conjunctions INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lib_conjunctions INTERFACE lib_j_parent)