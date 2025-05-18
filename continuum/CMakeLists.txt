cmake_minimum_required(VERSION "3.13")
project(vulkan_graphics3d_continuum CXX)


set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

#add_subdirectory("vendor/glfw")

find_package (Vulkan REQUIRED)
#set(CMAKE_CXX_STANDARD 17)

set(Vulkan_INCLUDE_DIR "C:/VulkanSDK/1.4.313.0/Include")
set(Vulkan_LIBRARY "C:/VulkanSDK/1.4.313.0/Lib/vulkan-1.lib")
include_directories(${Vulkan_INCLUDE_DIR}) 

list(APPEND library_source
        # src
        #src/main.cpp
        AppCore/vk_application.cpp
        AppCore/vk_window.cpp

        Renderer/vk_renderer.cpp

        VK_abstraction/vk_initializers.cpp
        VK_abstraction/vk_initializers.h
        VK_abstraction/vk_device.cpp
        VK_abstraction/vk_device.h
        VK_abstraction/vk_swapchain.cpp
        VK_abstraction/vk_swapchain.h
        VK_abstraction/vk_descriptors.cpp
        VK_abstraction/vk_descriptors.h
        VK_abstraction/vk_pipeline.cpp
        VK_abstraction/vk_pipeline.h
        VK_abstraction/vk_renderpass.cpp
        VK_abstraction/vk_renderpass.h
        VK_abstraction/vk_offscreen.cpp
        VK_abstraction/vk_offscreen.h
../../../port/graphics3d/vkbootstrap/VkBootstrap.cpp

        # vendor
        Renderer/vk_buffer.h
        Renderer/vk_buffer.cpp
        Renderer/Types/vk_basicRenderSystem.cpp
        Renderer/Types/vk_basicRenderSystem.h
        VK_abstraction/vk_camera.cpp
        VK_abstraction/vk_camera.h
        VK_abstraction/vk_model.cpp
        VK_abstraction/vk_model.h
        VK_abstraction/vk_gameObject.cpp
        VK_abstraction/vk_gameObject.h
        VK_abstraction/vk_input.cpp
        VK_abstraction/vk_input.h
        Renderer/Types/vk_pointLightSystem.cpp
        Renderer/Types/vk_pointLightSystem.h
        _.cpp
        _.h
        application.cpp
        application.h
        document.cpp
        document.h
        framework.cpp
        framework.h
        impact.cpp
        impact.h
        main_frame.cpp
        main_frame.h
        pane_impact.cpp
        pane_impact.h
)


## Define PROJECT_ROOT_DIR for use in the code
#target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")


# Include Directories
##target_include_directories(VKContinuum PUBLIC
#    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm"
#    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb"
#    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW"
#    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/tinyobjloader"
#    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/vkbootstrap"
#    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/vma"
#    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/vk_video"
#    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/tinygltf"
#    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui"
#    "${CMAKE_CURRENT_SOURCE_DIR}/src"
#)

# Link vendor libs
list(APPEND library_references
    acme
    apex
    aqua
    aura
    axis
    base
    bred
    core
    vulkan_graphics3d_landen
)


# Shader Compilation
file(GLOB SHADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/../_matter/continuum/_std/_std/shaders/*.vert"
        "${CMAKE_CURRENT_SOURCE_DIR}/../_matter/continuum/_std/_std/shaders/*.frag")


add_library(${PROJECT_NAME} SHARED ${library_source})
add_library(static_${PROJECT_NAME} STATIC ${library_source})


if (NOT ${NO_PRECOMPILED_HEADER})

    target_precompile_headers(${PROJECT_NAME} PRIVATE framework.h)
    target_precompile_headers(static_${PROJECT_NAME} PRIVATE framework.h)

endif ()

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE "/MD$<$<CONFIG:Debug>:d>")
    target_link_options(${PROJECT_NAME} PRIVATE "/DEFAULTLIB:msvcrt$<$<CONFIG:Debug>:d>")
    target_link_options(${PROJECT_NAME} PRIVATE "/SUBSYSTEM:WINDOWS")
    target_link_libraries(${PROJECT_NAME} PRIVATE acme apex aqua aura axis base)
elseif (${LINUX})

    #if(USE_PKGCONFIG)
    pkg_check_modules(PKGCONFIG vulkan)
    #endif()


    target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fpermissive  ${EXTRA_CXX_TARGET_COMPILER_OPTIONS}>)


    target_compile_options(${PROJECT_NAME} PRIVATE ${PKGCONFIG_CFLAGS})
    target_compile_options(${PROJECT_NAME} PRIVATE ${PKGCONFIG_CFLAGS})
    target_link_options(${PROJECT_NAME} PRIVATE ${PKGCONFIG_LDFLAGS})

    target_link_libraries(${PROJECT_NAME} PRIVATE ${library_references} ${global_library_references} ${PKGCONFIG_LIBRARIES})
    target_link_options(${PROJECT_NAME} PRIVATE -Wl,-z,defs)
    # target_link_options(static_${PROJECT_NAME} PRIVATE --whole-archive)


    message(STATUS "(C) PKGCONFIG_LIBRARIES is " ${PKGCONFIG_LIBRARIES})

endif ()


add_dependencies(${PROJECT_NAME} ${app_common_dependencies})
add_dependencies(${PROJECT_NAME} ${default_draw2d})
add_dependencies(${PROJECT_NAME} ${default_imaging})
add_dependencies(${PROJECT_NAME} ${default_networking})
add_dependencies(${PROJECT_NAME} ${default_write_text})
add_dependencies(${PROJECT_NAME} database_sqlite3)

set(SPIRV_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}../_matter/continuum/_std/_std/shaders/spirV")
file(MAKE_DIRECTORY ${SPIRV_OUTPUT_DIR})

foreach(SHADER_FILE ${SHADER_FILES})
    get_filename_component(FILE_NAME ${SHADER_FILE} NAME)
    add_custom_command(
            OUTPUT ${SPIRV_OUTPUT_DIR}/${FILE_NAME}.spv
            COMMAND ${CMAKE_COMMAND} -E echo "Compiling shader: ${FILE_NAME}"
            COMMAND glslangValidator -V ${SHADER_FILE} -o ${SPIRV_OUTPUT_DIR}/${FILE_NAME}.spv
            DEPENDS ${SHADER_FILE}
            COMMENT "Compiling ${FILE_NAME} to SPIR-V"
    )
    list(APPEND SPIRV_BINARIES ${SPIRV_OUTPUT_DIR}/${FILE_NAME}.spv)
endforeach()

add_custom_target(CompileShaders ALL DEPENDS ${SPIRV_BINARIES})

# Ensure shaders are compiled before the VKContinuum executable
add_dependencies(${PROJECT_NAME} CompileShaders)





list(APPEND library_include_directories ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND library_include_directories ${CMAKE_CURRENT_SOURCE_DIR}/..)
list(APPEND library_include_directories ${CMAKE_CURRENT_SOURCE_DIR}/../..)

target_include_directories(${PROJECT_NAME} PRIVATE ${library_include_directories})
target_include_directories(static_${PROJECT_NAME} PRIVATE ${library_include_directories})


target_compile_definitions(${PROJECT_NAME} PRIVATE _${PROJECT_NAME}_project)

add_subdirectory(__implement)