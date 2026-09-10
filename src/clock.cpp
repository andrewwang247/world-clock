/*
World clock utility.

Copyright 2026. Andrew Wang
*/
#include <unistd.h>

#include <chrono>
#include <cstdio>
#include <iostream>
#include <print>
#include <thread>

#include "zones.h"

using std::cin;
using std::fflush;
using std::println;
using std::this_thread::sleep_until;

namespace chrono = std::chrono;

int main() {
  cin.tie(nullptr);
  constexpr tick_t tick{1};

  auto sys_time = chrono::time_point_cast<tick_t>(chrono::system_clock::now());
  const auto manager = isatty(STDIN_FILENO) ? zones{} : zones{cin, sys_time};

  while (true) {
    for (const auto zone_time : manager.localize(sys_time)) {
      const auto name = zone_time.get_time_zone()->name();
      println("{:<32}{:%a %b-%d %r %Ez}", name, zone_time);
    }
    fflush(stdout);

    sleep_until(sys_time += tick);
    manager.clear_stdout();
  }
}
