CXX = mpic++

# Cast and conversation warnings
CXXFLAGS += -Wcast-align -Wcast-qual -Wno-old-style-cast -Wconversion -Wsign-conversion -Wsign-promo

# Strict warnings
CXXFLAGS += -Wstrict-null-sentinel -Wstrict-overflow=2 -fstrict-overflow

# Stack warnings
CXXFLAGS += -fstack-check -fstack-protector -Wstack-usage=8192

# switch warnings
CXXFLAGS += -Wswitch-default -Wswitch-enum

# format warnings
CXXFLAGS += -Wformat-nonliteral -Wformat-security -Wformat=2

# Other Very usefull warnings
CXXFLAGS += -g -Waggressive-loop-optimizations -Wchar-subscripts -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Winline -Wlarger-than=8192 -Wlogical-op -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wsync-nand -Wundef -Wunreachable-code -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-varargs -fno-omit-frame-pointer -fPIE -lm -pie



OBJS = main.o

all: main

main: $(OBJS)
	@${CXX} $^ -o $@ -MD ${CXXFLAGS} ${LDLIBS}

%.o: %.cpp
	@${CXX} -c $^ -o  $@ -MD ${CXXFLAGS} ${LDLIBS}

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt mpirun -np $(np) --oversubscribe --allow-run-as-root -v ./main $(K) $(M)

dopusk:
	@${CXX} dopusk.cpp -o dopusk ${CXXFLAGS} 

dopusk_run:
	@mpirun -np 2 --oversubscribe --allow-run-as-root ./dopusk

run:
	@mpirun -np $(np) --oversubscribe --allow-run-as-root -v ./main $(K) $(M)



clean:
	@$(RM) $(PROG) *.o *.d *.gcov *.gcda *.gcno *.swp test *.info valgrind-out.txt
	@rm -rf main dopusk



