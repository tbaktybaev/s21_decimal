#include "s21_decimal.h"

int s21_div(s21_decimal val_1, s21_decimal val_2, s21_decimal *result) {
  arithmetic_errors error = ok;
  if (!result) {
    error = other_error;
  } else if (s21_is_zero(&val_2)) {
    error = div_zero;
  } else if (s21_is_zero(&val_1)) {
    s21_set_decimal_to_zero(result);
  } else {
    bool sign = s21_is_negative(&val_1) != s21_is_negative(&val_2);
    int exp = s21_get_exp(&val_1) - s21_get_exp(&val_2);
    if (s21_fractional_div(&val_1, val_2, result, &exp))
      error = sign ? neg_inf : pos_inf;
    else {
      for (bool stop = false; !stop && exp < 0;)
        if (!(stop = s21_increase_exp(result, 1))) exp++;
      if (exp < 0)
        error = sign ? neg_inf : pos_inf;
      else
        s21_bank_round_d(val_1, val_2, result);
    }
    if (error || s21_is_zero(result))
      s21_set_decimal_to_zero(result);
    else {
      s21_set_exp(result, exp);
      if (sign) s21_make_negative(result);
    }
  }
  return error;
}
