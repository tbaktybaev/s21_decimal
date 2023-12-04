#include "s21_decimal.h"

int s21_sub(s21_decimal val_1, s21_decimal val_2, s21_decimal *result) {
  arithmetic_errors error = other_error;
  if (result) {
    error = ok;
    bool sign = false;
    bool sign_1 = s21_is_negative(&val_1);
    bool sign_2 = s21_is_negative(&val_2);
    if (sign_1) s21_negate(val_1, &val_1);
    if (sign_2) s21_negate(val_2, &val_2);
    if (sign_1 == sign_2) {
      unsigned remainder = s21_normalize(&val_1, &val_2);
      unsigned exp = s21_get_exp(&val_1);
      if (s21_is_greater(val_1, val_2)) {
        sign = sign_1;
        s21_mantissa_sub(val_1, val_2, result);
      } else {
        sign = !sign_1;
        s21_mantissa_sub(val_2, val_1, result);
      }
      s21_bank_round_u(result, remainder, false);
      s21_set_exp(result, exp);
    } else {
      sign = sign_1;
      if (s21_add(val_1, val_2, result)) error = sign ? neg_inf : pos_inf;
    }
    if (error || s21_is_zero(result))
      s21_set_decimal_to_zero(result);
    else if (sign)
      s21_make_negative(result);
  }
  return error;
}
