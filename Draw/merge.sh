#!/bin/bash

drawdir=/home/jou/Draw/
mcdir=/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211115/fall17/mc/

cp $drawdir/xMerge_GJet.C .; root -l -q "xMerge_GJet.C(2017)"
cp output_merge_GJet.root $mcdir/GJet
cp $drawdir/xMerge_WJet.C .; root -l -q "xMerge_WJet.C(2017)"
cp output_merge_WJet.root $mcdir/WJet
cp $drawdir/xMerge_ZJet.C .; root -l -q "xMerge_ZJet.C(2017)"
cp output_merge_ZJet.root $mcdir/ZJet
cp $drawdir/xMerge_DYJet.C .; root -l -q "xMerge_DYJet.C(2017)"
cp output_merge_DYJet.root $mcdir/DYJet
cp $drawdir/xMerge_monoboson.C .; root -l -q "xMerge_monoboson.C(2017)"
cp output_merge_monoBoson.root $mcdir/monoboson
cp $drawdir/xMerge_diboson.C .; root -l -q "xMerge_diboson.C(2017)"
cp output_merge_diBoson.root $mcdir/diboson
cp $drawdir/xMerge_triboson.C .; root -l -q "xMerge_triboson.C(2017)"
cp output_merge_triBoson.root $mcdir/triboson
