import os

for i in range(1,49):
    num = "{0:02}".format(i)
    os.system("curl http://mirlab.org/jang/courses/dsa/homework/2016/hw06/testSet/"+num+"_input.txt > "+num+".in");
    os.system("curl http://mirlab.org/jang/courses/dsa/homework/2016/hw06/testSet/"+num+"_output.txt > "+num+".out");
