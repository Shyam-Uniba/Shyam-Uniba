 /// \file
 /// \ingroup tutorial_fit
 /// \notebook
 /// Generate points distributed with some errors around a circle
 /// Fit a circle through the points and draw
 /// To run the script, do, eg
 ///
 /// ~~~{.cpp}
 ///   root > .x fitCircle.C   (10000 points by default)
 ///   root > .x fitCircle.C(100);  (with only 100 points
 ///   root > .x fitCircle.C(100000);  with ACLIC
 /// ~~~
 ///
 /// \macro_image
 /// \macro_output
 /// \macro_code
 ///

 /* std::cout << "Intersection point(s)\n";
  if(i_points == 2)
  {
   std::cout << i1 << "\n";
   std::cout << i2 << "\n";
 }
  std::cout<<std::endl;*/
 /// \author Rene Brun
 
 #include "TCanvas.h"
 #include "TRandom3.h"
 #include "TGraph.h"
 #include "TMath.h"
 #include "TArc.h"
 #include "Fit/Fitter.h"
 #include "Circle_Intersection.h"
 //____________________________________________________________________
 void fitCircle(Int_t n=9, Double_t pT = 1.0) { // pT = 1 GeV/c
    //generates n points around a circle and fit them
    TCanvas *c1 = new TCanvas("c1","c1",1200,1000);
    c1->SetGrid();
    TH1 *frame = c1->DrawFrame(-80,-80,80,80);
    frame->GetXaxis()->SetTitle("X [cm]");
    frame->GetYaxis()->SetTitle("Y [cm]");   
    frame->GetXaxis()->CenterTitle();
    frame->GetYaxis()->CenterTitle();  
    c1->Update();

    TGraph *gr = (TGraph*) c1->GetFrame();
    TRandom3 r;
    Double_t x,y;
    // Circle will pass through the origin a^2+b^2 = R^2
    Double_t Radius = (pT/0.9)*100; // cm with B= 3 Tesla 

    float a = sqrt(Radius*Radius/2); // start with a = b

   for (int icircle =0; icircle<1; icircle++){ // put the number of tracks
    
    Double_t b = sqrt(Radius*Radius-a*a); 
   
    gr = new TGraph(n);
    if (n> 999) gr->SetMarkerStyle(47);
    else gr->SetMarkerStyle(47);
    gr->SetMarkerSize(0.0);
    gr->SetMarkerColor(kMagenta);
    gr->GetXaxis()->SetTitle("X (cm)");
    gr->GetYaxis()->SetTitle("Y (cm)");
    gr->GetXaxis()->CenterTitle();
    gr->GetYaxis()->CenterTitle();

    for (Int_t i=0;i<n;i++) {
       r.Circle(x,y,Radius); 
       gr->SetPoint(i,x+a,y+b);
    }
   
    gr->Draw("p-same");
    //gPad->Update();
 
    auto chi2Function = [&](const Double_t *par) {
       //minimisation function computing the sum of squares of residuals
       // looping at the graph points
       Int_t np = gr->GetN();
       Double_t f = 0;
       Double_t *x = gr->GetX();
       Double_t *y = gr->GetY();
       for (Int_t i=0;i<np;i++) {
          Double_t u = x[i] - par[0];
          Double_t v = y[i] - par[1];
          Double_t dr = par[2] - std::sqrt(u*u+v*v);
          f += dr*dr;
       }
       return f;
    };
 
    // wrap chi2 funciton in a function object for the fit
    // 3 is the number of fit parameters (size of array par)
    ROOT::Math::Functor fcn(chi2Function,3);
    ROOT::Fit::Fitter  fitter;
 
 
    double pStart[3] = {0,0,1};
    fitter.SetFCN(fcn, pStart);
    fitter.Config().ParSettings(0).SetName("x0");
    fitter.Config().ParSettings(1).SetName("y0");
    fitter.Config().ParSettings(2).SetName("R");
 
    // do the fit 
    bool ok = fitter.FitFCN();
    if (!ok) {
       Error("line3Dfit","Line3D Fit failed");
    }   
 
    const ROOT::Fit::FitResult & result = fitter.Result();
 //   result.Print(std::cout);
 
    //Draw the circle on top of the points
    TArc *arc = new TArc(result.Parameter(0),result.Parameter(1),result.Parameter(2));
    arc->SetLineColor(kGreen);
    arc->SetFillStyle(0);
    arc->SetLineWidth(2);
    arc->Draw("same");

    //Draw Tracker
    Double_t radius[9] = {3.3,4.35,5.4,13.34,17.96,47.72,49.57,75.61,77.46};
   
    for (int ilayer=0; ilayer<9; ilayer++)
   {
    arc = new TArc(0.,0.,radius[ilayer]);
    if (ilayer<3) arc->SetLineColor(kBlue);
    else if (ilayer<5) arc->SetLineColor(kCyan);
    else  arc->SetLineColor(kRed);
    arc->SetFillStyle(0);
    arc->SetLineWidth(2);
    arc->Draw("same");
   }


  for (int iloop =0; iloop<1; iloop++){
  	
  // Put the hit Points on the detector layers
  TGraphErrors *gr1 = new TGraphErrors(n); 
  TGraphErrors *gr2 = new TGraphErrors(n); 
  gr1->SetMarkerStyle(47);
  gr1->SetMarkerSize(1.5);
  gr1->SetMarkerColor(kMagenta);
  gr2->SetMarkerStyle(47);
  gr2->SetMarkerSize(1.5);
  gr2->SetMarkerColor(kMagenta);
  // Find Intersection Point of two circle
  for (Int_t i=0;i<n;i++) 
  {  	
  Circle_Intersection c_1(result.Parameter(2),result.Parameter(0), result.Parameter(1));
  Circle_Intersection c_2(radius[i],0., 0.);
   // intersections point(s)
  Point2d i1, i2;
  size_t i_points = c_1.intersect(c_2, i1, i2);
  gr1->SetPoint(i,i1.x(),i1.y());
  gr2->SetPoint(i,i2.x(),i2.y());
  if (pT<0.02) continue;
  gr1->Draw("p-same");
  gr2->Draw("p-same");
  } 
} // loop
  a = gRandom->Uniform(-1.0*Radius,Radius);
 } // many circles for a and b

   TPaveText *pt = new TPaveText(0.1,0.9,0.2,0.99,"NDC");	
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   pt->SetTextSize(0.035);
   pt->SetTextColor(kGreen);
   pt->AddText(Form("Track p_{T} = %1.2f (GeV/c)",pT));
   pt->Draw("same");


   pt = new TPaveText(0.80,0.96,0.95,0.99,"NDC");	
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   pt->SetTextSize(0.03);
   pt->SetTextColor(kBlue);
   pt->AddText("Vertex Layers");
   pt->Draw("same");
   pt = new TPaveText(0.80,0.93,0.95,0.96,"NDC");	
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   pt->SetTextSize(0.03);
   pt->SetTextColor(kBlue);
   pt->SetTextColor(kCyan);
   pt->AddText("Barrel Layers");
   pt->Draw("same");
   pt = new TPaveText(0.80,0.90,0.95,0.93,"NDC");	
   pt->SetBorderSize(0);
   pt->SetFillStyle(0);
   pt->SetTextSize(0.03);
   pt->SetTextColor(kBlue);
   pt->SetTextColor(kRed);
   pt->AddText("Micromegas Layers");
   pt->Draw("same");
   c1->SaveAs(Form("track_pt_%1.2f_GeVperc.png",pT));
}
