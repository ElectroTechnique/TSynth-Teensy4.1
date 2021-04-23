#ifndef TSYNTH_UTILS_H
#define TSYNTH_UTILS_H

#include <Arduino.h>

// Clamp a value to the given range.
template <typename NumericType>
NumericType inRangeOrDefault(NumericType value, NumericType defaultValue, NumericType min, NumericType max) {
  static_assert(std::is_arithmetic<NumericType>::value, "NumericType must be numeric");
  if (value < min || value > max) return defaultValue;
  return value;
}

String milliToString(float milli) {
    if (milli < 1000) return String(int(milli)) + " ms";
    return String(milli / 1000) + " s";
}

#endif