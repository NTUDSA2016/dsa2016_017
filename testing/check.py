import os
import subprocess
from multiprocessing import Process

from default_diff import defaultDiff

#testcasepath = "~/htdocs/project/"
testcasepath = "judge_test/"


def goTest(name):
    arr = [testcasepath+name]
    inname  = name + ".in"
    outname = name + ".out"
    
    run  = subprocess.Popen(
            '/usr/bin/time -f "%Us"  -a '+
            "python3 test.py < "+testcasepath+inname+" > "+outname,
            stdout = subprocess.PIPE,stderr = subprocess.PIPE,
            shell=True,universal_newlines=True)
    outs, errs = run.communicate()
    arr.append( "time = "+str(errs).strip() )
    
    result = defaultDiff(testcasepath+outname,outname)
    if(result):
        arr.append("WA")
        arr.append( result.strip() )
    else:
        arr.append("AC")
        subprocess.Popen("rm "+outname,shell=True);

    return arr

if __name__ == '__main__':
    testcasepath = os.path.expanduser(testcasepath)
    testcases = [ os.path.splitext(i)[0] for i in os.listdir(testcasepath) if 
                  os.path.splitext(i)[1]=='.in' ]
    arr = []
    for i in testcases :
        arr.append(goTest(i))
    #print ( list(map(goTest,testcases)) )
    arr = sorted(arr,key = lambda a:a[0] )
    print( arr )
