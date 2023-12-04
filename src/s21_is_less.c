#include "s21_decimal.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int result = 0;
  if (!s21_is_equal(value_1, value_2)) {
    bool sign_1 = s21_is_negative(&value_1);
    bool sign_2 = s21_is_negative(&value_2);
    if (s21_is_zero(&value_1))
      result = !sign_2;
    else if (s21_is_zero(&value_2))
      result = sign_1;
    else if (sign_1 && !sign_2)
      result = 1;
    else if (!sign_1 && sign_2)
      ;
    else {
      s21_normalize(&value_1, &value_2);
      size_t bits_1 = s21_count_significant_bits(&value_1);
      size_t bits_2 = s21_count_significant_bits(&value_2);
      if (bits_1 == bits_2) {
        for (bool stop = false; !stop; --bits_1) {
          bool bit_1 = s21_check_decimal_bit(&value_1, bits_1 - 1);
          bool bit_2 = s21_check_decimal_bit(&value_2, bits_1 - 1);
          if (bit_1 != bit_2) {
            stop = true;
            result = (bit_1 && sign_1) || (!bit_1 && !sign_1);
          }
        }
      } else
        result = bits_1 > bits_2 ? sign_1 : !sign_1;
    }
  }
  return result;
}
