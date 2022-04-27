void Induced_Signal(){
	
 float ve = 5.5; // 5.5 cm/mu-s
 float d = 1; // 1 cm
 float te, dq, dx,i;
 te = d/ve;
 TCanvas* c1 = new TCanvas("Simple plot1", "Simulation", 1500, 1000);
 c1->SetMargin(0.1,0.03,0.1,0.07);
 std::vector<float> a,b;

 for (float t=0.0; t<te/7; t=t+0.0005){
 	dx = ve*t;
 	dq = 1*(d-dx)/d;
        if (t<0.001) i = 0;
        else i = dq/t;
        a.push_back(dx);
        b.push_back(i);
 }
const int n = a.size();
float x[n],y[n];
for (int k=0; k<n; k++){
	x[k] = a[k]*1000;
        y[k] = b[k];
}
TGraph *gr = new TGraph(n,x,y);
gr->SetMarkerStyle(20);
gr->SetMarkerSize(0.7);
gr->SetMarkerColor(kBlue);
gr->GetXaxis()->SetTitle("Time (ns)");
gr->GetYaxis()->SetTitle("Current (a.u.)");
gr->SetTitle("Current Pulse");
gr->GetXaxis()->CenterTitle();
gr->GetYaxis()->CenterTitle();
gr->SetLineColor(kBlue);
gr->SetLineWidth(2.0);
c1->cd();
gr->Draw("ACP");

}
