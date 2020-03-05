#include <yavin/window.h>
#include <iostream>
//==============================================================================
struct listener : yavin::window_listener {
  bool& run;
  //----------------------------------------------------------------------------
  listener(bool& _run) : run{_run} {}
  //----------------------------------------------------------------------------
  void on_key_pressed(yavin::key k) override {
    std::cerr << "key pressed " << to_string(k) << "\n";
  }
  //----------------------------------------------------------------------------
  void on_key_released(yavin::key k) override {
    std::cerr << "key released " << to_string(k) << "\n";
    if (k == yavin::KEY_ESCAPE) { run = false; }
  }
  //----------------------------------------------------------------------------
  void on_button_pressed(yavin::button b) override {
    std::cerr << "button pressed " << to_string(b) << "\n";
  }
  //----------------------------------------------------------------------------
  void on_button_released(yavin::button b) override {
    std::cerr << "button released " << to_string(b) << "\n";
  }
  //----------------------------------------------------------------------------
  void on_mouse_motion(int x, int y) override {
    std::cerr << "mouse moved " << x << ", " << y << "\n";
  }
};
//==============================================================================
int main() {
  yavin::window w{"glx window test", 600, 600};
  bool          run = true;
  listener      l{run};
  w.add_listener(l);
  while (run) { w.check_events(); }
}
