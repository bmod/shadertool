

macro(qt5_copy_dll APP DLL)
    # find the release *.dll file
    get_target_property(Qt5_${DLL}Location Qt5::${DLL} LOCATION)
    # find the debug *d.dll file
    get_target_property(Qt5_${DLL}LocationDebug Qt5::${DLL} IMPORTED_LOCATION_DEBUG)

    add_custom_command(TARGET ${APP} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<$<CONFIG:Debug>:${Qt5_${DLL}LocationDebug}>
            $<$<NOT:$<CONFIG:Debug>>:${Qt5_${DLL}Location}>
            $<TARGET_FILE_DIR:${APP}>)
endmacro()