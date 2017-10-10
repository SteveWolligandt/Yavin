cmake_minimum_required(VERSION 3.5)
set (CMAKE_CXX_STANDARD 17)
project(yavin)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(YAVIN_MAJOR_VERSION 0)
set(YAVIN_MINOR_VERSION 1)
set(YAVIN_PATCH_VERSION 0)
set(YAVIN_VERSION
  ${YAVIN_MAJOR_VERSION}.${YAVIN_MINOR_VERSION}.${YAVIN_PATCH_VERSION})

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH
  "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/CMake/yavin)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
  "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

find_package(gl3w REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL)

configure_file(Yavin/config.h.in "${CMAKE_CURRENT_BINARY_DIR}/Yavin/config.h" @ONLY)

file(GLOB hdr Yavin/*.h)
file(GLOB src Yavin/*.cpp)

add_library(yavin SHARED ${src})

target_include_directories(yavin PUBLIC ${OPENGL_INCLUDE_DIR} ${GLM_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIRECTORIES})
target_link_libraries(yavin ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} gl3w ${GLFW_LIBRARIES})

install(TARGETS yavin
  # IMPORTANT: Add the yavin library to the "export-set"
  EXPORT yavinTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/yavin"
    COMPONENT dev)

# add_subdirectory(test)

# Add all targets to the build-tree export set
export(TARGETS yavin
  FILE "${PROJECT_BINARY_DIR}/yavinTargets.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE yavin)

# Create the yavinConfig.cmake and yavinConfigVersion files
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
   "${INSTALL_INCLUDE_DIR}")
# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(yavinConfig.cmake.in
  "${PROJECT_BINARY_DIR}/yavinConfig.cmake" @ONLY)
# ... for the install tree
set(CONF_INCLUDE_DIRS "\${YAVIN_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(yavinConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/yavinConfig.cmake" @ONLY)
# ... for both
configure_file(yavinConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/yavinConfigVersion.cmake" @ONLY)

# Install the yavinConfig.cmake and yavinConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/yavinConfig.cmake"
  "${PROJECT_BINARY_DIR}/yavinConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

# Install the export set for use with the install-tree
install(EXPORT yavinTargets DESTINATION
  "${INSTALL_CMAKE_DIR}" COMPONENT dev)
