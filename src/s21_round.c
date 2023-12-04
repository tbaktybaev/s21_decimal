#include "s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  int error = 1;
  if (result) {
    error = 0;
    bool sign = s21_is_negative(&value);
    s21_decimal ten = {{10, 0, 0, 0}};
    s21_decimal one = {{1, 0, 0, 0}};
    s21_decimal remainder = {{0, 0, 0, 0}};
    for (size_t i = s21_get_exp(&value); i > 0; --i)
      s21_mantissa_div(value, ten, &value, &remainder);
    if (remainder.bits[0] >= 5) s21_mantissa_add(value, one, &value);
    if (sign) s21_make_negative(&value);
    *result = value;
  }
  return error;
}
