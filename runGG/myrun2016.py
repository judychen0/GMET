#!/usr/bin/env python
import os, string
import sys

#remotepath = raw_input("Data path : ")
#if len(sys.argv) <= 1:
#    print("1 argument required!")
#    remotepath = raw_input("Enter data path : ")
#else:
#    remotepath = sys.argv[1]
#storepath = raw_input("Output path : ")
remotepath = raw_input("Data path(enter the mc/data path list filename HERE) : ")
with open(remotepath, "r") as pathfile:
    path = pathfile.read().splitlines()
    for line in path:
        alist = line.split("/")
        namelist = []
        namelist.append("summer16_"+alist[-5]+"_"+alist[-2])
        dirname = "".join(namelist)
        
        script = ""
        script += "#!/bin/bash \n"
        script += "\n"
        script += "workdir=/afs/cern.ch/work/j/jou/runGG \n"
        script += "homedir=/afs/cern.ch/user/j/jou/runGG \n"
        script += "mkdir /afs/cern.ch/user/j/jou/ana/summer16/"+str(dirname)+" \n"
        script += "returndir=/afs/cern.ch/user/j/jou/ana/summer16/"+str(dirname)+" \n"
        script += "datadir="+str(line)+" \n"
        script += "\n"
    
        script += "cd $workdir \n"
        script += "tmp=$(date +%Y%m%d_%H%M%S) \n"
        script += "mkdir -p $tmp \n"
        script += "cd $tmp \n"
        script += "count=$(ls $datadir | wc -l) \n"
        script += """echo "There are tot $count ggtrees!!!" \n"""
        script += "cp -r $homedir/* . \n"
        script += "mkdir -p $returndir/$tmp \n"
        script += "ls $datadir | grep root > path.txt \n"
        script += """for filename in $(cat path.txt); do outfile=output_$filename; fullpath=$datadir/$filename; sleep 5; root -b -q "xZg.C(\\\"$fullpath\\\")"; cp output_ggtree_mc.root $outfile; rm output_ggtree_mc.root; done; \n"""
        script += """echo "Processed all $count ggtrees!!!" \n"""
        script += "for i in {1..9}; do hadd output_ggtree_$i.root *mc_$i*.root; done \n"
        script += "rm -r *mc* \n"
        script += "hadd tmp1.root *{1..4}.root; hadd tmp2.root *{5..9}.root; rm -r output*; hadd output_ggtree.root tmp1.root tmp2.root \n"
        script += "rm *.C *.h *.py *.sh *.txt tmp* \n"
        script += "cp output_ggtree.root $returndir/$tmp \n"
        #script += "cp $homedir/Draw/* . \n"
        #script += "mkdir graph; root -b -q xDraw.C; root -b -q xDrawIso.C; root -b -q xDraw_phoIDeff.C; rm *Draw*; \n"
        #script += "cp -r graph $returndir/$tmp \n"
        #script += "cp *.txt $returndir/$tmp \n"
        
        script += "cleandir=$(pwd) \n"
        script += "cd $workdir; rm -r $cleandir \n"
        
        frun = open("run2016.sh", "w")
        frun.write(script)
        frun.close()
        
        os.system( "chmod 744 run2016.sh " )
        os.system( "./run2016.sh")


