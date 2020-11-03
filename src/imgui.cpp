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
//------------------------------------------------------------------------------
bool BufferingBar(const char* label, float value, const ImVec2& size_arg,
                  const ImU32& bg_col, const ImU32& fg_col) {
  ImGuiWindow* window = GetCurrentWindow();
  if (window->SkipItems) return false;

  ImGuiContext&     g     = *GImGui;
  const ImGuiStyle& style = g.Style;
  const ImGuiID     id    = window->GetID(label);

  ImVec2 pos  = window->DC.CursorPos;
  ImVec2 size = size_arg;
  size.x -= style.FramePadding.x * 2;

  const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
  ItemSize(bb, style.FramePadding.y);
  if (!ItemAdd(bb, id)) return false;

  // Render
  const float circleStart = size.x * 0.7f;
  const float circleEnd   = size.x;
  const float circleWidth = circleEnd - circleStart;

  window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart, bb.Max.y),
                                  bg_col);
  window->DrawList->AddRectFilled(
      bb.Min, ImVec2(pos.x + circleStart * value, bb.Max.y), fg_col);

  const float t     = g.Time;
  const float r     = size.y / 2;
  const float speed = 1.5f;

  const float a = speed * 0;
  const float b = speed * 0.333f;
  const float c = speed * 0.666f;

  const float o1 =
      (circleWidth + r) * (t + a - speed * (int)((t + a) / speed)) / speed;
  const float o2 =
      (circleWidth + r) * (t + b - speed * (int)((t + b) / speed)) / speed;
  const float o3 =
      (circleWidth + r) * (t + c - speed * (int)((t + c) / speed)) / speed;

  window->DrawList->AddCircleFilled(
      ImVec2(pos.x + circleEnd - o1, bb.Min.y + r), r, bg_col);
  window->DrawList->AddCircleFilled(
      ImVec2(pos.x + circleEnd - o2, bb.Min.y + r), r, bg_col);
  window->DrawList->AddCircleFilled(
      ImVec2(pos.x + circleEnd - o3, bb.Min.y + r), r, bg_col);
  return true;
}
//------------------------------------------------------------------------------
bool Spinner(const char* label, float radius, int thickness,
             const ImU32& color) {
  ImGuiWindow* window = GetCurrentWindow();
  if (window->SkipItems) return false;

  ImGuiContext&     g     = *GImGui;
  const ImGuiStyle& style = g.Style;
  const ImGuiID     id    = window->GetID(label);

  ImVec2 pos = window->DC.CursorPos;
  ImVec2 size((radius)*2, (radius + style.FramePadding.y) * 2);

  const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
  ItemSize(bb, style.FramePadding.y);
  if (!ItemAdd(bb, id)) return false;

  // Render
  window->DrawList->PathClear();

  int num_segments = 30;
  int start        = abs(ImSin(g.Time * 1.8f) * (num_segments - 5));

  const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
  const float a_max =
      IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

  const ImVec2 centre =
      ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

  for (int i = 0; i < num_segments; i++) {
    const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
    window->DrawList->PathLineTo(
        ImVec2(centre.x + ImCos(a + g.Time * 8) * radius,
               centre.y + ImSin(a + g.Time * 8) * radius));
  }

  window->DrawList->PathStroke(color, false, thickness);
  return true;
}
inline static void StdStringNonStdResize(std::string& s, int size) {
  IM_ASSERT(size >= 0);
  const int oldLength = s.length();
  if (size < oldLength)
    s = s.substr(0, size);
  else if (size > oldLength)
    for (int i = 0, icnt = size - oldLength; i < icnt; i++)
      s += '\0';
}
//==============================================================================
}  // namespace ImGui
//==============================================================================
