void Pulse(){
        gStyle->SetTitleSize(0.04, "t");
        gStyle->SetTitleSize(0.05, "XY");
        gStyle->SetTitleOffset(0.80, "XY");
        gStyle->SetLegendBorderSize(0);
        gStyle->SetPadTickX(1);
        gStyle->SetPadTickY(1);
        gStyle->SetOptStat(0.);

float tauR =50, tauF = 30;  // Rise and Fall exponential
float amax = 100;
float b= 0;

//float fr, ff, a;
//fr = 1.0 - TMath::Exp( - a / tauR);
//ff = TMath::Exp( - (a - amax) / tauF );

TCanvas * c = new TCanvas("c","coutput",1400,1000);
c->SetMargin(0.09, 0.07 ,0.1,0.05);

TCanvas * c1 = new TCanvas("c1","coutput",1400,1000);
c1->SetMargin(0.09, 0.07 ,0.1,0.05);

TF1* f = new TF1("f","100+(1-exp(-x/50))*(exp(-(x-100)/30))",0.,300.);
c->cd();
f->Draw();
TF1 *ret = new TF1("ret","0 + 300 * (1.0 - 1.0 / (exp((x-0.001)/1e-06)+1.0)) * (0.5 * exp(-(x-0.001)/0.001) + (1.0 - 0.5) * exp(-(x-0.001)/0.001))",0 ,0.01);
c1->cd();
ret->Draw();

}


/*double pulseFun(double t, double t0, double tr, double b, double a, double af, double tauf, double taus)
{
    double ret;
    ret = b + a * (1.0 - 1.0 / (exp(((t/80)-t0)/tr)+1.0)) * (af * exp(-(t-t0)/tauf) + (1.0 - af) * exp(-(t-t0)/taus));
    return ret;
}*/
