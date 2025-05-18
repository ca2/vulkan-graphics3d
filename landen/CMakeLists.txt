cmake_minimum_required(VERSION 3.16)
project(vulkan_graphics3d_landen CXX)


list(APPEND library_source
        vulkan/application.cpp
        vulkan/application.h
        vulkan/benchmark.hpp
        vulkan/camera.hpp
        vulkan/CMakeLists.txt
        vulkan/CommandLineParser.hpp
        vulkan/Entrypoints.h
        vulkan/frustum.hpp
        vulkan/keycodes.h
        vulkan/keycodes.hpp
        vulkan/offscreen.cpp
        vulkan/offscreen.h
        #vulkan/render_headless.cpp
        #vulkan/render_headless.h
        vulkan/shared.h
        #vulkan/swap_chain.cpp
        #vulkan/swap_chain.h
        #vulkan/swap_chain2.cpp
        #vulkan/swap_chain2.h
        vulkan/threadpool.hpp
        vulkan/vk_container.cpp
        vulkan/vk_container.h
        #vulkan/vulkan_example_base_with_swap_chain.cpp
        #vulkan/vulkan_example_base_with_swap_chain.h
        vulkan/vulkan_example_base_no_swap_chain.cpp
        vulkan/vulkan_example_base_no_swap_chain.h
        vulkan/VulkanAndroid.cpp
        vulkan/VulkanAndroid.h
        vulkan/VulkanBuffer.cpp
        vulkan/VulkanBuffer.h
        vulkan/VulkanDebug.cpp
        vulkan/VulkanDebug.h
        vulkan/VulkanDevice.cpp
        vulkan/VulkanDevice.h
        vulkan/VulkanFrameBuffer.hpp
        vulkan/VulkanglTFModel.cpp
        vulkan/VulkanglTFModel.h
        vulkan/VulkanInitializers.h
        vulkan/VulkanInitializers.hpp
        #vulkan/VulkanRaytracingSample.cpp
        #vulkan/VulkanRaytracingSample.h
        #vulkan/VulkanSwapChain.cpp
        #vulkan/VulkanSwapChain.h
        #vulkan/VulkanTexture.cpp
        #vulkan/VulkanTexture.h
        vulkan/VulkanTools.cpp
        vulkan/VulkanTools.h
        vulkan/VulkanUIOverlay.cpp
        vulkan/VulkanUIOverlay.h
   _.cpp
   _.h
   application.cpp
   application.h
#   child_frame.cpp
#   child_frame.h
   document.cpp
   document.h
   #edit_impact.cpp
   #edit_impact.h
   frame.cpp
   frame.h
   framework.cpp
   framework.h
   impact.cpp
   impact.h
   #main_impact.cpp
   #main_impact.h
   #pane_impact.cpp
   #pane_impact.h
   #toggle_impact.cpp
   #toggle_impact.h
   #top_impact.cpp
   #top_impact.h
   #window.cpp
   #window.h
)


list(APPEND library_references
        acme
        apex
        aqua
        aura
        axis
        base
        bred
        core
        imgui
)


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
   target_compile_definitions(${PROJECT_NAME} PRIVATE VK_USE_PLATFORM_WIN32_KHR)
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

   target_compile_definitions(${PROJECT_NAME} PRIVATE VK_USE_PLATFORM_WAYLAND_KHR VK_USE_PLATFORM_XCB_KHR VK_USE_PLATFORM_WAYLAND_OR_XCB_KHR)
endif ()


list(APPEND library_include_directories ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND library_include_directories ${CMAKE_CURRENT_SOURCE_DIR}/..)
list(APPEND library_include_directories ${CMAKE_CURRENT_SOURCE_DIR}/../..)

target_include_directories(${PROJECT_NAME} PRIVATE ${library_include_directories})
target_include_directories(static_${PROJECT_NAME} PRIVATE ${library_include_directories})


target_compile_definitions(${PROJECT_NAME} PRIVATE _${PROJECT_NAME}_project)


add_subdirectory(__implement)



