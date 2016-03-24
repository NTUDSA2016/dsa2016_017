All:
	g++ --std=c++11 -O3 q2.cpp -o out
run: out
	@./out 
normal:
	g++ --std=c++11 q2.cpp -o out
debug:
	g++ --std=c++11 -g q2.cpp -o debug.out -Wall
	gdb debug.out
	rm debug.out
clean:
	rm out -f
