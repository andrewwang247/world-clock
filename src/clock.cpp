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
using std::ios_base;
using std::println;
using std::this_thread::sleep_until;

namespace chrono = std::chrono;

int main() {
  ios_base::sync_with_stdio(false);

  auto system_time =
      chrono::time_point_cast<chrono::seconds>(chrono::system_clock::now());
  constexpr chrono::seconds tick{1};

  const auto manager = isatty(STDIN_FILENO) ? zones{} : zones{cin, system_time};

  while (true) {
    for (const auto zone_time : manager.localize(system_time)) {
      const auto name = zone_time.get_time_zone()->name();
      println("{:<32}{:%a %b-%d %r %z}", name, zone_time);
    }
    fflush(stdout);

    sleep_until(system_time += tick);
    manager.clear_stdout();
  }
}
