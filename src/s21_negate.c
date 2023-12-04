#include "s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  int error = 1;
  if (result) {
    error = 0;
    *result = value;
    result->bits[S21_DECIMAL_SIZE - 1] =
        s21_switchbit(result->bits[S21_DECIMAL_SIZE - 1], 31);
  }
  return error;
}
