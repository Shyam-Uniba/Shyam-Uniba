// Macro to create different pad geometries
// CEE detector TPC has rectangular pads
void Honey_comb();
void Rectangle();
void Hexagon();
void Rect();

void Pad(){
gStyle->SetOptStat(0.);
gStyle->SetLineColor(kRed);
gStyle->SetLineWidth(2.0);

Honey_comb();
Rectangle();
Hexagon();
Rect();

}
// Creating a Hexagon
void Hexagon(){
double a=0.25; // Hexagon radius
double l=0.01; // margin around pad
a-=l;

TPolyLine* line=new TPolyLine(7);
for(int i=0; i<7; ++i){
  double al=(double)i*TMath::Pi()/3.;
  double x=a*TMath::Cos(al);
  double y=a*TMath::Sin(al);
  if(fabs(x)<1E-4)x=0;
  if(fabs(y)<1E-4)y=0;

  std::cout<<x<<" "<<y<<" "<<endl;
  line->SetPoint(i,x,y);
}
std::cout.flush();

TCanvas* cpolyline=new TCanvas("cpolyline","cpolyline",1000,800);
TH2D* h=new TH2D("h","Hexagon",1,-0.3,0.3,1,-0.3,0.3);
h->GetXaxis()->SetTitle("X-direction");
h->GetYaxis()->SetTitle("Y-direction");
cpolyline->cd();
line->SetFillColor(38);
h->Draw();
line->Draw("f");
line->Draw();
}

 // Honey comb structure using Polyline
void Honey_comb(){
  TCanvas* honeycomb=new TCanvas("honeycomb","honeycomb",1000,800);
   TH2Poly *hc = new TH2Poly();
   hc->SetName("HoneyComb");
   hc->SetTitle("Honeycomb Structure");
   hc->GetXaxis()->SetTitle("X-direction");
   hc->GetYaxis()->SetTitle("Y-direction");
   hc->Honeycomb(0,0,.1,5,5);
   honeycomb->cd();
   hc->Draw();
}

void Rectangle(){
         gStyle->SetPadTickX(1);
         gStyle->SetPadTickY(1);
// Rectangular structure using Polyline
   TCanvas* recpads=new TCanvas("recpads","recpads",1000,800);
   recpads->SetMargin(0.09, 0.03 ,0.1,0.1);
   TH2Poly *h2p = new TH2Poly();
   h2p->SetName("RectangularPads");
   h2p->SetTitle("CEE TPC Pad: Rectangular Pads (5 #times 12 mm^{2})");
   Int_t i,j;
   // TPC new design has the dimension of 100X90 cm^2
   Int_t nx = 200;
   Int_t nz = 75;
   Double_t xval1,zval1,xval2,zval2;
   Double_t dx=0.5, dz=1.2;
   xval1 = 0.;  
   xval2 = dx;  
   for (i = 0; i<nx; i++) {
      zval1 = 0.;
      zval2 = dz;
      
      for (j = 0; j<nz; j++) {
         h2p->AddBin(xval1, zval1, xval2, zval2);
         zval1+=1.2;
         zval2=zval1+1.2;
      }
         xval1+=0.5;
         xval2=xval1+0.5; 
   }
    h2p->GetXaxis()->SetTitle("X-direction");
    h2p->GetYaxis()->SetTitle("Z-direction");
    Int_t nbins = h2p->GetNumberOfBins();
    recpads->cd();
    h2p->Draw();
}

// Creating a Rectangle
void Rect(){
double a=0.25; // Hexagon radius

TPolyLine* line=new TPolyLine(5);
for(int i=0; i<5; ++i){
  double al=(double)(i+1)*TMath::Pi()/2.;
  double x=a*TMath::SignBit(TMath::Cos(al));
  double y=a*TMath::SignBit(TMath::Sin(al));
  if(fabs(x)<1E-4)x=0;
  if(fabs(y)<1E-4)y=0;
  std::cout<<x<<" "<<y<<" "<<endl;
  line->SetPoint(i,x,y);
}
std::cout.flush();

TCanvas* cpolyline=new TCanvas("cpolyline","cpolyline",1000,800);
TH2D* h=new TH2D("h","Rectangle",1,-0.3,0.3,1,-0.3,0.3);
h->GetXaxis()->SetTitle("X-direction");
h->GetYaxis()->SetTitle("Y-direction");
cpolyline->cd();
line->SetFillColor(38);
h->Draw();
line->Draw("f");
line->Draw();
}


