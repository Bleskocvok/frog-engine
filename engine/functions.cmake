
function(asset FILENAME)
    message(STATUS "ASSET ${FILENAME}")
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}
        ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}
        COPYONLY
    )
endfunction()


function(assets)
    foreach(F IN LISTS ARGV)
        asset(${F})
    endforeach()
endfunction()


function(place_engine_assets FOLDER)
    foreach(F IN LISTS FROG_3D_ASSETS)
        get_filename_component(FILENAME ${F} NAME)
        message(STATUS "ENGINE ASSET ${F}")
        configure_file(${F} ${FOLDER}/${FILENAME} COPYONLY)
    endforeach()
endfunction()
