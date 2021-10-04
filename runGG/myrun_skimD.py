#!/usr/bin/env python
import os, string
import sys

yearrun = raw_input("Year of run : ")

if int(yearrun) == 2016:
    remotepath = "summer16_data.txt"
    yeardir = "summer16"
elif int(yearrun) == 2017:
    remotepath = "fall17_data.txt"
    yeardir = "fall17"

with open(remotepath, "r") as filepath:
    path = filepath.read().splitlines()
    for line in path:
        alist = line.split("/")
        namelist = []
        namelist.append(alist[-2])
        dirname = "".join(namelist)#string without brackets

        script = ""
        script += "#!/bin/bash \n"
        script += "\n"
        script += "workdir=/home/jou/work/runGG \n"
        script += "homedir=/home/jou/runGG \n"
        script += "mkdir /data1/GMET/minitree/"+str(yeardir)+"/"+str(dirname)+" \n"
        script += "returndir=/data1/GMET/minitree/"+str(yeardir)+"/"+str(dirname)+" \n"
        script += "datadir="+str(line)+" \n"
        script += "year="+str(yearrun)+" \n"
        script += "\n"
        script += "cd $workdir \n"
        script += "mkdir "+str(dirname)+" \n"
        script += "cd "+str(dirname)+" \n"
        script += "cp -r $homedir/* . \n"
        script += "\n"
        script += "realpath $datadir/*.root > path.txt \n"
        script += "count=$(ls $datadir | wc -l) \n"
        script += """echo "There are tot $count ggtrees!!!" \n"""
        script += "\n"
        script += """/usr/bin/time parallel --progress --jobs 5 'root -l -b -q "xSkim_data.C(\\\"{1}\\\", {2})"' :::: path.txt ::: $year \n"""
        script += "echo Processed all $count ggtrees!!! \n"
        script += "cp mini_ggtree_data* $returndir \n"
        script += "thiswork=$(pwd) \n"
        script += "cd $workdir; rm -r $thiswork \n"

        frun = open("run"+str(yeardir)+"_skimD.sh", "w")
        frun.write(script)
        frun.close()

        os.system( "chmod 744 run"+str(yeardir)+"_skimD.sh" )
        os.system( "./run"+str(yeardir)+"_skimD.sh" )
