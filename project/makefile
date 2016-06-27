.PHONY: all normal debug run test clean check
name = project.cpp
test_data_path = demo/
diff_path = 

all:
	g++ -std=c++11 run_$(name) -o out -O3
run:
	./out
build:
	g++ -std=c++11 build_$(name) -o out -O3
	./out
	make all 
check:
	make all
	python3 ../check.py $(test_data_path) $(diff_path)


