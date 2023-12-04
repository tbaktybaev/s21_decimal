#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define S21_DEC_MAX 79228162514264337593543950335.
#define S21_DECIMAL_SIZE 4
#define S21_MANTISSA_SIZE 96
#define BUFF 20

typedef struct {
  unsigned bits[S21_DECIMAL_SIZE];
} s21_decimal;

typedef enum { ok, pos_inf, neg_inf, div_zero, other_error } arithmetic_errors;

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_is_less(s21_decimal value_1, s21_decimal value_2);
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

int s21_fractional_div(s21_decimal *dividend, s21_decimal divisor,
                       s21_decimal *result, int *exp);
unsigned s21_checkbit(const unsigned value, const unsigned position);
unsigned s21_setbit(const unsigned value, const unsigned position);
unsigned s21_switchbit(const unsigned value, const unsigned position);
bool s21_check_decimal_bit(const s21_decimal *number, const size_t idx);
void s21_set_decimal_bit(s21_decimal *number, const size_t idx);
void s21_switch_decimal_bit(s21_decimal *number, const size_t idx);
void s21_set_decimal_to_zero(s21_decimal *number);
int s21_set_exp(s21_decimal *number, const unsigned exp);
unsigned s21_get_exp(const s21_decimal *number);
size_t s21_count_significant_bits(const s21_decimal *number);
int s21_mantissa_left_shift(s21_decimal *number, size_t shift);
int s21_mantissa_right_shift(s21_decimal *number, size_t shift);
int s21_mantissa_add(s21_decimal val_1, s21_decimal val_2, s21_decimal *result);
int s21_mantissa_mul(s21_decimal val_1, s21_decimal val_2, s21_decimal *result);
int s21_mantissa_sub(s21_decimal val_1, s21_decimal val_2, s21_decimal *result);
int s21_mantissa_div(s21_decimal val_1, s21_decimal val_2, s21_decimal *result,
                     s21_decimal *remainder);
int s21_take_one(s21_decimal *number, size_t idx, size_t bits);
bool s21_is_zero(const s21_decimal *number);
bool s21_is_negative(const s21_decimal *number);
int s21_make_negative(s21_decimal *number);
unsigned s21_normalize(s21_decimal *val_1, s21_decimal *val_2);
int s21_increase_exp(s21_decimal *number, unsigned count);
unsigned s21_decrease_exp(s21_decimal *number, unsigned count);
int s21_bank_round_u(s21_decimal *number, const unsigned remainder,
                     bool is_add);
int s21_bank_round_d(s21_decimal remainder, s21_decimal divisor,
                     s21_decimal *result);
void print_decimal(s21_decimal *dec);
void remove_char_from_str(char *dest, int position);
int s21_set_mantissa(s21_decimal *number, const unsigned mantissa);
size_t s21_count_bits_in_array(const bool *array);
void s21_clear_array(bool *array);
void s21_copy_array(const bool *src, bool *dst);
bool s21_is_array_empty(const bool *array);
int s21_array_add(const bool *val_1, const bool *val_2, bool *res);
int s21_decimal_to_array(const s21_decimal *src, bool *dst);
int s21_array_to_decimal(const bool *src, s21_decimal *dst);
int s21_shift_array_to_left(bool *array, const size_t shift);
int s21_shift_array_to_right(bool *array, const size_t shift);
int s21_take_one_in_array(bool *array, size_t idx, const size_t bits);
int s21_array_sub(const bool *val_1, const bool *val_2, bool *res);
int s21_array_div(bool *val_1, bool *val_2, bool *res, bool *remainder);
bool s21_is_array_greater(bool *arr_1, bool *arr_2);
int s21_reduce_exp(bool *res_arr, unsigned *exp, unsigned *remainder);

#endif  // SRC_S21_DECIMAL_H_
