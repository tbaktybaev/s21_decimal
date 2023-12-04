#include "s21_decimal.h"

int s21_fractional_div(s21_decimal *dividend, s21_decimal divisor,
                       s21_decimal *result, int *exp) {
  int error = 0;
  s21_set_decimal_to_zero(result);
  s21_decimal quotient = {{0, 0, 0, 0}};
  for (bool stop = false; !stop;) {
    s21_mantissa_div(*dividend, divisor, &quotient, dividend);
    s21_mantissa_add(*result, quotient, result);
    if (!s21_is_zero(dividend) && *exp < 28) {
      s21_decimal temp_result = *result;
      s21_decimal ten = {{10, 0, 0, 0}};
      s21_decimal remainder = *dividend;
      if (!(stop = s21_mantissa_mul(temp_result, ten, &temp_result))) {
        (*exp)++;
        *result = temp_result;
      } else if (*exp < 0)
        error = 1;
      if (!stop && s21_mantissa_mul(remainder, ten, &remainder))
        s21_mantissa_div(divisor, ten, &divisor, NULL);
      else
        *dividend = remainder;
    } else
      stop = true;
  }
  return error;
}

size_t s21_count_bits_in_array(const bool *array) {
  size_t bits = S21_MANTISSA_SIZE * 2;
  while (bits > 0 && !array[bits - 1]) --bits;
  return bits;
}

void s21_clear_array(bool *array) {
  for (size_t i = 0, bits = S21_MANTISSA_SIZE * 2; i < bits; ++i)
    array[i] = false;
}

void s21_copy_array(const bool *src, bool *dst) {
  for (size_t i = 0, bits = S21_MANTISSA_SIZE * 2; i < bits; ++i)
    dst[i] = src[i];
}

bool s21_is_array_empty(const bool *array) {
  return s21_count_bits_in_array(array) == 0;
}

int s21_array_add(const bool *val_1, const bool *val_2, bool *res) {
  unsigned buffer = 0;
  for (size_t i = 0, bits = S21_MANTISSA_SIZE * 2; i < bits; ++i) {
    unsigned sum_of_bits = val_1[i] + val_2[i] + buffer;
    buffer = sum_of_bits >> 1;
    res[i] = sum_of_bits % 2;
  }
  return 0;
}

int s21_decimal_to_array(const s21_decimal *src, bool *dst) {
  for (size_t i = 0; i < S21_MANTISSA_SIZE; ++i)
    dst[i] = s21_check_decimal_bit(src, i);
  return 0;
}

int s21_array_to_decimal(const bool *src, s21_decimal *dst) {
  s21_set_decimal_to_zero(dst);
  size_t bits = s21_count_bits_in_array(src);
  size_t start_idx = bits > S21_MANTISSA_SIZE ? bits - S21_MANTISSA_SIZE : 0;
  for (size_t i = 0; i < S21_MANTISSA_SIZE; ++i, ++start_idx)
    if (src[start_idx]) s21_set_decimal_bit(dst, i);
  return 0;
}

int s21_shift_array_to_left(bool *array, const size_t shift) {
  bool temp[S21_MANTISSA_SIZE * 2] = {false};
  size_t bits = S21_MANTISSA_SIZE * 2;
  for (size_t i = 0; i < bits - shift; ++i) temp[i + shift] = array[i];
  s21_copy_array(temp, array);
  return 0;
}

int s21_shift_array_to_right(bool *array, const size_t shift) {
  bool temp[S21_MANTISSA_SIZE * 2] = {false};
  size_t bits = S21_MANTISSA_SIZE * 2;
  for (size_t i = 0; i < bits - shift; ++i) temp[i] = array[i + shift];
  s21_copy_array(temp, array);
  return 0;
}

int s21_take_one_in_array(bool *array, size_t idx, const size_t bits) {
  int error = 1;
  for (; error && idx < bits; ++idx) {
    if (array[idx]) error = 0;
    array[idx] = !array[idx];
  }
  return error;
}

int s21_array_sub(const bool *val_1, const bool *val_2, bool *res) {
  int error = 0;
  s21_clear_array(res);
  size_t bits_1 = s21_count_bits_in_array(val_1);
  size_t bits_2 = s21_count_bits_in_array(val_2);
  if (bits_2 > bits_1) {
    error = 1;
  } else {
    bool temp_res[S21_MANTISSA_SIZE * 2] = {false};
    bool temp_val_1[S21_MANTISSA_SIZE * 2] = {false};
    s21_copy_array(val_1, temp_val_1);
    for (size_t i = 0; !error && i < bits_1; ++i) {
      unsigned bit_1 = temp_val_1[i];
      unsigned bit_2 = val_2[i];
      if (bit_1 < bit_2) error = s21_take_one_in_array(temp_val_1, i, bits_1);
      if (!error && (bit_1 - bit_2)) temp_res[i] = true;
    }
    if (!error) s21_copy_array(temp_res, res);
  }
  return error;
}

int s21_array_div(bool *val_1, bool *val_2, bool *res, bool *remainder) {
  int error = 1;
  if (!s21_is_array_empty(val_2)) {
    error = 0;
    bool temp_res[S21_MANTISSA_SIZE * 2] = {false};
    size_t bits_1 = s21_count_bits_in_array(val_1);
    size_t bits_2 = s21_count_bits_in_array(val_2);
    bool _remainder[S21_MANTISSA_SIZE * 2] = {false};
    int steps = bits_1 - bits_2 + 1;
    if (steps > 0) {
      bool minuend[S21_MANTISSA_SIZE * 2] = {false};
      s21_copy_array(val_1, minuend);
      s21_shift_array_to_right(minuend, bits_1 - bits_2);
      for (size_t i = 0; i < (size_t)steps; ++i) {
        if (s21_array_sub(minuend, val_2, _remainder))
          s21_copy_array(minuend, _remainder);
        else
          temp_res[steps - i - 1] = true;
        if (i + 1 < (size_t)steps) {
          s21_shift_array_to_left(_remainder, 1);
          if (val_1[bits_1 - 1 - bits_2 - i]) _remainder[0] = true;
          s21_copy_array(_remainder, minuend);
        }
      }
    }
    s21_copy_array(temp_res, res);
    s21_copy_array(_remainder, remainder);
  }
  return error;
}

bool s21_is_array_greater(bool *arr_1, bool *arr_2) {
  bool result = false;
  size_t bits_1 = s21_count_bits_in_array(arr_1);
  size_t bits_2 = s21_count_bits_in_array(arr_2);
  if (bits_1 == bits_2) {
    for (bool stop = false; !stop && bits_1 > 0; --bits_1)
      if (arr_1[bits_1 - 1] != arr_2[bits_1 - 1]) {
        stop = true;
        result = arr_1[bits_1 - 1] > arr_2[bits_1 - 1];
      }
  } else
    result = bits_1 > bits_2;
  return result;
}

int s21_reduce_exp(bool *res_arr, unsigned *exp, unsigned *remainder) {
  int error = 0;
  s21_decimal max_dec = {{0xffffffff, 0xffffffff, 0xffffffff, 0}};
  s21_decimal ten_dec = {{10, 0, 0, 0}};
  bool max_arr[S21_MANTISSA_SIZE * 2] = {false};
  bool ten_arr[S21_MANTISSA_SIZE * 2] = {false};
  bool rem_arr[S21_MANTISSA_SIZE * 2] = {false};
  s21_decimal_to_array(&max_dec, max_arr);
  s21_decimal_to_array(&ten_dec, ten_arr);
  for (; !error && (*exp > 28 || s21_is_array_greater(res_arr, max_arr));) {
    *exp > 0 ? (*exp)-- : (error = 1);
    if (!error) s21_array_div(res_arr, ten_arr, res_arr, rem_arr);
  }
  if (!error) {
    s21_decimal rem_dec = {{0, 0, 0, 0}};
    s21_array_to_decimal(rem_arr, &rem_dec);
    *remainder = rem_dec.bits[0];
  }
  return error;
}

void s21_set_decimal_to_zero(s21_decimal *number) {
  for (size_t i = 0; i < S21_DECIMAL_SIZE; ++i) number->bits[i] = 0;
}

unsigned s21_checkbit(const unsigned value, const unsigned position) {
  return ((value & ((unsigned)1 << position)) != 0);
}

unsigned s21_setbit(const unsigned value, const unsigned position) {
  return (value | ((unsigned)1 << position));
}

unsigned s21_switchbit(const unsigned value, const unsigned position) {
  return (value ^ ((unsigned)1 << position));
}

void s21_set_decimal_bit(s21_decimal *number, const size_t idx) {
  number->bits[idx / 32] = s21_setbit(number->bits[idx / 32], idx % 32);
}

void s21_switch_decimal_bit(s21_decimal *number, const size_t idx) {
  number->bits[idx / 32] = s21_switchbit(number->bits[idx / 32], idx % 32);
}

bool s21_check_decimal_bit(const s21_decimal *number, const size_t idx) {
  return s21_checkbit(number->bits[(idx) / 32], (idx) % 32);
}

int s21_set_exp(s21_decimal *number, const unsigned exp) {
  int error = 0;
  if (number && exp < 29)
    number->bits[S21_DECIMAL_SIZE - 1] =
        (number->bits[S21_DECIMAL_SIZE - 1] & ~(0xFF << 16)) | (exp << 16);
  else
    error = 1;
  return error;
}

unsigned s21_get_exp(const s21_decimal *number) {
  return (number->bits[S21_DECIMAL_SIZE - 1] & (0xFF << 16)) >> 16;
}

size_t s21_count_significant_bits(const s21_decimal *number) {
  int count = -1;
  for (size_t i = 0; i < S21_MANTISSA_SIZE; ++i)
    if (s21_check_decimal_bit(number, i)) count = i + 1;
  return count < 0 ? 0 : count;
}

int s21_mantissa_add(s21_decimal val_1, s21_decimal val_2, s21_decimal *res) {
  int error = 1;
  if (res) {
    error = 0;
    s21_set_decimal_to_zero(res);
    size_t count_bits_1 = s21_count_significant_bits(&val_1);
    size_t count_bits_2 = s21_count_significant_bits(&val_2);
    size_t bits = count_bits_1 > count_bits_2 ? count_bits_1 : count_bits_2;
    unsigned buffer = 0;
    for (size_t i = 0; i < bits; ++i) {
      unsigned bit_1 = s21_check_decimal_bit(&val_1, i);
      unsigned bit_2 = s21_check_decimal_bit(&val_2, i);
      unsigned sum_of_bits = bit_1 + bit_2 + buffer;
      buffer = sum_of_bits >> 1;
      if (sum_of_bits % 2) s21_set_decimal_bit(res, i);
    }
    if (buffer) {
      if (bits == S21_MANTISSA_SIZE)
        error = 1;
      else
        s21_set_decimal_bit(res, bits);
    }
  }
  return error;
}

int s21_mantissa_left_shift(s21_decimal *number, size_t shift) {
  int error = 1;
  if (number) {
    error = 0;
    size_t count_bit = s21_count_significant_bits(number);
    if (count_bit + shift > S21_MANTISSA_SIZE) {
      error = 1;
    } else if (shift > 0) {
      s21_decimal temp = {{0, 0, 0, 0}};
      temp.bits[S21_DECIMAL_SIZE - 1] = number->bits[S21_DECIMAL_SIZE - 1];
      for (size_t i = 0; i < count_bit; ++i)
        if (s21_check_decimal_bit(number, i))
          s21_set_decimal_bit(&temp, i + shift);
      *number = temp;
    }
  }
  return error;
}

int s21_mantissa_right_shift(s21_decimal *number, size_t shift) {
  int error = 1;
  if (number) {
    error = 0;
    size_t count_bit = s21_count_significant_bits(number);
    if (shift > 0) {
      s21_decimal temp = {{0, 0, 0, 0}};
      temp.bits[S21_DECIMAL_SIZE - 1] = number->bits[S21_DECIMAL_SIZE - 1];
      if (count_bit > shift)
        for (size_t i = 0; i < count_bit - shift; ++i)
          if (s21_check_decimal_bit(number, i + shift))
            s21_set_decimal_bit(&temp, i);
      *number = temp;
    }
  }
  return error;
}

int s21_mantissa_mul(s21_decimal val_1, s21_decimal val_2, s21_decimal *res) {
  int error = 1;
  if (res) {
    error = 0;
    s21_set_decimal_to_zero(res);
    size_t count_bits_2 = s21_count_significant_bits(&val_2);
    size_t count_bits_1 = s21_count_significant_bits(&val_1);
    if (count_bits_1 + count_bits_2 > S21_MANTISSA_SIZE + 1) {
      error = 1;
    } else {
      s21_decimal temp = val_1;
      for (size_t i = 0; !error && i < count_bits_2; ++i) {
        if (s21_check_decimal_bit(&val_2, i))
          error = s21_mantissa_add(*res, temp, res);
        if (!error && i + 1 < count_bits_2)
          error = s21_mantissa_left_shift(&temp, 1);
      }
    }
  }
  return error;
}

int s21_mantissa_sub(s21_decimal val_1, s21_decimal val_2, s21_decimal *res) {
  int error = 1;
  if (res) {
    error = 0;
    s21_set_decimal_to_zero(res);
    size_t bits = s21_count_significant_bits(&val_1);
    size_t bits_2 = s21_count_significant_bits(&val_2);
    if (bits_2 > bits) {
      error = 1;
    } else {
      for (size_t i = 0; !error && i < bits; ++i) {
        unsigned bit_1 = s21_check_decimal_bit(&val_1, i);
        unsigned bit_2 = s21_check_decimal_bit(&val_2, i);
        if (bit_1 < bit_2) error = s21_take_one(&val_1, i, bits);
        if (!error && (bit_1 - bit_2)) s21_set_decimal_bit(res, i);
      }
    }
  }
  return error;
}

int s21_take_one(s21_decimal *number, size_t idx, size_t bits) {
  int error = 1;
  for (; error && idx < bits; ++idx) {
    if (s21_check_decimal_bit(number, idx)) error = 0;
    s21_switch_decimal_bit(number, idx);
  }
  return error;
}

int s21_mantissa_div(s21_decimal val_1, s21_decimal val_2, s21_decimal *res,
                     s21_decimal *remainder) {
  int error = 1;
  if (res && !s21_is_zero(&val_2)) {
    error = 0;
    s21_set_decimal_to_zero(res);
    size_t bits_1 = s21_count_significant_bits(&val_1);
    size_t bits_2 = s21_count_significant_bits(&val_2);
    s21_decimal _remainder = val_1;
    int steps = bits_1 - bits_2 + 1;
    if (steps > 0) {
      s21_decimal minuend = val_1;
      s21_mantissa_right_shift(&minuend, bits_1 - bits_2);
      for (size_t i = 0; i < (size_t)steps; ++i) {
        bool bit = s21_mantissa_sub(minuend, val_2, &_remainder);
        if (bit)
          _remainder = minuend;
        else
          s21_set_decimal_bit(res, steps - i - 1);
        if (i + 1 < (size_t)steps) {
          s21_mantissa_left_shift(&_remainder, 1);
          if (s21_check_decimal_bit(&val_1, bits_1 - 1 - bits_2 - i))
            s21_set_decimal_bit(&_remainder, 0);
          minuend = _remainder;
        }
      }
    }
    if (remainder) *remainder = _remainder;
  }
  return error;
}

bool s21_is_zero(const s21_decimal *number) {
  bool result = true;
  for (size_t i = 0; result && i < S21_DECIMAL_SIZE - 1; ++i)
    if (number->bits[i]) result = false;
  return result;
}

bool s21_is_negative(const s21_decimal *number) {
  return s21_checkbit(number->bits[S21_DECIMAL_SIZE - 1], 31);
}

int s21_make_negative(s21_decimal *number) {
  number->bits[S21_DECIMAL_SIZE - 1] =
      s21_setbit(number->bits[S21_DECIMAL_SIZE - 1], 31);
  return 0;
}

unsigned s21_normalize(s21_decimal *value_1, s21_decimal *value_2) {
  unsigned exp_1 = s21_get_exp(value_1);
  unsigned exp_2 = s21_get_exp(value_2);
  unsigned remainder = 0;
  if (exp_1 < exp_2) {
    s21_increase_exp(value_1, exp_2 - exp_1);
    remainder = s21_decrease_exp(value_2, exp_2 - s21_get_exp(value_1));
  } else if (exp_1 > exp_2) {
    s21_increase_exp(value_2, exp_1 - exp_2);
    remainder = s21_decrease_exp(value_1, exp_1 - s21_get_exp(value_2));
  }
  return remainder;
}

int s21_increase_exp(s21_decimal *number, unsigned count) {
  int error = 0;
  s21_decimal temp = *number;
  s21_decimal ten = {{10, 0, 0, 0}};
  unsigned exp = s21_get_exp(number);
  bool sign = s21_is_negative(number);
  for (; !error && count > 0; --count) {
    error = s21_mantissa_mul(temp, ten, &temp);
    if (!error) {
      exp++;
      *number = temp;
    }
  }
  s21_set_exp(number, exp);
  if (sign) s21_make_negative(number);
  return error;
}

unsigned s21_decrease_exp(s21_decimal *number, unsigned count) {
  s21_decimal ten = {{10, 0, 0, 0}};
  s21_decimal remainder = {{0, 0, 0, 0}};
  unsigned exp = s21_get_exp(number) - count;
  bool sign = s21_is_negative(number);
  for (; !s21_is_zero(number) && count > 0; --count)
    s21_mantissa_div(*number, ten, number, &remainder);
  s21_set_exp(number, exp);
  if (sign) s21_make_negative(number);
  return remainder.bits[0];
}

int s21_bank_round_u(s21_decimal *number, const unsigned remainder,
                     bool is_add) {
  int error = 0;
  if (remainder > 5 || (remainder == 5 && s21_check_decimal_bit(number, 0))) {
    s21_decimal one = {{1, 0, 0, 0}};
    error = is_add ? s21_mantissa_add(*number, one, number)
                   : s21_mantissa_sub(*number, one, number);
  }
  return error;
}

int s21_bank_round_d(s21_decimal remainder, s21_decimal divisor,
                     s21_decimal *result) {
  int error = 0;
  s21_mantissa_left_shift(&remainder, 1);
  divisor.bits[S21_DECIMAL_SIZE - 1] = 0;
  if (s21_is_greater(remainder, divisor) ||
      (s21_is_equal(remainder, divisor) && s21_check_decimal_bit(result, 0))) {
    s21_decimal one = {{1, 0, 0, 0}};
    error = s21_mantissa_add(*result, one, result);
  }
  return error;
}

void print_decimal(s21_decimal *dec) {
  for (int j = 3; j >= 0; j--) {
    for (int i = 31; i >= 0; i--) {
      printf("%u", (dec->bits[j] >> i) & 1);
    }
    printf(" ");
  }
  printf("\n");
}

void remove_char_from_str(char *dest, int position) {
  char new_dest[BUFF];
  int end = position;
  for (int i = position; i < (int)strlen(dest); i++) {
    new_dest[i - 1] = dest[i];
    end = i;
  }
  new_dest[end] = '\0';
  strcpy(dest, new_dest);
}

int s21_set_mantissa(s21_decimal *number, const unsigned mantissa) {
  int error = 0;
  if (number)
    number->bits[S21_DECIMAL_SIZE - 4] =
        (number->bits[S21_DECIMAL_SIZE - 4] & ~(0xFF << 0)) | (mantissa << 0);
  else
    error = 1;
  return error;
}
