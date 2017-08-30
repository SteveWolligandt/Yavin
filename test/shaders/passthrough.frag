#version 450 core

uniform float brightness;
uniform sampler2D tex1;
uniform sampler2D tex2;

in vec3 nor_frag;
in vec2 tex_frag;

layout (location=0) out vec4 out_color;

void main() {
  float light_strength = nor_frag.z;

  vec3 tex1_color=texture(tex1, tex_frag).rgb;
  vec3 tex2_color=texture(tex2, tex_frag).rgb;
  out_color = vec4(((tex1_color*(1-brightness) + tex2_color*brightness)), 1);
  out_color = vec4(nor_frag*0.5+0.5, 1);
  out_color = vec4(((tex1_color*(1-brightness) + tex2_color*brightness))*light_strength, 1);
  // out_color = vec4(tex_frag.x,tex_frag.y,0, 1);
}