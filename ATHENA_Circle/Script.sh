#!bin/bash

for (( ii=1; ii<=100; ii++ ))
do
step=0.01
Result=$(echo "$step*$ii" | bc)
root -b -l -q fitCircle.C'(9,'$Result')'
done
convert -delay 200 -loop 0 *.png tracks_athena.gif
