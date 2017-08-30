Yavin
===

Yavin is an OpenGL 4.5 Wrapper using modern C++17 features.

## Usage
#### Window
```cpp
#include <yavin>
Yavin::Window w("Test Window", 800, 600, 4, 5);

w.set_render_function([&]() {
// do render stuff
});

w.set_key_callback_function([&](int key, int scancode, int action, int mods) {
// do key management
});

w.set_update_function([&](double dt) {
// do update stuff
});

// set fps and start rendering
w.set_fps(60.0);
w.start_rendering();
```

#### Vertex Buffer Objects
Vertex buffer objects (VBO) can be created using the template class VertexBuffer. Specify the vertex attributes like this:

``` cpp
#include<yavin>
using vbo_t = Yavin::VertexBuffer<Yavin::vec2, Yavin::vec3>;
// ...
// create a vertex buffer object with a float vec2 and a float vec3
vbo_t vbo;
vbo.push_back({-1.0f, -0.5f}, {1.0f, 0.0f, 0.0f});
vbo.push_back({ 1.0f, -1.0f}, {0.0f, 0.0f, 1.0f});
vbo.push_back({-1.0f,  1.0f}, {0.0f, 0.0f, 1.0f});
vbo.push_back({ 1.0f,  0.5f}, {1.0f, 0.0f, 0.0f});
vbo.upload_data();
// ...
```

Create an index buffer object and a vertex array object to draw the vertex data
``` cpp
// ...
// create index buffer object and upload immediately
Yavin::IndexBuffer ibo({0, 1, 2, 1, 3, 2}, true);
// create vertex array object
Yavin::VertexArray vao;

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
vao.draw(Yavin::TRIANGLES, ibo.gpu_size());
// ...
``` 

#### Shaders

Yavin automatically searches for uniform variables and vertex attributes in the shader source. Additionally you can use #include in shader source.

``` cpp
#include<yavin>
// ...
// create shader
Yavin::Shader shader;
shader.add_shader_stage<Yavin::VertexShader>("<path to vertex shader>");
shader.add_shader_stage<Yavin::FragmentShader>("<path to fragment shader>");
shader.create();
//...
// bind shader
shader.bind();
// draw Vertex Array Object
vao.bind();
vao.draw(Yavin::TRIANGLES, ibo.gpu_size());
// ...
``` 

## Compiling
Use CMake to compile the library and some tests. You will need [glm](https://github.com/g-truc/glm), [gl3w](https://github.com/skaslev/gl3w) and [glfw3](https://github.com/glfw/glfw)

Tested under Arch Linux with gcc 7.1