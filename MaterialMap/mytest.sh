#!/bin/bash
echo "The Script is working..."
filename="mat_scan.log"
cat $filename | awk 'NR>422' > file1.txt
sed -e 's/         Theta(deg)    Phi(deg)  Length(mm)          x0     lambda0//g' file1.txt > file2.txt
sed -i '/^$/d' file2.txt
awk '!/ ave. for theta/' file2.txt > file3.txt
head -n -2 file3.txt > finalmaterial_map.txt


