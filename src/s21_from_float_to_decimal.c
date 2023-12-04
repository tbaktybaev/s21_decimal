#include "s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int error = 0, is_big_num = 0;
  if ((fabsl(src) < 1e-28)) {
    error = 1;
    s21_set_decimal_to_zero(dst);
  } else if (fabsl(src) > S21_DEC_MAX || finite(src) == 0) {
    error = 1;
  } else {
    char dest[BUFF] = "\0";
    sprintf(dest, "%.7g", src);
    if (memcmp(dest, "-", 1) == 0) {
      dst->bits[3] = s21_setbit(dst->bits[3], 31);
      remove_char_from_str(dest, 1);
    }
    double mantissa = 0.0;
    unsigned res_mantissa = 0;
    int exponenta = 0;
    if (strchr(dest, 'e')) {
      char *float_num;
      char *exp;
      float_num = strtok(dest, "e");
      exp = strtok(NULL, "e");
      exponenta = atoi(exp);
      mantissa = atof(float_num);
      if (exponenta < 0) {
        exponenta = abs(exponenta);
        for (int len = strlen(float_num) - 2; len >= 0 && exponenta < 28;
             len--, exponenta++)
          mantissa *= 10.0;
        res_mantissa = (int)(roundf(mantissa));
        while (res_mantissa % 10 == 0 && exponenta > 0) {
          res_mantissa /= 10.0;
          exponenta--;
        }
      } else if (exponenta > 0) {
        for (int len = strlen(float_num) - 3; len >= 0 && exponenta > 0;
             len--, exponenta--)
          mantissa *= 10.0;
        res_mantissa = round(mantissa);
        s21_set_mantissa(dst, res_mantissa);
        s21_increase_exp(dst, exponenta);
        exponenta = 0;
        is_big_num = 1;
      }
    } else {
      mantissa = atof(dest);
      if (strchr(dest, '.') && (int)mantissa < 10) {
        for (int len = strlen(dest) - 2; len >= 0 && exponenta < 28;
             len--, exponenta++)
          mantissa *= 10.0;
        res_mantissa = (int)(round(mantissa));
        while (res_mantissa % 10 == 0 && exponenta > 0) {
          res_mantissa /= 10.0;
          exponenta--;
        }
      } else if (mantissa > (int)mantissa) {
        while (exponenta < 8 && mantissa - (int)mantissa > 0.00000001) {
          mantissa *= 10.0;
          exponenta++;
        }
        res_mantissa = (int)mantissa;
      } else {
        res_mantissa = (int)(atof(dest));
      }
    }
    s21_set_exp(dst, abs(exponenta));
    if (!is_big_num) s21_set_mantissa(dst, res_mantissa);
  }
  return error;
}
