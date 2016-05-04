import os
for i in range(1,11):
    os.system("make run -s < testdata4/in"+str(i)+" > test")
    os.system("diff  testdata4/out"+str(i)+"  test")
