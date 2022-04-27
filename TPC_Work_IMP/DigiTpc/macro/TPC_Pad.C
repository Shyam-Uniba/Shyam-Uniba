// Macro: shyam kumar: shyam055119@gmail.com: 
// For Strips: for (double x = 0; x<50; x=x+5.0)
// Pad X dimension is controlled by binwidthx and y-dimension by binwidthy and z-dimension z binwidth
void TPC_Pad()
{
   auto c = new TCanvas("c","c",1500,1000);
   gStyle->SetOptStat(kFALSE);
   gStyle->SetTitleOffset(1.7,"XY");
   auto h3box = new TH3F("h3box","TPC (Pad Size = 1 x 1 cm^{2})",25, 0., 50., 50, 0.,50., 100, 0., 50.); // x = 2 cm, y = 1 cm ,and z = 0.5 cm
   int count=0;

for (double z=0; z<50; z=z+1.0){
for (double x = 0; x<50; x=x+2.0){
      count++;
      h3box->Fill(x+2.0/2,z+1.0/2, 49.9,  1.);
      
}
}
   h3box->SetFillColor(kGreen);
   h3box->GetXaxis()->SetTitle("X-direction [cm]");
   h3box->GetYaxis()->SetTitle("Z-direction [cm]");
   h3box->GetZaxis()->SetTitle("Y-direction [cm]");
   h3box->GetXaxis()->CenterTitle();
   h3box->GetYaxis()->CenterTitle();
   h3box->GetZaxis()->CenterTitle();
   h3box->Draw("box1");
   c->SaveAs("TPC.pdf");
}
