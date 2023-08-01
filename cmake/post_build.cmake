# generate keys
if (WIN32)
    execute_process(
            COMMAND cmd /c ${CMAKE_CURRENT_SOURCE_DIR}/scripts/gen_keys.bat
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
elseif(UNIX)
    execute_process(
            COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/scripts/gen_keys.sh
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endif()

# copy files and dirs
add_custom_target(copy_files ALL)

add_custom_command(
    TARGET copy_files POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res/templates
        ${CMAKE_CURRENT_BINARY_DIR}/templates
    COMMENT "Copying HTML template dir"
)

add_custom_command(
    TARGET copy_files POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res/css
        ${CMAKE_CURRENT_BINARY_DIR}/css
    COMMENT "Copying CSS dir"
)

add_custom_command(
        TARGET copy_files POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/res/config_default.ini
        ${CMAKE_CURRENT_BINARY_DIR}/config_default.ini
        COMMENT "Copying config_default.ini -> config_default.ini"
)

add_custom_command(
        TARGET copy_files POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/res/config_default.ini
        ${CMAKE_CURRENT_BINARY_DIR}/config.ini
        COMMENT "Copying config_default.ini -> config.ini"
)


