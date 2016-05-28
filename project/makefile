.PHONY: all normal debug run test clean check
name = project.cpp
test_data_path = demo
special_judge = diff
all:
	g++ -std=c++11 $(name) -o out -O3
normal:
	g++ -std=c++11 $(name) -o out
debug:
	g++ -std=c++11 $(name) -o out -g -Wall
run:
	./out

test_data_in = $(sort $(wildcard $(test_data_path)/*.in))
test_data_out= $(subst .in,.out,$(test_data_in))
data_out = $(notdir $(test_data_out))
data_diff= $(subst .out,.diff,$(data_out))

%.diff:$(test_data_path)/%.in
	$(info init)
	$(shell echo $< > $@)

%.out:$(test_data_path)/%.in %.diff
	$(eval DIFF = $(word 2,$^))
	$(info main)
	$(shell /usr/bin/time -f "%Us" -o $(DIFF) -a make run --silent < $< > $@) 
	$(info jduge)
	$(eval result = $(special_judge) $@ $(test_data_path)/$@ )
	$(if $(shell $(result)),\
		 $(shell echo WA >> $(DIFF))\
		 $(shell $(result) >> $(DIFF)),\
		 $(shell echo AC >> $(DIFF)))

clean:
	$(info delete)
	$(shell rm *.diff *.out -f)
	
.PRECIOUS: %.diff
test:$(data_out)
	cat $(data_diff)
check:|test
	$(foreach name,$(data_diff),\
		$(if $(shell head -3 $(name) | tail -1 | grep AC),\
			$(shell rm $(name) $(subst .diff,.out,$(name)) -f)))
$(data_out):|all clean
