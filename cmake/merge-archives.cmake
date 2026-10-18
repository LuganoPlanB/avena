# Copyright (C) 2026 Plan-B Foundation
# SPDX-License-Identifier: Apache-2.0
include("${manifest}")
set(output "${library}.merged")
if(msvc)
    execute_process(COMMAND "${archiver}" /NOLOGO "/OUT:${output}" "${library}" ${archives}
        COMMAND_ERROR_IS_FATAL ANY)
elseif(apple)
    find_program(libtool NAMES libtool REQUIRED)
    execute_process(COMMAND "${libtool}" -static -o "${output}" "${library}" ${archives}
        COMMAND_ERROR_IS_FATAL ANY)
else()
    # ADDLIB preserves members with duplicate basenames from different modules.
    set(mri "CREATE \"${output}\"\nADDLIB \"${library}\"\n")
    foreach(archive IN LISTS archives)
        string(APPEND mri "ADDLIB \"${archive}\"\n")
    endforeach()
    string(APPEND mri "SAVE\nEND\n")
    file(WRITE "${library}.mri" "${mri}")
    execute_process(COMMAND "${archiver}" -M INPUT_FILE "${library}.mri"
        COMMAND_ERROR_IS_FATAL ANY)
endif()
file(RENAME "${output}" "${library}")
