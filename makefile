want = hw6
make:
	scp $(want)/*.cpp b04611017@linux1.csie.ntu.edu.tw:~/$(want)/
all:
	scp -r $(want)/ b04611017@linux1.csie.ntu.edu.tw:~/
con:
	ssh b04611017@linux13.csie.ntu.edu.tw
