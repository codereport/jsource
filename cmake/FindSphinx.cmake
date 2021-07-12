find_package (Python3 COMPONENTS Interpreter Development REQUIRED)

execute_process(
        COMMAND ${Python3_EXECUTABLE}  "-c" "exec(\"import sphinx\\nimport os\\nprint (os.path.dirname(sphinx.__file__))\")"
        RESULT_VARIABLE EXIT_CODE
        OUTPUT_VARIABLE PIP3_SPHINX_DATA
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS ${Python3_EXECUTABLE})
message(STATUS ${PIP3_SPHINX_DATA}/bin)
find_program(SPHINX_EXECUTABLE NAMES sphinx-build
        HINTS ${PIP3_SPHINX_DATA} ~/.local/bin
        DOC "Sphinx documentation generator"
        )

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(Sphinx DEFAULT_MSG
        SPHINX_EXECUTABLE
        )

mark_as_advanced(SPHINX_EXECUTABLE)