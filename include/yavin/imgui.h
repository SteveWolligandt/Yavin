#ifndef YAVIN_IMGUI_H
#define YAVIN_IMGUI_H
//==============================================================================
#include "imgui_api_backend.h"
#include "imgui_render_backend.h"
//==============================================================================
#include <imgui/imgui_internal.h>
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
//==============================================================================
}  // namespace ImGui
//==============================================================================
#endif
