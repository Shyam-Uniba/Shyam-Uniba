#include "TGraph.h"
#include "TFitResult.h"
#include "TMatrixD.h"

int Simple_Covariance()
{
Double_t x[] = {1, 2, 3, 4, 5};
Double_t y[] = {1.1, 1.9, 3.2, 3.9, 5.5};
TGraph *g = new TGraph((sizeof(x) / sizeof(Double_t)), x, y);
g->Draw("A");
TFitResultPtr r = g->Fit("pol2", "S");
TMatrixD cor = r->GetCorrelationMatrix();
TMatrixD cov = r->GetCovarianceMatrix();
cor.Print();
cov.Print(); // diagonal 0.508836*0.508836 = 0.25891407, 0.387767*0.387767 = 0.15036325, 0.0634067*0.0634067 = 0.0040204096
return 0;
}
