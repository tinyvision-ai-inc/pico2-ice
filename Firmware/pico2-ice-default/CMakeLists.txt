cmake_minimum_required(VERSION 3.13)

# import the pico-sdk
set(PICO_SDK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/pico-sdk/)
include(pico_sdk_import.cmake)

# Configure the build based on the selected pico ice board
# Pass board in make "cmake -DPICO_ICE_BOARD=pico-ice ..." or "cmake -DPICO_ICE_BOARD=pico-ice ..."
if(PICO_ICE_BOARD STREQUAL "pico-ice")    
    project(pico_ice_default C CXX ASM)
elseif(PICO_ICE_BOARD STREQUAL "pico2-ice")
    project(pico2_ice_default C CXX ASM)
else()
    message(STATUS "Pico ice not passed in cmake.")
endif()

# configure the pico-sdk project
#project(pico2_ice_default C CXX ASM)
pico_sdk_init()

# add the pico-ice-sdk
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/pico-ice-sdk/)
add_compile_definitions(VERSION=\"${PICO_ICE_SDK_VERSION}\")

# add the local files
add_executable(${CMAKE_PROJECT_NAME}
    main.c
    usb_descriptors.c
    )


# Configure the build based on the selected pico ice board
# Pass board in make "cmake -DPICO_ICE_BOARD=pico-ice ..." or "cmake -DPICO_ICE_BOARD=pico-ice ..."
if(PICO_ICE_BOARD STREQUAL "pico-ice")    
    target_compile_definitions(${PROJECT_NAME} PRIVATE PICO_ICE_BOARD_ID=1)
    message(STATUS "Configuring for pico-ice (rp2040)")
elseif(PICO_ICE_BOARD STREQUAL "pico2-ice")
    target_compile_definitions(${PROJECT_NAME} PRIVATE PICO_ICE_BOARD_ID=2)
    message(STATUS "Configuring for pico2-ice (rp2350)")
else()
    message(STATUS "Pico ice not passed in cmake.")
#    target_compile_definitions(${PROJECT_NAME} PRIVATE PICO_ICE_BOARD_ID=2)
#    message(STATUS "Pico ice not passed in cmake. It will use default pico-ice (rp2040)")
endif()

target_link_libraries(${CMAKE_PROJECT_NAME}
    pico_ice_sdk
    pico_ice_usb
    pico_stdio_usb
    )
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    )
pico_add_extra_outputs(${CMAKE_PROJECT_NAME})
pico_enable_stdio_usb(${CMAKE_PROJECT_NAME} 1)
pico_enable_stdio_uart(${CMAKE_PROJECT_NAME} 0)
