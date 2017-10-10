#
# Try to find GLFW library and include path.
# Once done this will define
#
# GLFW3_FOUND
# GLFW3_INCLUDE_DIRECTORIES
# GLFW3_LIBRARIES
# 

IF(WIN32)
    FIND_PATH( GLFW3_INCLUDE_DIRECTORIES GLFW/glfw3.h
		$ENV{PROGRAMFILES}/GLFW/include
		${GLFW_ROOT_DIR}/include
		DOC "The directory where GLFW/glfw3.h resides")

    FIND_LIBRARY( GLFW3_LIBRARIES
        NAMES glfw3 GLFW
        PATHS
        $ENV{PROGRAMFILES}/GLFW/lib
		${GLFW_ROOT_DIR}/lib
        DOC "The GLFW library")
ELSE(WIN32)
	FIND_PATH( GLFW3_INCLUDE_DIRECTORIES GLFW/glfw3.h
		/usr/include
		/usr/local/include
		/sw/include
        /opt/local/include
		${GLFW_ROOT_DIR}/include
		DOC "The directory where GLFW/glfw3.h resides")

	# Prefer the static library.
	FIND_LIBRARY( GLFW3_LIBRARIES
        NAMES libGLFW.a GLFW libGLFW3.a GLFW3 libglfw.so libglfw.so.3
		PATHS
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
		${GLFW_ROOT_DIR}/lib
		DOC "The GLFW library")
ENDIF(WIN32)

SET(GLFW3_FOUND "NO")
IF(GLFW3_INCLUDE_DIRECTORIES AND GLFW3_LIBRARIES)
	SET(GLFW_LIBRARIES ${GLFW3_LIBRARIES})
	SET(GLFW3_FOUND "YES")
    message(STATUS "Found GLFW")
ENDIF(GLFW3_INCLUDE_DIRECTORIES AND GLFW3_LIBRARIES)
