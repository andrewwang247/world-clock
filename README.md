# World Clock

CLI world clock utility that displays local dates and times in user-selected IANA time zones.

## Usage

Build and run with the included `Makefile`. Time zones are specified via `stdin`. The clock has 2 initialization modes.

1. If `stdin` is attached to a live terminal, e.g. running the executable by itself, the host computer's current time zone is used.
2. If `stdin` is piped or input redirected, white space separated zone names are read until `EOF`.

```bash
./build/clock                                               # Option 1 live terminal
echo "America/Los_Angeles Europe/Rome" | ./build/clock      # Option 2 piping
./build/clock < zone_list.txt                               # Option 2 redirection
```

## Display

Times are printed to `stdout` and will update every second. Time zones are sorted primarily by offset, followed by name. We use the default "C" locale. From left to right, each line shows:

1. Canonical IANA time zone name.
2. Abbreviated weekday name.
3. Abbreviated month name and day of month.
4. 12-hour clock time with AM/PM.
5. Time zone offset in ISO 8601 format.

## Time Zones

The clock relies on an [IANA time zone database](https://en.wikipedia.org/wiki/List_of_tz_database_time_zones) provided by your standard library implementation and OS.

- Specify time zones by their exact case-sensitive name.
- Multiple zones should be white space separated.
- Only use canonical time zone names. Links and aliases are disallowed.
- Duplicate time zones are ignored.

We throw `std::runtime_error` if requirements are not met.
