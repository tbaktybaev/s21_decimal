#include "s21_decimal.h"

int s21_mul(s21_decimal val_1, s21_decimal val_2, s21_decimal *result) {
  arithmetic_errors error = ok;
  if (!result) {
    error = other_error;
  } else if (s21_is_zero(&val_2) || s21_is_zero(&val_1)) {
    s21_set_decimal_to_zero(result);
  } else {
    bool sign = s21_is_negative(&val_1) != s21_is_negative(&val_2);
    unsigned exp = s21_get_exp(&val_1) + s21_get_exp(&val_2);
    size_t count_bits_2 = s21_count_significant_bits(&val_2);
    bool res_arr[S21_MANTISSA_SIZE * 2] = {false};
    bool val_1_arr[S21_MANTISSA_SIZE * 2] = {false};
    bool val_2_arr[S21_MANTISSA_SIZE * 2] = {false};
    s21_decimal_to_array(&val_1, val_1_arr);
    s21_decimal_to_array(&val_2, val_2_arr);
    for (size_t i = 0; i < count_bits_2; ++i) {
      if (val_2_arr[i]) s21_array_add(res_arr, val_1_arr, res_arr);
      if (i + 1 < count_bits_2) s21_shift_array_to_left(val_1_arr, 1);
    }
    unsigned remainder = 0;
    if (s21_reduce_exp(res_arr, &exp, &remainder))
      error = sign ? neg_inf : pos_inf;

    if (error || s21_is_array_empty(res_arr))
      s21_set_decimal_to_zero(result);
    else {
      s21_array_to_decimal(res_arr, result);
      s21_bank_round_u(result, remainder, true);
      s21_set_exp(result, exp);
      if (sign) s21_make_negative(result);
    }
  }
  return error;
}
