def diff(correct_name,ans_name):
    correct_f,ans_f = open(correct_name,"r"),open(ans_name,"r")
    line = 0
    while True:
        line = line + 1
        correct = correct_f.readline()
        ans     =     ans_f.readline()
        if(  correct =='' and ans==''):
            break;
        elif(correct =='' or  ans==''):
            if(ans==''):return "ans not enough"
            else:return "ans too more line"
        elif (correct!=ans):
            return "line = "+str(line)+"\ncorrect= "+correct + "yourans= "+ans

