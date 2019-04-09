Yavin
===

## Usage
#### Vertex Buffer Objects
Vertex buffer objects can be created using the template class vertexbuffer. Specify the vertex attributes like this:

``` cpp
#include<yavin>
using vbo_t = yavin::vertexbuffer<yavin::vec2, yavin::vec3>;
// ...
// create a vertex buffer object with a float vec2 and a float vec3
vbo_t vbo;
vbo.push_back({-1.0f, -0.5f}, {1.0f, 0.0f, 0.0f});
vbo.push_back({ 1.0f, -1.0f}, {0.0f, 0.0f, 1.0f});
vbo.push_back({-1.0f,  1.0f}, {0.0f, 0.0f, 1.0f});
vbo.push_back({ 1.0f,  0.5f}, {1.0f, 0.0f, 0.0f});
// ...
```

Create an index buffer object and a vertex array object to draw the vertex data
``` cpp
// ...
// create index buffer object and upload immediately
yavin::indexbuffer ibo{0, 1, 2, 1, 3, 2};
// create vertex array object
yavin::vertexarray vao;

// associate vbo and ibo to vao and activate vertex attributes
vao.bind();
vbo.bind();
ibo.bind();
vbo_t::activate_attributes();
// ...
``` 
To draw the vertex data do the following
``` cpp
// ...
vao.bind();
vao.draw_triangles(ibo.size());
// ...
``` 

#### Shaders

Yavin automatically searches for uniform variables and vertex attributes in the shader source. Additionally you can use #include in shader source.

``` cpp
#include<yavin>
// ...
// create shader
Yavin::shader s;
s.add_stage<yavin::vertexshader>("<path to vertex shader>");
s.add_stage<yavin::fragmentshader>("<path to fragment shader>");
s.create();
//...
// bind shader
s.bind();
// draw Vertex Array Object
vao.bind();
vao.draw_triangles(ibo.gpu_size());
// ...
``` 

## Compiling
Use CMake to compile the library and tests. You will need [glm](https://github.com/g-truc/glm), [glew](http://glew.sourceforge.net/) and [glfw3](https://github.com/glfw/glfw)
