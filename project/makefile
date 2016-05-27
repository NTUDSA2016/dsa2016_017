.PHONY: all normal debug run test
name = project.cpp
test_data_path = ~/htdocs/project
all:
	g++ -std=c++11 $(name) -o out -O3
normal:
	g++ -std=c++11 $(name) -o out
debug:
	g++ -std=c++11 $(name) -o out -g -Wall
run:
	./out

test_data_in = $(sort $(wildcard $(test_data_path)/*.in))
test: all
	$(foreach data_in,$(test_data_in), \
		$(eval data_out = $(subst .in,.out,$(data_in))) \
		$(info $(notdir $(data_in) ))\
		$(info $(notdir $(data_out)))\
		$(eval notdir_out = $(notdir $(data_out)))\
		$(shell time make run --silent < $(data_in) > $(notdir_out))\
		$(if $(wildcard $(data_out)),\
		    $(eval wa = $(shell diff -q $(notdir_out) $(data_out))) \
			$(if wa, \
				$(info AC) $(shell rm   -f $(notdir_out)),\
				$(info WA) $(shell diff  $(notdir_out) $(data_out)))))
