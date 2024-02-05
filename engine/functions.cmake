
function(asset FILENAME)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}
        ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}
        COPYONLY
    )
endfunction()
