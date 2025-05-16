cmake_minimum_required(VERSION 3.16)
project(_vulkan_land_landen CXX)



list(APPEND application_source main.cpp)


if(MSVC)
    list(APPEND application_source ../deployment.rc main.rc)
endif()


add_executable(${PROJECT_NAME} ${application_source})




if(${LINUX} OR ${__BSD__})


   add_custom_command(
      OUTPUT ${PROJECT_SOURCE_DIR}/../.link_object/_matter.zip.o
      COMMAND ${APPLICATION_BUILD_HELPER_BINARY} --prepare-application ${PROJECT_SOURCE_DIR}
      COMMENT "Generating ${PROJECT_SOURCE_DIR}/../.link_object/_matter.zip.o"
   )

   target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/../.link_object/_matter.zip.o)

endif()



list(APPEND executable_references
   acme
   apex
   aqua
   aura
   axis
   base
   bred
   core
   vulkan_land_landen)


if (MSVC)

   target_compile_options(${PROJECT_NAME} PRIVATE "/MD$<$<CONFIG:Debug>:d>")
   target_link_options(${PROJECT_NAME} PRIVATE "/DEFAULTLIB:msvcrt$<$<CONFIG:Debug>:d>")
   target_link_options(${PROJECT_NAME} PRIVATE "/SUBSYSTEM:WINDOWS")
   target_link_libraries(${PROJECT_NAME} PRIVATE ${executable_references})

elseif(${LINUX} OR ${__BSD__})

   target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fpermissive  ${EXTRA_CXX_TARGET_COMPILER_OPTIONS}>)
   #target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-rpath=\$ORIGIN")
   target_link_libraries(${PROJECT_NAME} PRIVATE  ${executable_references})
   target_link_options(${PROJECT_NAME} PRIVATE -Wl,-z,defs)

endif ()


add_dependencies(${PROJECT_NAME} ${app_common_dependencies})
add_dependencies(${PROJECT_NAME} experience_core)
add_dependencies(${PROJECT_NAME} ${default_draw2d})
add_dependencies(${PROJECT_NAME} ${default_imaging})
add_dependencies(${PROJECT_NAME} ${default_write_text})
add_dependencies(${PROJECT_NAME} database_sqlite3)
add_dependencies(${PROJECT_NAME} networking_bsd)
add_dependencies(${PROJECT_NAME} regular_expression_pcre2)
add_dependencies(${PROJECT_NAME} crypto_openssl)


list(APPEND executable_include_directories ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND executable_include_directories ${CMAKE_CURRENT_SOURCE_DIR}/..)
list(APPEND executable_include_directories ${CMAKE_CURRENT_SOURCE_DIR}/../..)

target_include_directories(${PROJECT_NAME} PRIVATE ${executable_include_directories})


target_link_directories(${PROJECT_NAME} PRIVATE ${library_link_directories})

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)





