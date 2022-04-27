void Plot_TPC_Pad_Plane()
{
         gStyle->SetPadTickX(1);
         gStyle->SetPadTickY(1);
         gStyle->SetOptStat(0.);
         gStyle->SetLineColor(kRed);
         gStyle->SetLineWidth(2.0);
// Rectangular structure using Polyline
   TCanvas* recpads=new TCanvas("recpads","recpads",1400,1000);
   recpads->SetMargin(0.09, 0.03 ,0.1,0.1);
   TH2Poly *h2p = new TH2Poly();
   h2p->SetName("RectangularPads");
   h2p->SetTitle("CEE TPC Pad: Rectangular Pads (5 #times 12 mm^{2})");
   Int_t i,j;
   // TPC new design has the dimension of 100X90 cm^2
   Int_t nx = 240;
   Int_t nz = 75;

   Double_t xval1,zval1,xval2,zval2;
   Double_t dx=0.5, dz=1.2;
   xval1 = -60.;   xval2 = dx;  

   for (i = 0; i<nx; i++) {  
  
     zval1 = -45.; zval2 = zval1 + dz; 
      
   for (j = 0; j<nz; j++) {
         h2p->AddBin(xval1, zval1, xval2, zval2);
         zval1+=dz;
         zval2=zval1+dz;
      }
         xval1+=dx;
         xval2=xval1+dx; 
   }
    h2p->GetXaxis()->SetTitle("X [cm]");
    h2p->GetYaxis()->SetTitle("Z [cm]");
    h2p->GetXaxis()->CenterTitle();
    h2p->GetYaxis()->CenterTitle();
    Int_t nbins = h2p->GetNumberOfBins();
    recpads->cd();
    h2p->Draw();
    recpads->SaveAs("Cee_TPC_Pad_June20.pdf");
}
