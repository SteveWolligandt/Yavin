#ifndef LINKED_LIST_LAYOUTS_GLSL
#define LINKED_LIST_LAYOUTS_GLSL

layout (binding = 7, r32ui) uniform uimage2D head_index;
layout (binding = 8, std430) buffer linked_list_size_buffer { 
  uint size;
};
layout (binding = 9, std430) buffer linked_list { 
  linked_list_element nodes[];
};

#endif
