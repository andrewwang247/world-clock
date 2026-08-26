/*
World clock utility.

Copyright 2026. Andrew Wang
*/
#include <chrono>
#include <format>
#include <iostream>
#include <thread>

#include "zone.h"

using std::cin;
using std::cout;
using std::flush;
using std::format;
using std::ios_base;
using std::this_thread::sleep_until;

namespace chrono = std::chrono;

int main() {
  ios_base::sync_with_stdio(false);
  auto system_time =
      chrono::time_point_cast<chrono::seconds>(chrono::system_clock::now());
  const zones manager{cin, system_time};

  while (true) {
    for (const auto zone_time : manager.localize(system_time)) {
      const auto name = zone_time.get_time_zone()->name();
      cout << format("{:<32}{:%a %b %d %r %z}\n", name, zone_time);
    }
    cout << flush;
    sleep_until(system_time += chrono::seconds(1));
    manager.clear_output(cout);
  }
}
