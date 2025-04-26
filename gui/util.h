#ifndef UTIL_H
#define UTIL_H

#include <QString>

/**
 * Given `total_levels', returns an integer between 4 and 12 which is a linear
 * map of `level' onto `total_levels'. This is used for generating cache sizes
 * given a number of levels.
 * @param the total number of cache levels, zero-indexed.
 * @param a numberedcache level, zero-indexed.
 * @return an integer between 4-12, linearly scaled with level.
 */
int cache_size_mapper(int total_levels, int level);

/**
 * Contains the main formatting logic used to format integers. Uses 2's
 * complement for hexadecimal numbers.
 * @param the value to be formated
 * @param if the value should be displayed in hex. If false, displays in
 * decimal.
 * @param if the value should display.
 * @return a string respecting the above parameters.
 */
QString format_toggled_value(int value, bool is_hex, bool is_cleared = false);

#endif // UTIL_H
