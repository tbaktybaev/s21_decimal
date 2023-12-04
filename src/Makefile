CFLAGS = -Wall -Werror -Wextra -std=c11 -g -D_GNU_SOURCE
CHECKF = -lcheck -lm -lsubunit

all: clean gcov_report

s21_decimal.a: s21_*.c
	gcc $(CFLAGS) -c s21_*.c utils.c
	ar -rcs s21_decimal.a *.o
	ranlib s21_decimal.a

test:
	gcc $(CFLAGS) --coverage -c *.c
	ar -rcs s21_gcov_decimal.a *.o
	ranlib s21_gcov_decimal.a
	gcc $(CFLAGS) --coverage -c test_s21_decimal.c
	gcc $(CFLAGS) --coverage -o test_s21_decimal test_s21_decimal.o s21_gcov_decimal.a  $(CHECKF)
	./test_s21_decimal > tmp_out.log

gcov_report: test
	geninfo s21_*.gcda  -o ./coverage.info --rc lcov_branch_coverage=1
	genhtml coverage.info -o html --rc lcov_branch_coverage=1

clean:
	rm -rf *.o *.a a.out *.gcno *.gcda *.log test_s21_decimal html coverage.info

style_check:
	clang-format -style=google -i *.c *.h
	clang-format -style=google -n *.c *.h
