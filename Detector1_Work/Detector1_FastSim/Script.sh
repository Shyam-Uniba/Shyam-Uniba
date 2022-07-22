#!bin/bash
root -b -l -q testDetectorUp.C'(-1.0,1.0)'
root -b -l -q PlotFastSimOutput.C'(-1.0,-0.5)'
root -b -l -q PlotFastSimOutput.C'(-0.5,0.0)'
root -b -l -q PlotFastSimOutput.C'(0.0,0.5)'
root -b -l -q PlotFastSimOutput.C'(0.5,1.0)'


