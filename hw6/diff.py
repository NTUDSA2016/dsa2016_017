import os

for i in range(1,49):
    num = "{0:02}".format(i)
    print ( num);
    os.system("make run --silent < demo/"+num+".in > test");
    fwant = open("demo/"+num+".out","r");
    fans  = open("test"           ,"r");

    digwant =  float(fwant.readline())
    digans  =  float(fans .readline())
    if abs ( digwant - digans)  > 0.05 :
        print ( "{0}:{1}\n".format(digwant,digans))
    strwant =  list(map(int,(fwant.readline()).split()))
    strans  =  list(map(int,(fans .readline()).split()))
    if strwant != strans:
        print ( "want:{0}\nans :{1}\n".format(strwant,strans))



