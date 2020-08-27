![alt text](https://raw.githubusercontent.com/SteveWolligandt/yavin/master/yavin.gif)

Yavin is an OpenGL Wrapper for C++-20.  It uses EGL for creating contexts and
windows and GLEW for GL extensions handling. Additionally it has implemented
Dear ImGUI.

## Usage
#### Context creation
Yavin creates windows via X11 and EGL. A shared context can be created the
do_async method of yavin::window. Internally a shared context of the window
will be created and made current in the new thread. However OpenGL is not
thread-safe so you have to synchronize your rendering and uploading otherwise
you will get segmentation faults.

``` cpp
#include<yavin>
void create_window() {
  yavin::window win{"Title", window_width, window_height};
  // do your rendering jobs
  win.do_async([&](){
    // do your async uploading of buffers or textures
  });
}
```

You can also create a offscreen rendering context without creating a window:
``` cpp
#include<yavin>
void offscreen_rendering() {
  yavin::context ctx;
  // do offscreen rendering
}
```

#### Vertex Buffer Objects
Vertex buffer objects can be created using the template class vertexbuffer.
Specify the vertex attributes like this:

``` cpp
#include<yavin>
using vec2 = std::array<GLfloat, 2>;
using vec3 = std::array<GLfloat, 3>;
using vbo_t = yavin::vertexbuffer<vec2, vec3>;
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
s.add_stage<yavin::vertexshader>("path/to/vertex/shader");
s.add_stage<yavin::fragmentshader>("path/to/fragment/shader");
s.create();
//...
// bind shader
s.bind();
// draw Vertex Array Object
vao.bind();
vao.draw_triangles(ibo.gpu_size());
// ...
``` 
#### Textures
TODO write

## Compiling
Use CMake to compile the library and tests.
