// Check for details G4MSSteppingAction.cc
void Plot_MaterialMap(){
	
        gStyle->SetTitleSize(0.04,"");
        gStyle->SetTitleSize(0.04,"XY");	
        gStyle->SetTitleOffset(1.00,"Y");	
        gStyle->SetOptStat(1);
        gStyle->SetOptFit(1);

   	TCanvas *c = new TCanvas("c","c",0,52,1400,1000);
   	c->SetGridy();
	   c->SetMargin(0.08, 0.12 ,0.10,0.07);
	
   FILE *f=fopen("finalmaterial_map.txt","r+");

  TProfile2D* histhephi = new TProfile2D("hThePhi","",180,0.,360.,90,0.,180.);
  histhephi->SetTitle("Effective radiation length (X/X_{0}) vs #theta (deg) & #varphi (deg);#varphi (deg);#theta (deg);X/X_{0}");
 
  Double_t theta, phi, pathlength, x_x0, nuclear_int_length;

 	while(fscanf(f,"%lf %lf %lf %lf %lf",&theta,&phi,&pathlength,&x_x0,&nuclear_int_length) != EOF)
  {
  	// Theta is taken from vertical axis not from Z axis theta = 90. fot +Z axis
  	// theta 0. to 360. 360 bins
  if (theta >= 0. && theta <= 90.) histhephi->Fill(phi,90.0-theta,x_x0); 
  if (theta >= 180. && theta <= 270.) histhephi->Fill(phi,theta-90.,x_x0); // equivalent to 270. to 360.
  }

  c->cd();
  histhephi->GetXaxis()->CenterTitle();
  histhephi->GetYaxis()->CenterTitle();
  histhephi->GetZaxis()->CenterTitle();
  histhephi->GetZaxis()->SetRangeUser(0.,150.);
	 gStyle->SetPalette(kInvertedDarkBodyRadiator);
  histhephi->SetStats(kFALSE);
  histhephi->Draw("colz");
  c->Print("Mb_geant4.pdf");

}
