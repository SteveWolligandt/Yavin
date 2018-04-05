#ifndef LINKED_LIST_ELEMENT_GLSL
#define LINKED_LIST_ELEMENT_GLSL

struct linked_list_element {
  vec4  color;
  uint  next_index;
  float depth;
  vec2  pad;
};
#endif
