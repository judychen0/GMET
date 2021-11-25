#!/usr/bin/env python
import os, string
import sys

yearrun = raw_input("Year of run : ")
isDcount = raw_input("Is Double counting? (T==1; F==0) : ")
isSignal = raw_input("Is Signal sample? (T==1; F==0) : ")

if int(yearrun) == 2016:
    if int(isDcount) == 1: remotepath = "summer16_VJet.txt"
    if int(isSignal) == 1: remotepath = "summer16_mc.txt"
    if int(isDcount) == 0 and int(isSignal) == 0: remotepath = "summer16_bkg.txt"
    yeardir = "summer16"
    pupath = "PileupWeight/summer16/PU_histo_13TeV_2016_GoldenJSON_69200nb.root"
    IDSFpath = "2016_PhotonsMedium.root"
    PSVpath = "PixelSeed_ScaleFactors_2016.root"
elif int(yearrun) == 2017:
    if int(isDcount) == 1: remotepath = "fall17_VJet.txt"
    if int(isSignal) == 1: remotepath = "fall17_mc.txt"
    if int(isDcount) == 0 and int(isSignal) == 0: remotepath = "fall17_bkg.txt"    
    yeardir = "fall17"
    pupath = "PileupWeight/fall17_corrected/PU_correctedPU_13TeV_2017_GoldenJSON_69200nb.root"
    IDSFpath = "2017_PhotonsMedium.root"
    PSVpath = "PixelSeed_ScaleFactors_2017.root"
elif int(yearrun) == 2018:
    if int(isDcount) == 1: remotepath = "autumn18_VJet.txt"
    if int(isSignal) == 1: remotepath = "autumn18_mc.txt"
    if int(isDcount) == 0 and int(isSignal) == 0: remotepath = "autumn18_bkg.txt"    
    yeardir = "autumn18"
    pupath = "PileupWeight/autumn18/PU_histo_13TeV_2018_GoldenJSON_69200nb.root"
    IDSFpath = "2018_PhotonsMedium.root"
    PSVpath = "PixelSeed_ScaleFactors_2018.root"

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
        script += "workdir=/home/jou/work/runMC \n"
        script += "homedir=/home/jou/runMC \n"
        script += "mkdir /data1/GMET/minitree/"+str(yeardir)+"/"+str(dirname)+" \n"
        script += "returndir=/data1/GMET/minitree/"+str(yeardir)+"/"+str(dirname)+" \n"
        script += "datadir="+str(line)+" \n"
        script += "PUpath="+str(pupath)+" \n"
        script += "IDSFpath="+str(IDSFpath)+" \n"
        script += "PSVpath="+str(PSVpath)+" \n"
        if int(isDcount) == 1: script += "isdcount="+str(isDcount)+" \n"
        else: script += "isdcount=0 \n"
        if int(isSignal) == 1: script += "isSig="+str(isSignal)+" \n"
        else: script += "isSig=0 \n"
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
        script += """/usr/bin/time parallel --progress --jobs 2 'root -l -b -q "xSkim_mc.C(\\\"{1}\\\", \\\"{2}\\\", \\\"{3}\\\", \\\"{4}\\\", {5}, {6}, {7})"' :::: path.txt ::: $PUpath ::: $IDSFpath ::: $PSVpath ::: $isdcount ::: $year ::: $isSig \n"""
        script += "echo Processed all $count ggtrees!!! \n"
        script += "cp mini_ggtree_mc* $returndir \n"
        script += "thiswork=$(pwd) \n"
        script += "cd $workdir; rm -r $thiswork \n"

        frun = open("run"+str(yeardir)+"_skim_"+str(isDcount)+"_"+str(isSignal)+".sh", "w")
        frun.write(script)
        frun.close()

        os.system( "chmod 744 run"+str(yeardir)+"_skim_"+str(isDcount)+"_"+str(isSignal)+".sh" )
        os.system( "./run"+str(yeardir)+"_skim_"+str(isDcount)+"_"+str(isSignal)+".sh" )
