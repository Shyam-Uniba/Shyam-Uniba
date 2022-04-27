void gain(){

        gStyle->SetTitleSize(0.04, "t");
        gStyle->SetTitleSize(0.05, "XY");
        gStyle->SetTitleOffset(0.80, "XY");
        gStyle->SetLegendBorderSize(0);
        gStyle->SetPadTickX(1);
        gStyle->SetPadTickY(1);
        gStyle->SetOptStat(0.);

TCanvas * c = new TCanvas("c","coutput",1400,1000);
c->SetMargin(0.09, 0.07 ,0.1,0.05);

TH1F *hrgain = new TH1F("hrgain","Random Gain",5000,0.,5000.);

TFile *f = TFile::Open("gain.root");	
TH1F *hgain = (TH1F*) f->Get("gain1");

hgain->GetXaxis()->CenterTitle();
hgain->GetYaxis()->CenterTitle();
hgain->GetXaxis()->SetRangeUser(0.,5000.);


for (int num=0; num<10000; num++){
int random = hgain->GetRandom();
hrgain->Fill(random);
}
c->cd();
hrgain->Draw();
}
