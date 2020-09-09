all:
	@g++ AMS/efl_create.cpp ../Stack/stack.cpp ../SortLines/sort_lines.cpp -w -I ../Headers/
	@AMS/a.out prog AMS/ams; g++ CPU/cpu.cpp ../Stack/stack.cpp ../SortLines/sort_lines.cpp -w -I../Headers; CPU/a.out AMS/ams

read:
	g++ AMS/ams_read.cpp ../Stack/stack.cpp ../SortLines/sort_lines.cpp -w -I ../Headers/ -o read
	AMS/read AMS/ams
