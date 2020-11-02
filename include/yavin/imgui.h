#ifndef YAVIN_IMGUI_H
#define YAVIN_IMGUI_H
//==============================================================================
#include <yavin/imgui_includes.h>
//==============================================================================
namespace ImGui {
//==============================================================================
bool InputDouble2(const char* label, double v[2], const char* format = "%.3lf",
                  ImGuiInputTextFlags flags = 0);
bool InputDouble3(const char* label, double v[3], const char* format = "%.3lf",
                  ImGuiInputTextFlags flags = 0);
bool InputDouble4(const char* label, double v[4], const char* format = "%.3lf",
                  ImGuiInputTextFlags flags = 0);

bool DragDouble(const char* label, double* v, double v_speed = 1.0,
                double v_min = 0.0, double v_max = 0.0,
                const char* format = "%.3lf", double power = 1.0);
bool DragDouble2(const char* label, double v[2], double v_speed = 1.0,
                 double v_min = 0.0, double v_max = 0.0,
                 const char* format = "%.3lf", double power = 1.0);
bool DragDouble3(const char* label, double v[3], double v_speed = 1.0,
                 double v_min = 0.0, double v_max = 0.0,
                 const char* format = "%.3lf", double power = 1.0);
bool DragDouble4(const char* label, double v[4], double v_speed = 1.0,
                 double v_min = 0.0, double v_max = 0.0,
                 const char* format = "%.3lf", double power = 1.0);
bool BufferingBar(const char* label, float value, const ImVec2& size_arg,
                  const ImU32& bg_col, const ImU32& fg_col);
bool Spinner(const char* label, float radius, int thickness,
             const ImU32& color);
//==============================================================================
}  // namespace ImGui
//==============================================================================
#include <yavin/imgui_api_backend.h>
#include <yavin/imgui_render_backend.h>
//==============================================================================
#endif
