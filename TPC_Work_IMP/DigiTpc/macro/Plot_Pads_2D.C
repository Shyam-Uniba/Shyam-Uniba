void Plot_Pads_2D(){
gStyle->SetOptStat(0);
TH2F *h[100];
TCanvas * c1 = new TCanvas("c1","coutput",1500,900);
c1->SetMargin(0.09, 0.03 ,0.1,0.07);
int i = 0;
for (int x=0; x<10; x++){
for (int y=0; y<10; y++){
h[i] = new TH2F(Form("h_%d",i),"TPC 2D-Pads",10,0.,10.,10,0.,10.);	
h[i]->Fill(x+1/2,y+1/2);
h[i]->SetFillColor(kGreen);
c1->cd();
if (i==0) h[i]->Draw("Box1");
else h[i]->Draw("Box1-same");
i++;
}
}
h[0]->GetXaxis()->SetTitle("X-direction [cm]");
h[0]->GetYaxis()->SetTitle("Y-direction [cm]");
h[0]->GetXaxis()->CenterTitle();
h[0]->GetYaxis()->CenterTitle();
}
