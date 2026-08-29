/*
Time zone management implementation.

Copyright 2026. Andrew Wang.
*/
#include "zones.h"

#include <algorithm>
#include <chrono>
#include <format>
#include <iostream>
#include <print>
#include <stdexcept>
#include <string>
#include <string_view>

using std::format;
using std::istream;
using std::print;
using std::runtime_error;
using std::string;
using std::string_view;

namespace chrono = std::chrono;
namespace ranges = std::ranges;

zones::zones() : m_tzs({chrono::current_zone()}) {}

zones::zones(istream& is, instant_t time) {
  const auto& db = chrono::get_tzdb();
  for (string token; is >> token;) {
    const auto* tz = get_valid_zone(db, token);
    m_tzs.emplace_back(tz);
  }
  if (m_tzs.empty()) {
    throw runtime_error("No time zones were specified.");
  }

  ranges::sort(m_tzs, cmp{time});
  const auto unique_begin = ranges::unique(m_tzs).begin();
  m_tzs.erase(unique_begin, m_tzs.end());

  m_tzs.shrink_to_fit();
}

void zones::clear_stdout() const { print(CLEAR_TEMPLATE, m_tzs.size()); }

const tz_t* zones::get_valid_zone(const chrono::tzdb& db, string_view token) {
  const auto link_iter =
      ranges::lower_bound(db.links, token, {}, &chrono::time_zone_link::name);
  if (link_iter != db.links.end() && link_iter->name() == token) {
    throw runtime_error(format("{} is a link. Prefer canonical {}.", token,
                               link_iter->target()));
  }
  // Will throw runtime_error if zone does not exist.
  return db.locate_zone(token);
}

bool zones::cmp::operator()(const tz_t* lhs, const tz_t* rhs) const {
  const auto lhs_offset = lhs->get_info(m_time).offset;
  const auto rhs_offset = rhs->get_info(m_time).offset;
  return lhs_offset != rhs_offset ? lhs_offset < rhs_offset
                                  : lhs->name() < rhs->name();
}
