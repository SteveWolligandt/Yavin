#include "linked_list_element.glsl"
#include "linked_list_layouts.glsl"

layout (binding = 5, offset = 0) uniform atomic_uint cnt;

vec4 shading();
void main() {
  vec4 color = shading();
  if (color.a > 0) {
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
