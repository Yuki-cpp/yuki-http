cmake_minimum_required(VERSION 3.9.0)
set(CMAKE_VERBOSE_MAKEFILE OFF)

project(yukihttp VERSION 1.0.0 LANGUAGES CXX)


add_library(yukihttp SHARED src/request.cpp src/mime.cpp src/mime_part.cpp src/slist_wrapper.cpp src/utils.cpp)

target_compile_features(yukihttp PRIVATE cxx_std_17)
target_compile_options(yukihttp PRIVATE -Wall -Wextra -Wpedantic -Wunused-parameter -Wunreachable-code)

target_include_directories(yukihttp
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)



find_package(CURL 7.56 REQUIRED)
target_link_libraries(yukihttp
    PRIVATE
        ${CURL_LIBRARIES}
)


# Installation instructions
include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/yukiHTTP)

install(TARGETS yukihttp
    EXPORT yukihttp-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)



#This is required so that the exported target has the name yukiHTTP and not yukihttp
set_target_properties(yukihttp PROPERTIES EXPORT_NAME yukiHTTP)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})


#Export the targets to a script
install(EXPORT yukihttp-targets
  FILE
    yukiHTTPTargets.cmake
  NAMESPACE
    yukiHTTP::
  DESTINATION
    ${INSTALL_CONFIGDIR}
)

#Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/yukiHTTPConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/yukiHTTPConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/yukiHTTPConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)


#Install the config, configversion and custom find modules
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/yukiHTTPConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/yukiHTTPConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)


##############################################
## Exporting from the build tree
export(EXPORT yukihttp-targets FILE ${CMAKE_CURRENT_BINARY_DIR}/yukiHTTPTargets.cmake NAMESPACE yukiHTTP::)

#Register package in user's package registry
export(PACKAGE yukiHTTP)
