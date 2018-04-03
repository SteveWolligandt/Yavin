#version 430

uniform mat4 modelview;
uniform mat4 projection;

layout (location = 0) in vec2 pos;

void main() {
  gl_Position = projection * modelview * vec4(pos, 0, 1);
}
