void Signal(){
        gStyle->SetTitleSize(0.04, "t");
        gStyle->SetTitleSize(0.05, "XY");
        gStyle->SetTitleOffset(0.80, "XY");
        gStyle->SetLegendBorderSize(0);
        gStyle->SetPadTickX(1);
        gStyle->SetPadTickY(1);
        //gStyle->SetOptStat(0.);

TCanvas * c1 = new TCanvas("c1","coutput",1400,1000);
c1->SetMargin(0.09, 0.07 ,0.1,0.05);
TFile *f = TFile::Open("function.root");
TCanvas *c = (TCanvas*)f->Get("c1_n2");
TProfile *prof = (TProfile*)c->FindObject("profile");
TH1F *h = (TH1F*)prof->ProjectionX("hsignal");
for(int i=0 ; i<h->GetNbinsX() ; ++i) h->SetBinContent(i,h->GetBinContent(i)-90);
h->SetMinimum(-20);
h->GetXaxis()->SetTitle("TimeBin");
h->GetYaxis()->SetTitle("ADC");
TFile *fout = new TFile("signal.root","recreate");
fout->cd();
h->Write();
fout->Close();
c1->cd();
h->Draw();
h->Reset("");
}
