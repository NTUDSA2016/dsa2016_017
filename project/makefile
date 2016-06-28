.PHONY: all normal debug run test clean check
name = project.cpp
preprocess_path = /tmp/b04611017/preprocess.data
test_data_path = mytest/
diff_path = ../default_diff.py
how_to_run = ./execute.sh%$(preprocess_path)
#use % to separate

all:
	g++ -std=c++11 -O3   run_$(name) -o runout
	g++ -std=c++11 -O3 build_$(name) -o buildout
run:
	./runout $(arg)
build:
	/usr/bin/time -f "%Us"  -a ./buildout $(arg)
clean:
	rm runout buildout -f
check:
	./compile.sh
	./preprocess.sh $(preprocess_path)
	python3 ../check.py $(test_data_path) $(diff_path) $(how_to_run)
	rm $(preprocess_path) -f 
	make clean


