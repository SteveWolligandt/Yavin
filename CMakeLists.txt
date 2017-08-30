cmake_minimum_required (VERSION 2.6)
project (yavin)
set(CMAKE_CXX_STANDARD 17)

file(GLOB hdr Yavin/*.h)
file(GLOB src Yavin/*.cpp)
set(all_src ${src} ${hdr})
# set(src Yavin/ComputeShader.cpp Yavin/FragmentShader.cpp Yavin/GeometryShader.cpp Yavin/GLSLVar.cpp Yavin/gl_wrapper.cpp Yavin/IndexBuffer.cpp Yavin/Mesh.cpp Yavin/Shader.cpp Yavin/ShaderStage.cpp Yavin/ShaderStageParser.cpp Yavin/TesselationControlShader.cpp Yavin/TesselationEvaluationShader.cpp Yavin/tiny_obj_loader.cpp Yavin/VertexArray.cpp Yavin/VertexShader.cpp Yavin/Window.cpp Yavin/Texture2D.cpp Yavin/FrameBuffer.cpp Yavin/error_check.cpp)

add_library (yavin SHARED ${src})

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

find_package(glfw3 REQUIRED)
find_package(gl3w REQUIRED)
find_package(glm REQUIRED)
find_package(Threads REQUIRED)

target_link_libraries(yavin glfw gl3w glm png ${CMAKE_THREAD_LIBS_INIT})

install (TARGETS yavin DESTINATION bin)
install (FILES yavin ${src} DESTINATION include)

add_subdirectory(test)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -pedantic")

# foreach (SOURCE_FILE ${all_src})
#   message(STATUS ${SOURCE_FILE})
# endforeach ()

# cppcheck
add_custom_target(
        cppcheck
        COMMAND /usr/bin/cppcheck 
        -I../Yavin
        --enable=warning,performance,portability,information,missingInclude
        --std=c++11
        --template="[{severity}][{id}] {message} {callstack} \(On {file}:{line}\)"
        --verbose
        --quiet
        --xml-version=2
        ${src}
        2> cppcheck.xml
)