#version 430

struct linked_list_element {
  vec4  color;
  uint  next_index;
  float depth;
  uint  idx;
};

layout (binding = 0, r32ui) uniform uimage2D head_index;
layout (binding = 0, offset = 0) uniform atomic_uint cnt;
layout (std430, binding = 0) buffer linked_list { 
  linked_list_element nodes[];
};

uniform uint linked_list_size;

vec4 shade();
void main() {
  uint i = atomicCounterIncrement(cnt);
  if (i < linked_list_size) {
    uint next_index = imageAtomicExchange(head_index, ivec2(gl_FragCoord.xy), i);
    nodes[i].color = shade();
    nodes[i].depth = gl_FragCoord.z;
    nodes[i].next_index = next_index;
    nodes[i].idx = i;
  }
}

uniform uint screen_width;
uniform uint screen_height;
uniform vec4 color;

vec4 shade() {
  return color;
}
