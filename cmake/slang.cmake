function(target_add_slang_shaders TARGET)
    cmake_parse_arguments(SHADER "" "" "SOURCES" ${ARGN})

    set(outputs "")

    foreach(src ${SHADER_SOURCES})
        # Get only the filename without extension
        get_filename_component(name ${src} NAME_WE)
        get_filename_component(dir ${src} DIRECTORY)

        set(SHADERS_DIR_OUT "${dir}/bin")
        file(MAKE_DIRECTORY ${SHADERS_DIR_OUT})

        # Output SPIR-V file for THIS shader
        set(out ${SHADERS_DIR_OUT}/${name}.spv)

        add_custom_command(
            OUTPUT ${out}
            COMMAND ${SLANGC_EXECUTABLE} ${src}
                -target spirv
                -profile spirv_1_4
                -emit-spirv-directly
                -fvk-use-entrypoint-name
                -entry vert_main 
                -entry frag_main
                -o ${out}
            DEPENDS ${src}
            COMMENT "Compiling shader ${src} -> ${out}"
            VERBATIM
        )

        list(APPEND outputs ${out})
    endforeach()

    add_custom_target(${TARGET} DEPENDS ${outputs})
endfunction()

