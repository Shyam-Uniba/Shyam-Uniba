#!/bin/bash

#       	 hvq       lha
#	cteq6m	 131	10050
#	AFG MC	  42	362
#	SMRS-P 2  32	232


# Chose the desired PDF library by commenting/uncommenting
# "hvq" uses hvqpdfpho.f, "lha" uses the external LHAPDF library
#pdflib='hvq'
pdflib='lha'
if [[ $pdflib == 'hvq' ]]; then
  proton=131
  photon=42
  pion=32
  outfile=testfonll2-hvq
  FONLL=../`uname`/fonll
  ../pdfdata/linkpdf.sh
else
  proton=10042 # Pdf index of beam1 it will search cteq6l1_0000.dat; if you put index 10043 it will search cteq6l1_0001.dat, and so on.
  photon=252
  pion=232 # not used in it
  outfile=testfonll2-lha
  FONLL=../`uname`/fonlllha
fi

\rm $outfile.outlog
echo $FONLL


# electroproduction at HERA

# for ffact in 0.5 1 2
for ffact in 2
do
# for fren in 0.5 1 2
for fren in 0.5
do
# for pt in 0.5 5 20
for pt in 20
do
# for y in -1 0 1
for y in 1
do
$FONLL <<xxx
$outfile
 1  920. 0 0 $proton ! beam1: type, ener.,  nptype, ngroup, nset
 1  920. 0 0 $proton ! beam2: type, ener.,  nptype, ngroup, nset
  1.5 ! heavy quark mass
 -1    ! Lambda_5, <=0 for default
  $ffact $fren !  fact. and ren. scale factors
  $pt $y ! pt,y_lab
 1   ! icalctype
 1    ! itype ww (1-4)
  1.   ! effective ww scale
  0.2  0.8 ! zminww,zmaxww
xxx
done
done
done
done

\rm hdms-log.tmp  hdrsout.tmp phms-log.tmp hdres.tmp phres.tmp
\rm *.tmp  $outfile'fonll.log'
\rm cor* ctq* cteq* *MRS* PION* vnv* LAC* GR* ft* alf*

exit
