// Creating a Rectangular Pad
// Shyam Kumar
void PlotRect_Pad(){
double a=0.2; // 
double b=0.1;
gStyle->SetOptStat(0);
TPolyLine* line;
for (int j=0; j<1; j++){
line=new TPolyLine(5);
for(int i=0; i<5; ++i){
  double al=(double)(i+1)*TMath::Pi()/2.;
  double x=a*(TMath::Cos(al)-TMath::Sin(al));
  double y=b*(TMath::Cos(al)+TMath::Sin(al));
  if(fabs(x)<1E-4)x=0;
  if(fabs(y)<1E-4)y=0;
  line->SetPoint(i,x,y);
}
a+=0.1;
}

TCanvas* cpolyline=new TCanvas("cpolyline","cpolyline",1000,800);
TH2D* h=new TH2D("h","Hexagon",1,-0.3,0.3,1,-0.3,0.3);
h->GetXaxis()->SetTitle("X-direction");
h->GetYaxis()->SetTitle("Z-direction");
cpolyline->cd();
line->SetFillColor(38);
h->Draw();
line->Draw("f");
line->Draw();
}
