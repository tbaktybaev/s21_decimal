#include "s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  int error = 1;
  if (result) {
    error = 0;
    unsigned exp = s21_get_exp(&value);
    if (exp) {
      bool sign = s21_is_negative(&value);
      s21_decrease_exp(&value, exp);
      if (sign) {
        s21_decimal one = {{1, 0, 0, 0}};
        s21_mantissa_add(value, one, &value);
        s21_make_negative(&value);
      }
    }
    *result = value;
  }
  return error;
}
