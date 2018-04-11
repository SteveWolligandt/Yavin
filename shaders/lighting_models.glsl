#ifndef YAVIN_LIGHTING_MODELS_H
#define YAVIN_LIGHTING_MODELS_H

//==============================================================================
// PHONG
//==============================================================================

float phong_diffuse_brightness(
    vec3 normalized_light_dir, 
    vec3 normalized_normal) {
  return max(abs(dot(normalized_light_dir, normalized_normal)), 0);
}

//------------------------------------------------------------------------------

float phong_specular_brightness(
    vec3 normalized_eye_pos, 
    vec3 normalized_reflection_dir,
    float shininess) {
  return pow(max(dot(normalized_eye_pos, normalized_reflection_dir), 0), shininess);
}

//------------------------------------------------------------------------------
/**
 * Phong Lighting with all parameters specified
 */
vec3 phong_lighting(
    vec3 vertex_pos,
    vec3 normal,
    vec3 light_pos, 
    vec3 material_color,
    vec3 light_color,
    float ambient_part,
    float diffuse_part,
    float specular_part,
    float shininess) {
  vec3 L = normalize(light_pos - vertex_pos); // light direction
  vec3 E = normalize(-vertex_pos);            // eye position
  vec3 N = normalize(normal);                 // normal
  vec3 R = normalize(-reflect(L,N));          // reflection vector

  return material_color * ambient_part + 
         material_color * diffuse_part * phong_diffuse_brightness(L, N) +
         light_color * specular_part * phong_specular_brightness(E, R, shininess);

}

//------------------------------------------------------------------------------
/**
 * Phong Headlight with all parameters 
 */
vec3 phong_lighting(
    vec3 vertex_pos,
    vec3 normal,
    vec3 material_color,
    vec3 light_color,
    float ambient_part,
    float diffuse_part,
    float specular_part,
    float shininess) {
  vec3 L = normalize(-vertex_pos);   // light direction
  vec3 N = normalize(normal);        // normal
  vec3 R = normalize(-reflect(L,N)); // reflection vector

  return material_color * ambient_part + 
         material_color * diffuse_part * phong_diffuse_brightness(L, N) +
         light_color * specular_part * phong_specular_brightness(L, R, shininess);

}

//------------------------------------------------------------------------------
/**
 * Phong Headlight with white lightsource
 */
vec3 phong_lighting(
    vec3 vertex_pos,
    vec3 normal,
    vec3 material_color,
    float ambient_part,
    float diffuse_part,
    float specular_part,
    float shininess) {
  vec3 L = normalize(-vertex_pos);   // light direction
  vec3 N = normalize(normal);        // normal
  vec3 R = normalize(-reflect(L,N)); // reflection vector

  return material_color * ambient_part + 
         material_color * diffuse_part * phong_diffuse_brightness(L, N) +
         specular_part * vec3(phong_specular_brightness(L, R, shininess));

}

#endif
