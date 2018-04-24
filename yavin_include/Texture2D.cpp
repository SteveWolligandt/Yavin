#include "Texture2D.h"

//==============================================================================
namespace Yavin {
//==============================================================================

template class Texture2D<int8_t, R>;
template class Texture2D<int8_t, RG>;
template class Texture2D<int8_t, RGB>;
template class Texture2D<int8_t, RGBA>;
template class Texture2D<int8_t, BGR>;
template class Texture2D<int8_t, BGRA>;

template class Texture2D<uint8_t, R>;
template class Texture2D<uint8_t, RG>;
template class Texture2D<uint8_t, RGB>;
template class Texture2D<uint8_t, RGBA>;
template class Texture2D<uint8_t, BGR>;
template class Texture2D<uint8_t, BGRA>;

template class Texture2D<int16_t, R>;
template class Texture2D<int16_t, RG>;
template class Texture2D<int16_t, RGB>;
template class Texture2D<int16_t, RGBA>;
template class Texture2D<int16_t, BGR>;
template class Texture2D<int16_t, BGRA>;

template class Texture2D<uint16_t, R>;
template class Texture2D<uint16_t, RG>;
template class Texture2D<uint16_t, RGB>;
template class Texture2D<uint16_t, RGBA>;
template class Texture2D<uint16_t, BGR>;
template class Texture2D<uint16_t, BGRA>;

template class Texture2D<int32_t, R>;
template class Texture2D<int32_t, RG>;
template class Texture2D<int32_t, RGB>;
template class Texture2D<int32_t, RGBA>;
template class Texture2D<int32_t, BGR>;
template class Texture2D<int32_t, BGRA>;

template class Texture2D<uint32_t, R>;
template class Texture2D<uint32_t, RG>;
template class Texture2D<uint32_t, RGB>;
template class Texture2D<uint32_t, RGBA>;
template class Texture2D<uint32_t, BGR>;
template class Texture2D<uint32_t, BGRA>;

template class Texture2D<float, R>;
template class Texture2D<float, RG>;
template class Texture2D<float, RGB>;
template class Texture2D<float, RGBA>;
template class Texture2D<float, BGR>;
template class Texture2D<float, BGRA>;

template class Texture2D<int8_t, Depth8>;
template class Texture2D<int16_t, Depth16>;
template class Texture2D<float, Depth24>;
template class Texture2D<float, Depth32>;

//==============================================================================
}  // namespace Yavin
//==============================================================================
