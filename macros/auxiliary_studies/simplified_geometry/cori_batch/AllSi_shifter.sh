#!/bin/bash

dir="/global/homes/r/reynier"
outdir="/project/projectdirs/alice/reynier"
if [[ ! -e $outdir/out_AllSi/Simplified_geom/$1 ]]; then
    mkdir $outdir/out_AllSi/Simplified_geom/$1
fi

source $dir/Singularity/cvmfs/sphenix.sdcc.bnl.gov/x8664_sl7/opt/sphenix/core/bin/sphenix_setup.sh -n
export MYINSTALL=$dir/Singularity/install
source $dir/Singularity/cvmfs/sphenix.sdcc.bnl.gov/x8664_sl7/opt/sphenix/core/bin/setup_local.sh $MYINSTALL

cd $dir/Singularity/g4lblvtx/macros/auxiliary_studies/simplified_geometry/
root -b -q "Fun4All_G4_simplified_v2.C($3,$4,$5,$6,$7,$8,\"$outdir/out_AllSi/Simplified_geom/$1/out_simp_geom_split_$2\")"

mkdir $outdir/out_AllSi/Simplified_geom/$1/logs/
mv /global/homes/r/reynier/Singularity/g4lblvtx/macros/auxiliary_studies/simplified_geometry/cori_batch/slurm-$1_$2.out $outdir/out_AllSi/Simplified_geom/$1/logs/
