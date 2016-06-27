.PHONY: all normal debug run test clean check
name = project.cpp
preprocss_path = /tmp/b04611017/preprocess.data
test_data_path = demo/
diff_path = 

all:
	g++ -std=c++11 run_$(name)    -o runout -O3
	g++ -std=c++11 build_$(name) -o buildout -O3
run:
	./runout $(arg)
build:
	time ./buildout $(arg)
clean:
	rm runout buildout -f
check:
	./compile.sh
	./preprocess.sh $(preprocss_path)
	python3 ../check.py $(test_data_path) $(diff_path) $(preprocss_path)


