me:
	g++ frontend.cpp ../SortLines/sort_lines.cpp ../Stack/stack.cpp -I../Headers/; ./a.out; g++ backend.cpp ../SortLines/sort_lines.cpp ../Stack/stack.cpp -I../Headers/ -fsanitize=address; ./a.out

he:
	g++ frontend_back.cpp ../SortLines/sort_lines.cpp ../Stack/stack.cpp -I../Headers/; ./a.out; g++ normal_funny.cpp ../SortLines/sort_lines.cpp ../Stack/stack.cpp -I../Headers/; ./a.out Programs/collect_program;

run_he:
	g++ frontend_back.cpp ../SortLines/sort_lines.cpp ../Stack/stack.cpp -I../Headers/; ./a.out; cp Programs/collect_program Programs/program; g++ frontend.cpp ../SortLines/sort_lines.cpp ../Stack/stack.cpp -I../Headers/; ./a.out; g++ backend.cpp ../SortLines/sort_lines.cpp ../Stack/stack.cpp -I../Headers/ -fsanitize=address; ./a.out
