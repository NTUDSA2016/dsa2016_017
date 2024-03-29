import os
import subprocess
import sys
from multiprocessing import Process,Queue

testcasepath = "judge_test/"
diffpath = "../default_diff.py"
how_to_run = " make run --silent "

def goTest(name,que):
    arr = [testcasepath+name]
    inname  = name + ".in"
    outname = name + ".out"
    
    run  = subprocess.Popen(
            '/usr/bin/time -f "%Us"  -a '+
            how_to_run +" < "+testcasepath+inname+" > "+outname,
            stdout = subprocess.PIPE,stderr = subprocess.PIPE,
            shell=True,universal_newlines=True)
    outs, errs = run.communicate()
    arr.append( "time = "+str(errs).strip() )
    
    result = diff(testcasepath+outname,outname)
    if(result):
        arr.append("WA")
        arr.append( result.strip() )
    else:
        arr.append("AC")
        subprocess.Popen("rm "+outname,shell=True);

    que.put(arr)
    #return arr

if __name__ == '__main__':
    if( len(sys.argv) < 2):  # argv = self,testcasepath,diff program,how to run
        print( "no test cases")
        sys.exit()
    testcasepath =  sys.argv[1]
    if( len(sys.argv) > 2):
        diffpath =  sys.argv[2]
    if( len(sys.argv) > 3):# use % to separate
        how_to_run= sys.argv[3].replace('%',' ')
    testcasepath = os.path.expanduser(testcasepath)
    diffpath     = os.path.expanduser(diffpath    )
    exec( open(diffpath).read() )
    diffpath     = os.path.expanduser(diffpath    )
    testcases = [ os.path.splitext(i)[0] for i in os.listdir(testcasepath) if 
                  os.path.splitext(i)[1]=='.in' ]

    que = Queue()
    proc= []
    for i in testcases :
        proc.append(Process(target=goTest,args=(i,que)))
    for i in proc:
        i.start()
    for i in proc:
        i.join()

    arr = []
    for i in range(len(proc)):
        arr.append(que.get())
    arr = sorted(arr,key = lambda a:a[0] )
    for i in arr:
        print( *i )
