want =  hw6
make:
	scp $(want)/*.cpp b04611017@linux15.csie.ntu.edu.tw:~/$(want)/
all:
	scp -r $(want)/ b04611017@linux15.csie.ntu.edu.tw:~/

station = $(shell python3 get_station.py)
con:
	$(info $(station))
	ssh b04611017@$(station).csie.ntu.edu.tw
