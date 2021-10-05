#!/bin/bash

dir="/home/shyam"
outdir="/home/shyam"

source $dir/Singularity/cvmfs/eic.opensciencegrid.org/default/opt/fun4all/core/bin/eic_setup.sh -n
export MYINSTALL=$dir/Singularity/install
source $dir/Singularity/cvmfs/eic.opensciencegrid.org/default/opt/fun4all/core/bin/setup_local.sh $MYINSTALL

cd $dir/Singularity/g4lblvtx/macros/auxiliary_studies/simplified_geometry/
root -b -l -q Fun4All_G4_simplified_Pythia.C


