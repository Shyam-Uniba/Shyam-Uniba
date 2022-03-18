#!/bin/bash
PDFDIR=`dirname $0`

function stripextension {
echo $1 | sed "s/\..*\$//"
}

function capitalize {
echo $1 | sed "y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/"
}

for i in ${PDFDIR}/*.dat ${PDFDIR}/*.tbl
do
name=`basename $i`
name=`stripextension $name`
case $name in
*mrs*|grpol|grvdm|lac1|pion[1-3]) name=`capitalize $name`;;
esac
ln -sf $i $name
done
