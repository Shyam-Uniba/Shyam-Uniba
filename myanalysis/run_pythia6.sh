#!/bin/bash

dir="/home/shyam"
outdir="/home/shyam"
if [[ ! -e $outdir/out_AllSi ]]; then
    mkdir $outdir/out_AllSi
fi

source $dir/Singularity/cvmfs/sphenix.sdcc.bnl.gov/gcc-8.3/opt/sphenix/core/bin/sphenix_setup.sh -n
export MYINSTALL=$dir/Singularity/install
source $dir/Singularity/cvmfs/sphenix.sdcc.bnl.gov/gcc-8.3/opt/sphenix/core/bin/setup_local.sh $MYINSTALL

cd $dir/Singularity/myanalysis/
root -b -l -q Fun4All_G4_simplified_pythia6.C


