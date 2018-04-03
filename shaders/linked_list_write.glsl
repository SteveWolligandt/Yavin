#version 450

struct linked_list_element {
  vec4  color;
  uint  next_index;
  float depth;
  uint  idx;
};

layout (binding = 7, r32ui) uniform uimage2D head_index;
layout (binding = 5, offset = 0) uniform atomic_uint cnt;
layout (binding = 8, std430) buffer linked_list { 
  linked_list_element nodes[];
};

vec4 shading();
void main() {
  uint i = atomicCounterIncrement(cnt);
  if (i < nodes.length()) {
    uint running_index = imageLoad(head_index, ivec2(gl_FragCoord.xy)).r;

    bool duplicate = false;
    while (running_index != 0xffffffff) {
      if (nodes[running_index].depth == gl_FragCoord.z) {
        duplicate = true;
        break;
      }
      running_index = nodes[running_index].next_index;
    }

    if (!duplicate) {
      uint next_index = imageAtomicExchange(head_index, ivec2(gl_FragCoord.xy), i);
      nodes[i].color = shading();
      nodes[i].depth = gl_FragCoord.z;
      nodes[i].next_index = next_index;
      nodes[i].idx = i;
    }
  }
}

