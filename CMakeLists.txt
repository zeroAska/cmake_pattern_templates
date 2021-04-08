set(PROJECT_NAME cmake_pattern_templates)
set(PROJECT_VERSION 1.0)
if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)
cmake_minimum_required(VERSION 3.8)

##################################
#  project
##################################
project(cmake_pattern_templates VERSION ${PROJECT_VERSION})

set(CMAKE_VERBOSE_MAKEFILE ON)

# Release Mode
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

## build output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)
LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

find_package(Boost REQUIRED COMPONENTS filesystem system chrono)

######################################
# add libraries
######################################
add_library(lib1 SHARED ${PROJECT_SOURCE_DIR}/src/lib1.cpp)
target_include_directories(lib1 PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}>
  $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}> )
target_link_libraries(lib1 PUBLIC  Boost::filesystem)
set_target_properties(lib1 PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

add_library(lib2 SHARED ${PROJECT_SOURCE_DIR}/src/lib2.cpp)
add_dependencies(lib2 lib1)
target_include_directories(lib2 PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}> 
        $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/${PROJECT_NAME}-${${PROJECT_NAME}_VERSION}> )
target_link_libraries(lib2 PRIVATE Boost::chrono  lib1)
set_target_properties(lib2 PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

#######################################
# add executables
#######################################
add_executable(test1 ${PROJECT_SOURCE_DIR}/test/test1.cpp)
target_include_directories(test1 PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}>)
target_link_libraries(test1 PRIVATE lib2)


#######################################
# install libraries
#######################################
set(${PROJECT_NAME}_INCLUDE_DEST "include/${PROJECT_NAME}-${PROJECT_VERSION}")
set(${PROJECT_NAME}_LIB_DEST "lib/${PROJECT_NAME}-${PROJECT_VERSION}")
set(PROJECT_EXPORT_TARGETS ${PROJECT_NAME}Targets)

# export cmake targets to the build/cmake/
message("Project export target is ${PROJECT_EXPORT_TARGETS}.cmake")
message("Project install lib dest is ${${PROJECT_NAME}_LIB_DEST}")
export(
  TARGETS lib1 lib2
  NAMESPACE  ${PROJECT_NAME}::
  FILE ${PROJECT_EXPORT_TARGETS}.cmake
  )

install(
    # targets to install
    TARGETS lib1 lib2
    # name of the CMake "export group" containing the targets we want to install
    EXPORT ${PROJECT_EXPORT_TARGETS}  #${PROJECT_NAME}Targets
    # Dynamic, static library and include destination locations after running
    # "make install"
    LIBRARY DESTINATION ${${PROJECT_NAME}_LIB_DEST}
    ARCHIVE DESTINATION ${${PROJECT_NAME}_LIB_DEST}
    INCLUDES DESTINATION ${${PROJECT_NAME}_INCLUDE_DEST}
    PUBLIC_HEADER DESTINATION "include/${PROJECT_NAME}-${PROJECT_VERSION}" 
    )

install(
  # The export we want to save (matches name defined above containing the
  # install targets)
  EXPORT ${PROJECT_EXPORT_TARGETS}
  # CMake file in which to store the export's information
  FILE   ${PROJECT_EXPORT_TARGETS}.cmake
  # Namespace prepends all targets in the export (when we import later, we
  # will use CMakeDemo::CMakeDemo)
  NAMESPACE ${PROJECT_NAME}::
  # where to place the resulting file (here, we're putting it with the library)
  DESTINATION cmake/ #${${PROJECT_NAME}_LIB_DEST}
  )  

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
  )
configure_package_config_file(
  ${PROJECT_NAME}Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION  cmake/
  PATH_VARS PROJECT_NAME
  )


install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}
  DESTINATION ${${PROJECT_NAME}_INCLUDE_DEST})


# Write a "version file" in case someone wants to only load a particular version 
  
install(
  FILES 
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  #DESTINATION ${${PROJECT_NAME}_LIB_DEST}/cmake/
  DESTINATION cmake/
  )

# Configure config file for local build tree
#configure_file(${PROJECT_NAME}Config.cmake.in
#  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" @ONLY)





