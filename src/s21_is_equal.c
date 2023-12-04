#include "s21_decimal.h"

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int result = 1;
  if (s21_is_zero(&value_1) && s21_is_zero(&value_2))
    ;
  else if (s21_is_negative(&value_1) != s21_is_negative(&value_2))
    result = 0;
  else {
    s21_normalize(&value_1, &value_2);
    for (size_t i = 0; result && i < S21_DECIMAL_SIZE - 1; ++i)
      if (value_1.bits[i] != value_2.bits[i]) result = 0;
  }
  return result;
}
