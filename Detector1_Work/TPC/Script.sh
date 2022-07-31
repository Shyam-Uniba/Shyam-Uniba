#!bin/bash
root -b -l -q docompare.C'(0.0,0.5,"P_Resol")'
root -b -l -q docompare.C'(0.5,1.0,"P_Resol")'
root -b -l -q docompare.C'(-1.0,-0.5,"P_Resol")'
root -b -l -q docompare.C'(-0.5,0.0,"P_Resol")'

root -b -l -q docompare.C'(0.0,0.5,"Pt_Resol")'
root -b -l -q docompare.C'(0.5,1.0,"Pt_Resol")'
root -b -l -q docompare.C'(-1.0,-0.5,"Pt_Resol")'
root -b -l -q docompare.C'(-0.5,0.0,"Pt_Resol")'

root -b -l -q docompare.C'(0.0,0.5,"PR_ResolXY")'
root -b -l -q docompare.C'(0.5,1.0,"PR_ResolXY")'
root -b -l -q docompare.C'(-1.0,-0.5,"PR_ResolXY")'
root -b -l -q docompare.C'(-0.5,0.0,"PR_ResolXY")'

root -b -l -q docompare.C'(0.0,0.5,"PR_ResolZ")'
root -b -l -q docompare.C'(0.5,1.0,"PR_ResolZ")'
root -b -l -q docompare.C'(-1.0,-0.5,"PR_ResolZ")'
root -b -l -q docompare.C'(-0.5,0.0,"PR_ResolZ")'

root -b -l -q docompare.C'(0.0,0.5,"PR_ResolXY_Mom")'
root -b -l -q docompare.C'(0.5,1.0,"PR_ResolXY_Mom")'
root -b -l -q docompare.C'(-1.0,-0.5,"PR_ResolXY_Mom")'
root -b -l -q docompare.C'(-0.5,0.0,"PR_ResolXY_Mom")'

