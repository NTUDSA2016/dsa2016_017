#!/usr/bin/python3
# -*- coding: utf-8 -*-

import requests
from bs4 import BeautifulSoup
import re

def get_station_data() :
    url = "https://mrtg.csie.ntu.edu.tw/status.html"

    mypage = requests.get(url)
    mypage.encoding = 'utf8'
    soup = BeautifulSoup(mypage.text,"html.parser")

    #table =soup.select("#tableData")  this return type is not bs
    table = soup.find("table")
    #print (table)



    #this is for head
    headarr = []
    cols = table.find_all("th")
    headarr = [re.search("[\w ]+",col.get_text()).group(0).strip() for col in cols]
	
    #this is for data
    arr     = []
    rows = table.find_all("tr")
    for row in rows:
        cols = row.find_all("td")
        arr.append([col.get_text() for col in cols ])
    arr = list(filter(None,arr))

    return headarr,arr

def data_rank():
    headarr,arr = get_station_data()
    #print( headarr)
    #print( arr)

    #delete some not want to use
    head = headarr.index("Host")
    arr[:] = [name for name in arr if "linux" in name[head] and ( int(re.search("\d+",name[head]).group(0)) not in [15])]
    #print(arr)

    #delete too high
    cpu = headarr.index("CPU")
    swap = headarr.index("Swap")
    arr[:] = [name for name in arr if float(name[cpu])<90 and float(name[swap])<90 ]

    #sorted
    arr.sort(key= lambda name : name[cpu] + name[swap])

    #print( arr,sep)
    #just output the smallest one
    print(arr[0][head])

data_rank()
