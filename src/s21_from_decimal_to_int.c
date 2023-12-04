#include "s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int error = 1;
  if (dst) {
    error = 0;
    s21_decrease_exp(&src, s21_get_exp(&src));
    if (s21_is_zero(&src)) {
      *dst = 0;
    } else if (s21_count_significant_bits(&src) > 31) {
      error = 1;
      *dst = 0;
    } else {
      *dst = s21_is_negative(&src) ? -src.bits[0] : src.bits[0];
    }
  }
  return error;
}
