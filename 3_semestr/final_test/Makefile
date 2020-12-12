LDLIBS=-I../../1_semestr/Headers/
CXX:=g++
PROG=sem

# Compilator
CXXFLAGS += -std=c++17

# Compound flags
CXXFLAGS += -Wall -Wextra -Weffc++ 

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
CXXFLAGS += -g -Waggressive-loop-optimizations -Wchar-subscripts -Wconditionally-supported -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Winline -Wlarger-than=8192 -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wsync-nand -Wundef -Wunreachable-code -Wunused  -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-varargs -fsized-deallocation -flto-odr-type-merging -fno-omit-frame-pointer -fPIE -lm -pie


all:
	@${CXX} ${PROG}.cpp -o ${PROG} ${CXXFLAGS} ${LDLIBS}

clean:
	@$(RM) $(PROG)

