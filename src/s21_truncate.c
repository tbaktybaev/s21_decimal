#include "s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int error = 1;
  if (result) {
    error = 0;
    s21_decrease_exp(&value, s21_get_exp(&value));
    *result = value;
  }
  return error;
}
