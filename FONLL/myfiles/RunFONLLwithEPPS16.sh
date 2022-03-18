#!/bin/bash

#       	 hvq       lha
#	cteq6m	 131	10050
#	AFG MC	  42	362
#	SMRS-P 2  32	232
#	cteq66	 ?	10550
#sed -i -e 's/\r$//' scriptname.sh (Run this command if script doesn't work:Shyam)

# Chose the desired PDF library by commenting/uncommenting
# "hvq" uses hvqpdfpho.f, "lha" uses the external LHAPDF library
#pdflib='hvq'
pdflib='lha'

FONLL=../`uname`/fonlllha
#
for Set in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45
do
let proton=10549+$Set
echo "processing set" $proton
for y in -4.5 -4. -3.5 -3. -2.5 -2. -1.5 -1. -0.5 0. 0.5 1. 1.5 2. 2.5 3. 3.5 4. 4.5
#for y in -8. -7.5 -7. -6.5 -6. -5.5 -5. 5. 5.5 6. 6.5 7. 7.5 8. 
do
let lead=-208001
echo "processing set" $lead
file=FONLLpPb5.2TeVwithEPPS16_y${y}_$Set
echo "file name is " $file
# remove old files, if present
\rm $file.log $file.dat $file.out
touch $file.log  $file.dat $file.out

# bottom at LHC (p-Pb at 5.02 TeV):

for pt in 0 0.1 0.2 0.3 0.4 0.5 0.6 0.8 1. 1.5 1.7 2. 3. 4. 5. 6. 7. 8. 10. 12. 14. 16. 18. 20.
# for pt in 5
do
$FONLL << EOD >> $file.log
tmp$$
 $lead 1580 0 0 $proton ! beam1: type, ener.,  nptype, ngroup, nset
 1 4000 0 0 $proton ! beam2: type, ener.,  nptype, ngroup, nset
  4.75    ! heavy quark mass
 -1    ! Lambda_5, <=0 for default
  1.  1. !  fact. and ren. scale factors
  $pt $y ! pt,y_lab
 1   ! icalctype
EOD

cat tmp$$.out >> $file.out
cat tmp$$.outlog >> $file.outlog
\rm tmp$$*
\rm hdms-log.tmp  hdrsout.tmp phms-log.tmp hdres.tmp phres.tmp
done
done
#
#\rm hdms-log.tmp  hdrsout.tmp phms-log.tmp hdres.tmp phres.tmp
#\rm *.tmp $outfile'.out' $outfile'fonll.log'
\rm cor* ctq* cteq* *MRS* PION* vnv* LAC* GR* ft* alf*
done
exit
