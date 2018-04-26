#ifndef __YAVIN_TO_STRING_H__
#define __YAVIN_TO_STRING_H__

#include <string>
#include "gl_includes.h"

//==============================================================================
namespace Yavin {
//==============================================================================

std::string to_string(GLenum e) {
  switch (e) {
    // buffers
    case GL_ARRAY_BUFFER: return "GL_ARRAY_BUFFER";
    case GL_ATOMIC_COUNTER_BUFFER: return "GL_ATOMIC_COUNTER_BUFFER";
    case GL_COPY_READ_BUFFER: return "GL_COPY_READ_BUFFER";
    case GL_COPY_WRITE_BUFFER: return "GL_COPY_WRITE_BUFFER";
    case GL_DISPATCH_INDIRECT_BUFFER: return "GL_DISPATCH_INDIRECT_BUFFER";
    case GL_DRAW_INDIRECT_BUFFER: return "GL_DRAW_INDIRECT_BUFFER";
    case GL_ELEMENT_ARRAY_BUFFER: return "GL_ELEMENT_ARRAY_BUFFER";
    case GL_PIXEL_PACK_BUFFER: return "GL_PIXEL_PACK_BUFFER";
    case GL_PIXEL_UNPACK_BUFFER: return "GL_PIXEL_UNPACK_BUFFER";
    case GL_QUERY_BUFFER: return "GL_QUERY_BUFFER";
    case GL_SHADER_STORAGE_BUFFER: return "GL_SHADER_STORAGE_BUFFER";
    case GL_TEXTURE_BUFFER: return "GL_TEXTURE_BUFFER";
    case GL_TRANSFORM_FEEDBACK_BUFFER: return "GL_TRANSFORM_FEEDBACK_BUFFER";
    case GL_UNIFORM_BUFFER: return "GL_UNIFORM_BUFFER";

    // accesses
    case GL_READ_ONLY: return "GL_READ_ONLY";
    case GL_WRITE_ONLY: return "GL_WRITE_ONLY";
    case GL_READ_WRITE: return "GL_READ_WRITE";

    // usages
    case GL_STREAM_DRAW: return "GL_STREAM_DRAW";
    case GL_STREAM_READ: return "GL_STREAM_READ";
    case GL_STREAM_COPY: return "GL_STREAM_COPY";
    case GL_STATIC_DRAW: return "GL_STATIC_DRAW";
    case GL_STATIC_READ: return "GL_STATIC_READ";
    case GL_STATIC_COPY: return "GL_STATIC_COPY";
    case GL_DYNAMIC_DRAW: return "GL_DYNAMIC_DRAW";
    case GL_DYNAMIC_READ: return "GL_DYNAMIC_READ";
    case GL_DYNAMIC_COPY: return "GL_DYNAMIC_COPY";

    default: return "GLenum(" + std::to_string(e) + ")";
  }
}

//------------------------------------------------------------------------------

std::string map_access_to_string(GLbitfield b) {
  std::string flags;
  auto        pipe = [&flags]() {
    if (!flags.empty()) flags += std::string(" | ");
  };
  if (GL_MAP_READ_BIT & b) {
    pipe();
    flags += "GL_MAP_READ_BIT";
  }
  if (GL_MAP_WRITE_BIT & b) {
    pipe();
    flags += "GL_MAP_WRITE_BIT";
  }
  if (GL_MAP_PERSISTENT_BIT & b) {
    pipe();
    flags += "GL_MAP_PERSISTENT_BIT";
  }
  if (GL_MAP_COHERENT_BIT & b) {
    pipe();
    flags += "GL_MAP_COHERENT_BIT";
  }  // namespace Yavin
  if (GL_MAP_INVALIDATE_RANGE_BIT & b) {
    pipe();
    flags += "GL_MAP_INVALIDATE_RANGE_BIT";
  }
  if (GL_MAP_INVALIDATE_BUFFER_BIT & b) {
    pipe();
    flags += "GL_MAP_INVALIDATE_BUFFER_BIT";
  }
  if (GL_MAP_FLUSH_EXPLICIT_BIT & b) {
    pipe();
    flags += "GL_MAP_FLUSH_EXPLICIT_BIT";
  }
  if (GL_MAP_UNSYNCHRONIZED_BIT & b) {
    pipe();
    flags += "GL_MAP_UNSYNCHRONIZED_BIT";
  }
  return flags;
}

//==============================================================================
}  // namespace Yavin
//==============================================================================

#endif
