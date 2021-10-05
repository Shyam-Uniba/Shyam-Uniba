#!/bin/bash

dir="/home/shyam"
outdir="/home/shyam"
if [[ ! -e $outdir/out_AllSi ]]; then
    mkdir $outdir/out_AllSi
fi

source $dir/Singularity/cvmfs/eic.opensciencegrid.org/default/opt/fun4all/core/bin/eic_setup.sh -n
export MYINSTALL=$dir/Singularity/install
source $dir/Singularity/cvmfs/eic.opensciencegrid.org/default/opt/fun4all/core/bin/setup_local.sh $MYINSTALL

cd $dir/Singularity/g4lblvtx/macros/auxiliary_studies/simplified_geometry/
root -b -l -q  Fun4All_G4_simplified_v2_new_shyam.C

rm -rf $outdir/out_AllSi/out_$3_det$4_$1_G4LBLVtx.root
