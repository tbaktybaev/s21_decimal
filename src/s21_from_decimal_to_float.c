#include "s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int error = 0;
  unsigned exp = s21_get_exp(&src);

  if (dst == NULL)
    error = 1;
  else {
    long double result = 0;
    if (s21_is_zero(&src))
      *dst = 0.0;
    else {
      for (int i = 0; i < 96; i++)
        result += pow(2, i) * (int)s21_check_decimal_bit(&src, i);
      for (; exp > 0; exp--) result /= 10.0;
    }
    if (s21_is_negative(&src)) result *= -1;
    *dst = (float)result;
  }
  return error;
}
