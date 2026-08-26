/*
World clock utility.

Copyright 2026. Andrew Wang
*/
#include <unistd.h>

#include <chrono>
#include <format>
#include <iostream>
#include <thread>

#include "zones.h"

using std::cin;
using std::cout;
using std::format;
using std::ios_base;
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
      cout << format("{:<32}{:%a %b-%d %r %z}\n", name, zone_time);
    }
    cout.flush();

    sleep_until(system_time += tick);
    manager.clear_output(cout);
  }
}
