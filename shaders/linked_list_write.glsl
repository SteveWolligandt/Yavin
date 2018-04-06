#include "linked_list_element.glsl"
#include "linked_list_layouts.glsl"

layout (binding = 5, offset = 0) uniform atomic_uint cnt;

const bool depth_duplication_test = false;

vec4 shading();
void main() {
  vec4 color = shading();
  if (color.a > 0) {
    bool duplicate = false;
    if (depth_duplication_test) {
      uint running_index = imageLoad(head_index, ivec2(gl_FragCoord.xy)).r;
      for (uint i = 0; i < 20; ++i) {
      // while (running_index != 0xffffffff) {
        if (running_index == 0xffffffff) break;
        if (abs(nodes[running_index].depth - gl_FragCoord.z) < 1e-7) {
        //if (nodes[running_index].depth == 1e-3) {
          duplicate = true;
          break;
        }
        running_index = nodes[running_index].next_index;
      }
    }
  
    if (!duplicate) {
      uint i = atomicCounterIncrement(cnt);
      if (i < size) {
        uint next_index 
          = imageAtomicExchange(head_index, ivec2(gl_FragCoord.xy), i);
        nodes[i].color = color;
        nodes[i].depth = gl_FragCoord.z;
        nodes[i].next_index = next_index;
      }
    }
  }
}
