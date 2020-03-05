#include <yavin/window.h>
#include <iostream>
//==============================================================================
struct keyboard_listener : yavin::keyboard_listener {
  bool& run;
  keyboard_listener(bool& _run) : run{_run} {}
  void on_key_pressed(yavin::key k) override {
    std::cerr << "key pressed " << to_string(k) << "\n";
  }
  void on_key_released(yavin::key k) override {
    std::cerr << "key released " << to_string(k) << "\n";
    if (k == yavin::ESCAPE) { run = false; }
  }
};
//==============================================================================
int main() {
  yavin::window     w{"glx window test", 600, 600};
  bool              run = true;
  keyboard_listener l{run};
  w.add_listener(l);
  while (run) { w.check_events(); }
}
