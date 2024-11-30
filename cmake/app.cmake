add_executable(${CMAKE_PROJECT_NAME}) # Create an executable object type
add_subdirectory(cmake/stm32cubemx) # Add STM32CubeMX generated sources

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE)

file(GLOB_RECURSE SOURCES "App/*.*")
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE # Add include paths
    App/Inc
    Lib/ngSTLib/src
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE)
target_link_libraries(${CMAKE_PROJECT_NAME} stm32cubemx)

set(HEX_FILE ${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin)

message(${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
    Building ${BIN_FILE}")

add_custom_target(program
        COMMAND
        STM32_Programmer_CLI
        -c port=SWD reset=HWrst mode=UR -w ${PROJECT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin 0x08000000 -rst
        VERBATIM)