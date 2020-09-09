all:
	@nasm -w+all -f elf64 -o 'hello_world.o' 'main.asm'; ld -o 'hello_world.out' 'hello_world.o'
	@tail -c +129 hello_world.out>my_stdlib; truncate --size=666 my_stdlib
