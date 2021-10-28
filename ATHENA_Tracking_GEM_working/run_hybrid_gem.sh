#!/bin/bash

dir="/home/shyam"
outdir="/home/shyam"

source $dir/Singularity/cvmfs/sphenix.sdcc.bnl.gov/gcc-8.3/opt/sphenix/core/bin/sphenix_setup.sh -n
export MYINSTALL=$dir/Singularity/install
source $dir/Singularity/cvmfs/sphenix.sdcc.bnl.gov/gcc-8.3/opt/sphenix/core/bin/setup_local.sh $MYINSTALL
cd $dir/Singularity/ATHENA_Tracking_GEM
root -b -l  Fun4All_G4_HybridGEM.C
