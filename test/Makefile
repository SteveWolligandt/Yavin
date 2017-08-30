GL3W=/home/steve/libraries/gl3w/build
GLM=/home/steve/libraries/glm
GLFW=/home/steve/libraries/glfw

GLMINCLUDE=-I$(GLM)
GL3WINCLUDE=-I$(GL3W)/include
GLFWINCLUDE=-I$(GLFW)/include

CXX=g++
BIN=window_test
LIB=-L../debug -ldl -lyavin
#LIB=-lglfw -lyavin -ldl
INC=-I../../ $(GLMINCLUDE) $(GL3WINCLUDE) $(GLFWINCLUDE)

all: window_test regex_glsl test

window_test: yavin window_test.cpp 
	$(CXX) -std=c++17 -g -Wl,-rpath,../debug $(INC) -o window_test window_test.cpp $(LIB)

regex_glsl: yavin regex_glsl.cpp
	$(CXX) -std=c++17 -g -Wl,-rpath,../debug $(INC) -o regex_glsl regex_glsl.cpp $(LIB)

test: yavin test.cpp
	$(CXX) -std=c++17 -g -Wl,-rpath,../debug $(INC) -o test test.cpp $(LIB)

yavin:
	make -C .. debug

window_test.run: window_test
	./window_test

regex_glsl.run: regex_glsl
	./regex_glsl

test.run: test
	./test

clean:
	rm -f test regex_glsl window_test
	make -C .. clean