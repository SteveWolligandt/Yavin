#include <yavin/window.h>
#include <iostream>
//==============================================================================
int main() {
  yavin::window w{"glx window test", 1000, 1000};
  struct keyboard_listener : yavin::keyboard_listener {
    void on_key_pressed(yavin::key k) override { std::cerr << "key pressed\n"; }
    void on_key_released(yavin::key k) override {
      std::cerr << "key released\n";
    }
  } l;
  w.add_listener(l);
  while(true) {
    w.check_events();
  }
}
