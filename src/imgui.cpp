#include <yavin/imgui.h>
//==============================================================================
namespace ImGui {
//==============================================================================
bool InputDouble2(const char* label, double v[2], const char* format,
                  ImGuiInputTextFlags flags) {
  return InputScalarN(label, ImGuiDataType_Double, v, 2, NULL, NULL, format,
                      flags);
}
//------------------------------------------------------------------------------
bool InputDouble3(const char* label, double v[3], const char* format,
                  ImGuiInputTextFlags flags) {
  return InputScalarN(label, ImGuiDataType_Double, v, 3, NULL, NULL, format,
                      flags);
}
//------------------------------------------------------------------------------
bool InputDouble4(const char* label, double v[4], const char* format,
                  ImGuiInputTextFlags flags) {
  return InputScalarN(label, ImGuiDataType_Double, v, 4, NULL, NULL, format,
                      flags);
}
//------------------------------------------------------------------------------
bool DragDouble(const char* label, double* v, double v_speed, double v_min,
                double v_max, const char* format, double power) {
  return DragScalar(label, ImGuiDataType_Double, v, v_speed, &v_min, &v_max,
                    format, power);
}
//------------------------------------------------------------------------------
bool DragDouble2(const char* label, double v[2], double v_speed, double v_min,
                 double v_max, const char* format, double power) {
  return DragScalarN(label, ImGuiDataType_Double, v, 2, v_speed, &v_min, &v_max,
                     format, power);
}
//------------------------------------------------------------------------------
bool DragDouble3(const char* label, double v[3], double v_speed, double v_min,
                 double v_max, const char* format, double power) {
  return DragScalarN(label, ImGuiDataType_Double, v, 3, v_speed, &v_min, &v_max,
                     format, power);
}
//------------------------------------------------------------------------------
bool DragDouble4(const char* label, double v[4], double v_speed, double v_min,
                 double v_max, const char* format, double power) {
  return DragScalarN(label, ImGuiDataType_Double, v, 4, v_speed, &v_min, &v_max,
                     format, power);
}
//==============================================================================
}  // namespace ImGui
//==============================================================================
