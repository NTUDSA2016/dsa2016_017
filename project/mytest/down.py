import os

for i in range(0,6):
    num = "{0:01}".format(i)
    os.system("wget http://www.csie.ntu.edu.tw/~b04611017/project/"+num+".in");
    os.system("wget http://www.csie.ntu.edu.tw/~b04611017/project/"+num+".out");

