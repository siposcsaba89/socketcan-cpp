cmake_minimum_required(VERSION 3.0)
project(socketcan_cpp)
set_property(GLOBAL PROPERTY USE_FOLDERS On) 

option(BUILD_EXAMPLES "Build socketcan_cpp examples" ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include(CheckIncludeFile)
check_include_file("linux/can.h" HAVE_LINUX_CAN_H)

if (HAVE_LINUX_CAN_H)
    message(STATUS "Found linux can headers, enabling.")
else()
    message(STATUS "Linux can headers not found, disabling")
endif()


set(PUBLIC_HDRS
    include/socketcan_cpp/socketcan_cpp.h
)

set(SRCS 
    src/socketcan_cpp.cpp
)

add_library(${PROJECT_NAME} ${PUBLIC_HDRS} ${SRCS})
add_library(::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
if (BUILD_SHARED_LIBS)
    set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)
    set_target_properties(${PROJECT_NAME} PROPERTIES VISIBILITY_INLINES_HIDDEN 1)
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX _d)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER libs)
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE "/MP")
endif()

if (HAVE_LINUX_CAN_H)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DHAVE_SOCKETCAN_HEADERS=1)
endif()

source_group(${PROJECT_NAME} FILES ${PUBLIC_HDRS})

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/gen/${PROJECT_NAME}/${PROJECT_NAME}_export.h)


target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/gen>
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

if (BUILD_EXAMPLES)
    add_executable(read_write examples/read_write.cpp)
    set_target_properties(read_write PROPERTIES DEBUG_POSTFIX _d)
    set_target_properties(read_write PROPERTIES FOLDER examples)
    target_link_libraries(read_write PRIVATE ::${PROJECT_NAME})
endif()


include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/gen/${PROJECT_NAME}-config-version.cmake"
  VERSION 0.0.1
  COMPATIBILITY AnyNewerVersion
)

configure_file(cmake/${PROJECT_NAME}-config.cmake.in ${PROJECT_NAME}-config.cmake @ONLY)
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-targets DESTINATION 
    ARCHIVE DESTINATION lib LIBRARY DESTINATION lib RUNTIME DESTINATION bin)
install(FILES 
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake 
        ${CMAKE_CURRENT_BINARY_DIR}/gen/${PROJECT_NAME}-config-version.cmake
    DESTINATION 
        lib/cmake/${PROJECT_NAME})
    
install(EXPORT ${PROJECT_NAME}-targets NAMESPACE :: DESTINATION lib/cmake/${PROJECT_NAME})

install(FILES
    ${CMAKE_BINARY_DIR}/gen/${PROJECT_NAME}/${PROJECT_NAME}_export.h
	${PUBLIC_HDRS}
    DESTINATION include/${PROJECT_NAME}/${PROJECT_NAME})
    
########### Add uninstall target ###############
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake")
  set_target_properties(uninstall PROPERTIES FOLDER CMakePredefinedTargets)



