/*
Time zone management interface.

Copyright 2026. Andrew Wang.
*/
#pragma once

#include <chrono>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

using instant_t = std::chrono::sys_time<std::chrono::seconds>;
using zoned_t = std::chrono::zoned_time<std::chrono::seconds>;
using tz_t = std::chrono::time_zone;

/**
 * @brief Manage and validate time zones.
 */
class zones {
 private:
  static constexpr auto CLEAR_TEMPLATE = "\x1b[{}A\x1b[1G";
  std::vector<const tz_t*> m_tzs;

 public:
  /**
   * @brief Construct with only current time zone.
   */
  explicit zones();

  /**
   * @brief Construct from input stream with time zones.
   * @param is Input stream with white-space separated zones.
   * @param time System time at initialization.
   */
  zones(std::istream& is, instant_t time);

  /**
   * @brief Localize given time with stored zones.
   * @param time The system time to localize.
   * @return View of zoned times for each managed zone.
   */
  inline auto localize(instant_t time) const;

  /**
   * @brief Clear the output assuming one line per zone.
   * @param os The output stream to clear.
   */
  void clear_output(std::ostream& os) const;

 private:
  /**
   * @brief Get time zone if and only if it is valid.
   * @param db IANA time zone database.
   * @param token The potential time zone name.
   * @return Pointer into tzdb if token is a canonical zone.
   */
  static const tz_t* get_valid_zone(const std::chrono::tzdb& db,
                                    std::string_view token);

  /**
   * @brief Custom comparator for time zones given a time point.
   */
  struct cmp {
    instant_t m_time;

    /**
     * @brief Compare two time zones with offset then name.
     * @param lhs The left time zone.
     * @param rhs The right time zone.
     * @return Whether lhs compares less than rhs.
     */
    bool operator()(const tz_t* lhs, const tz_t* rhs) const;
  };
};

// AUTO RETURN TYPE IMPLEMENTATIONS

auto zones::localize(instant_t time) const {
  const auto to_local = [time](const auto* tz) { return zoned_t{tz, time}; };
  return m_tzs | std::views::transform(to_local);
}
