#!bin/bash

for (( ii=1; ii<2; ii++ ))
do
step=0.0181
Result=$(echo "$step*$ii" | bc)
root -b -l -q fitCircle_myvideo.C'(6,'$Result')'
done
convert -delay 200 -loop 0 *.png tracks_athena.gif
