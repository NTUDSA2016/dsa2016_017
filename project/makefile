.PHONY: all normal debug run test clean check
name = project.cpp
test_data_path = demo/
diff_path = 

all:
	g++ -std=c++11 $(name) -o out -O3
normal:
	g++ -std=c++11 $(name) -o out
debug:
	g++ -std=c++11 $(name) -o out -g -Wall
run:
	./out
check:
	make all
	python3 ../check.py $(test_data_path) $(diff_path)


