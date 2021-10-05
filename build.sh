#!/bin/bash

dir="/home/shyam"
outdir="/home/shyam"
export MYINSTALL=$dir/Singularity/install
$dir/Singularity/g4lblvtx/source/autogen.sh --prefix=$MYINSTALL
make -j 16
make install

