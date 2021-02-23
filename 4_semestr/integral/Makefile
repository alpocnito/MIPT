LDLIBS = -I../Headers
CXX := g++

# Compilator
CXXFLAGS += -std=c++17

# Cast and conversation warnings
CXXFLAGS += -Wcast-align -Wcast-qual -Wuseless-cast -Wno-old-style-cast -Wconversion -Wsign-conversion -Wsign-promo

# Sanitizer warnings
CXXFLAGS += -fsanitize=bool -fsanitize=enum -fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero -fsanitize=integer-divide-by-zero -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=object-size -fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=shift -fsanitize=signed-integer-overflow -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=vptr

# Strict warnings
CXXFLAGS += -Wstrict-null-sentinel -Wstrict-overflow=2 -fstrict-overflow

# Stack warnings
CXXFLAGS += -fstack-check -fstack-protector -Wstack-usage=8192

# Suggestions
CXXFLAGS += -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override

# switch warnings
CXXFLAGS += -Wswitch-default -Wswitch-enum

# format warnings
CXXFLAGS += -Wformat-signedness -Wformat-nonliteral -Wformat-security -Wformat=2

# Other Very usefull warnings
CXXFLAGS += -g -Waggressive-loop-optimizations -Wchar-subscripts -Wconditionally-supported -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Winline -Wlarger-than=8192 -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wsync-nand -Wundef -Wunreachable-code -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-varargs -fsized-deallocation -flto-odr-type-merging -fno-omit-frame-pointer -fPIE -lm -pie


all: test

test: rbt.h rbt_inner.h my_system.h
test: unit_tests.o rbt.o my_system.o rbt_inner.o
	@${CXX} $^ -o $@ -fprofile-arcs ${CXXFLAGS} ${LDLIBS}

%.o: %.cpp
	@${CXX} -c $^ -o $@ --coverage ${CXXFLAGS} ${LDLIBS}

lcov: test
	@lcov/bin/lcov -t "result" -o ex_test.info -c -d .; lcov/bin/genhtml -o res ex_test.info
	@make clean

valgrind: test
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./test

clean:
	@$(RM) $(PROG) *.o *.d *.gcov *.gcda *.gcno *.swp test *.info valgrind-out.txt

