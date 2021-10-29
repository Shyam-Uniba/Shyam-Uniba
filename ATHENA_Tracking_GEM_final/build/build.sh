#!/bin/bash

dir="/home/shyam"
outdir="/home/shyam"
export MYINSTALL=$dir/Singularity/install
$dir/Singularity/g4lblvtx/source/autogen.sh --prefix=$MYINSTALL
make -j 16
make install
$dir/Singularity/ATHENA_Tracking_GEM/g4alsupp/source/autogen.sh --prefix=$MYINSTALL
make -j 16
make install
$dir/Singularity/ATHENA_Tracking_GEM/g4_modifieddRICH/source/autogen.sh --prefix=$MYINSTALL
make -j 16
make install
$dir/Singularity/ATHENA_Tracking_GEM/GEM_g4alsupp/source/autogen.sh --prefix=$MYINSTALL
make -j 16
make install
$dir/Singularity/ATHENA_Tracking_GEM/EIC_MMStripCZ/source/autogen.sh --prefix=$MYINSTALL
make -j 16
make install
$dir/Singularity/ATHENA_Tracking_GEM/EicToyModel/fun4all_with_eicroot/sandbox/autogen.sh --prefix=$MYINSTALL
make -j 16
make install
#cmake -DCMAKE_INSTALL_PREFIX=. -DGEANT=YES -Wno-dev -DVGM=/home/shyam/Singularity/cvmfs/eic.opensciencegrid.org/gcc-8.3/release/release_new/new.1/ ..
# Use above command to build EicToyModel

