#version 430
#define MAX_FRAGMENTS 20

struct linked_list_element {
  vec4  color;
  uint  next_index;
  float depth;
};

layout (binding = 7, r32ui) uniform uimage2D head_index;
layout (std430, binding = 8) buffer linked_list { 
  linked_list_element nodes[];
};

uniform vec4 clear_color;

out vec4 frag_color;

void main() {
  uint running_index = imageLoad(head_index, ivec2(gl_FragCoord.xy)).r;

  // Check, if a fragment was written.
  if (running_index == 0xffffffff)
      frag_color = clear_color;

  else {
    uint frag_indices[MAX_FRAGMENTS];
    uint num_frags = 0;
    uint tmp_idx;
  
    // Copy the fragment indices of this pixel.
    while (running_index != 0xffffffff && num_frags < MAX_FRAGMENTS) {
      frag_indices[num_frags++] = running_index;
      running_index = nodes[running_index].next_index;
    }

    // Sort the indices: Highest to lowest depth.
    for(uint i = 0; i < num_frags; i++)
      for(uint j = i + 1; j < num_frags; j++)
        if (nodes[frag_indices[j]].depth > nodes[frag_indices[i]].depth) {
          tmp_idx = frag_indices[i];
          frag_indices[i] = frag_indices[j];
          frag_indices[j] = tmp_idx;
        }
        
    vec4 dst_col = clear_color;

    // Back to front blending.
    for (uint i = 0; i < num_frags; i++)
      // Blend using "default" blend equation.
      dst_col = vec4(mix(dst_col.rgb, nodes[frag_indices[i]].color.rgb, nodes[frag_indices[i]].color.a), 1.0);
        
    frag_color = dst_col;
  }
}
