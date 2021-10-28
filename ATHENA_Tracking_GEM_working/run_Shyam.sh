#!/bin/bash

dir="/home/shyam"
outdir="/home/shyam"

source $dir/Singularity/cvmfs/sphenix.sdcc.bnl.gov/gcc-8.3/opt/sphenix/core/bin/sphenix_setup.sh -n
export MYINSTALL=$dir/Singularity/install
source $dir/Singularity/cvmfs/sphenix.sdcc.bnl.gov/gcc-8.3/opt/sphenix/core/bin/setup_local.sh $MYINSTALL
export LD_LIBRARY_PATH=/home/shyam/Singularity/ATHENA_Tracking_GEM/EicToyModel/build/lib:${OPT_SPHENIX}/vgm/lib64:${LD_LIBRARY_PATH}
source $dir/Singularity/cvmfs/sphenix.sdcc.bnl.gov/gcc-8.3/opt/sphenix/core/bin/setup_local.sh /home/shyam/Singularity/ATHENA_Tracking_GEM/EicToyModel/fun4all_with_eicroot
export ROOT_INCLUDE_PATH=/home/shyam/Singularity/ATHENA_Tracking_GEM/EicToyModel/build/include/etm:${ROOT_INCLUDE_PATH}
cd $dir/Singularity/ATHENA_Tracking_GEM
root -b -l  Fun4All_G4_Baseline2.C
