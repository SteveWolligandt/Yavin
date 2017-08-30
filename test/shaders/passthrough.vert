#version 450 core

uniform mat4 modelview;
uniform mat4 projection;

layout (location=0) in vec3 pos;
layout (location=1) in vec3 nor;
layout (location=2) in vec2 tex;

out vec3 nor_frag;
out vec2 tex_frag;

void main() {
  gl_Position = projection*modelview*vec4(pos,1);
  nor_frag = (modelview*vec4(nor,0)).xyz;
  tex_frag = tex;
}