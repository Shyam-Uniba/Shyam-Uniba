//==============================================================================
// Global variables.
//------------------------------------------------------------------------------
// run the macro: 
//root [0] .L pythia_display.C++
//Info in <TUnixSystem::ACLiC>: creating shared library /home/shyam/./pythia_display_C.so
//root [1] pythia_display()

 
#include "TEveTrack.h"
#include "TEveTrackPropagator.h"
#include "TEveElement.h"
#include "TEveGeoShape.h"
#include "TDatabasePDG.h" 
#include "TPythia8.h"
#include "TGeoTube.h"
#include "TMCParticle.h"
#include "TROOT.h" 
#include "TSystem.h"

#ifndef __RUN_PYTHIA_DISPLAY__
 
void pythia8_display()
{
   TString dir = gSystem->UnixPathName(__FILE__);
   dir.ReplaceAll("pythia8_display.C","");
   dir.ReplaceAll("/./","/");
   gROOT->LoadMacro("MultiView.C+");
 
#ifndef R__WIN32 // libPythia6 is a static library on Windoze
   if (gSystem->Load("libEGPythia8") < 0)
   {
      Error("pythia8_display()",
            "Could not load 'libPythia6', make sure it is available!");
      return;
   }
#endif
 //gSystem->Setenv("PYTHIA8DATA",Form("%s/xmldoc",getenv("PYTHIA8")));
 //gSystem->Setenv("PYTHIA8",Form("%s/xmldoc",getenv("PYTHIA8")));
 //gSystem->getenv("PYTHIA8");
// Load libraries
   gSystem->Load("libEG");
   gSystem->Load("libEGPythia8");
 
   gROOT->ProcessLine("#define __RUN_PYTHIA_DISPLAY__ 1");
   gROOT->ProcessLine("#include \"pythia8_display.C\"");
   gROOT->ProcessLine("run_pythia_display()");
   gROOT->ProcessLine("#undef __RUN_PYTHIA_DISPLAY__");
}
 
#else
 
//==============================================================================
// Constants.
//------------------------------------------------------------------------------
 
const Double_t kR_min = 0.0;
const Double_t kR_max = 10.;
const Double_t kZ_d   = 50.;
 
// Solenoid field along z, in Tesla.
const Double_t kMagField = 3;

TPythia8   *g_pythia = 0;
 
// Implemented in MultiView.C
class MultiView;
MultiView* gMultiView = 0;
 
TEveTrackList *gTrackList = 0;
 
//==============================================================================
// Forward decalarations of CINT functions.
//------------------------------------------------------------------------------
 
void pythia_next_event();
void pythia_make_gui();
 
//==============================================================================
// Main - pythia_display()
//------------------------------------------------------------------------------
 
void run_pythia_display()
{
   if (g_pythia != 0)
   {
      Warning("pythia_display()", "Already initialized.");
      return;
   }
 
   //========================================================================
   //========================================================================
 
   // Create an instance of the Pythia event generator ...
   g_pythia = new TPythia8;
   TPythia8& P = * g_pythia;
 
  // P.ReadString("HardQCD:all = on");
  P.ReadString("SoftQCD:inelastic = on"); // Pythia MB
  P.ReadString("Random:setSeed = on");
  P.ReadString("Random:seed = 200000000");
  P.ReadString("PartonLevel:MPI = on");
  P.ReadString("PhaseSpace:pTHatMin = 0.0");
  //pythia.readString("PhaseSpace:pTHatMinDiverge = 1"); 
  P.ReadString("Beams:idA = 2212");
  P.ReadString("Beams:idB = 2212");   // pid for proton
  P.ReadString("Beams:eCM = 13000."); // 13000 TeV
  P.ReadString("ColourReconnection:reconnect = on");
  P.ReadString("MultipartonInteractions:Kfactor= 1");
  P.ReadString("Random:setSeed = on");
 
 
// Initialize
 
  P.Initialize(2212 /* p */, 2212 /* p */, 13000. /* TeV */);
 
   //========================================================================
   // Create views and containers.
   //========================================================================
 
   TEveManager::Create();
 
   TEveElementList *Athena_Tracker = new TEveElementList("Geometry");
 
   TEveGeoShape *b;

	// Vertexing
  	double si_vtx_r_pos[] = {3.3,4.35,5.4}; 
	  const int nVtxLayers = sizeof(si_vtx_r_pos)/sizeof(*si_vtx_r_pos);
	  double si_z_vtxlength[] = {28, 28, 28};

	// Barrel
 	 double si_r_pos[] = {13.34, 17.96};
   const int nTrckLayers = sizeof(si_r_pos)/sizeof(*si_r_pos);
	  double si_z_length[] = {34.34, 46.68};

	  double si_thick = 0.55/100.*9.37;

   for (int i=0; i<nVtxLayers+nTrckLayers; i++){
   if (i<3){
   b = new TEveGeoShape(Form("Vtx_%d",i));
   b->SetShape(new TGeoTube(si_vtx_r_pos[i], si_vtx_r_pos[i]+si_thick, si_z_vtxlength[i]));
   b->SetMainColor(kRed);  	
   }	
   else{
   b = new TEveGeoShape(Form("Barr_%d",i-3));
   b->SetShape(new TGeoTube(si_r_pos[i-3], si_r_pos[i-3]+si_thick, si_z_length[i-3]));
   b->SetMainColor(kGreen); 	
   }
   b->SetMainTransparency(80);
   Athena_Tracker->AddElement(b);
  }
  

 
   gEve->AddGlobalElement(Athena_Tracker);
 
 
   gMultiView = new MultiView;
 
   gMultiView->ImportGeomRPhi(Athena_Tracker);
   gMultiView->ImportGeomRhoZ(Athena_Tracker);
 
   gEve->GetBrowser()->GetTabRight()->SetTab(1);
 
   gTrackList = new TEveTrackList("Pythia Tracks");
   gTrackList->SetMainColor(kYellow);
   gTrackList->SetMarkerColor(kRed);
   gTrackList->SetMarkerStyle(4);
   gTrackList->SetMarkerSize(0.5);
   gEve->AddElement(gTrackList);
 
   TEveTrackPropagator* trkProp = gTrackList->GetPropagator();
   trkProp->SetMagField(kMagField);
   trkProp->SetMaxR(2.0*kR_max);
   trkProp->SetMaxZ(kZ_d);
 
   //========================================================================
   //========================================================================
 
   pythia_make_gui();
   pythia_next_event();
 
   gEve->Redraw3D(kTRUE);
}
 
 
//==============================================================================
// Next event
//------------------------------------------------------------------------------
 
void pythia_next_event()
{
   gTrackList->DestroyElements();
 
   TPythia8& P = *g_pythia;
 
   P.GenerateEvent();
   P.EventListing();
 
   TEveTrackPropagator *trkProp = gTrackList->GetPropagator();
  // TClonesArray        &MC      = * (TClonesArray*) P.GetListOfParticles();
   TClonesArray        &MC      = * (TClonesArray*) P.ImportParticles("All"); // All, Final
   int ntracks = MC.GetEntriesFast();
   for (Int_t i = 0; i <ntracks; ++i)
   {
      TParticle& p = (TParticle&)*MC[i];
      TParticle pb(p.GetPdgCode(), p.GetStatusCode(), 0, 0,
                   p.GetFirstDaughter(), p.GetLastDaughter() ,
                   p.Px(), p.Py(), p.Pz(), p.Energy(),
                   p.Vx(), p.Vy(), p.Vz(), p.T());

       Int_t ist = p.GetStatusCode();
      // Positive codes are final particles.
      if (ist != 1) continue;
      Int_t pdg = p.GetPdgCode();
      Float_t charge = TDatabasePDG::Instance()->GetParticle(pdg)->Charge();
      if (charge == 0.) continue;
 
      TEveTrack* track = new TEveTrack(&pb, i, trkProp);
      track->SetName(Form("%s [%d]", pb.GetName(), i));
      track->SetStdTitle();
      track->SetAttLineAttMarker(gTrackList);
      if (fabs(pdg)==211) track->SetLineColor(kYellow);
      else if (fabs(pdg)==321) track->SetLineColor(kMagenta);
      else if (fabs(pdg)==2212) track->SetLineColor(kBlue);
      else if (fabs(pdg)==13) track->SetLineColor(kCyan);
      else if (fabs(pdg)==11) track->SetLineColor(kWhite);
      else track->SetLineColor(28); 
      gTrackList->AddElement(track);
   }
 
   gTrackList->MakeTracks();
 
 
   TEveElement* top = static_cast<TEveElement*>(gEve->GetEventScene());
 
   gMultiView->DestroyEventRPhi();
   gMultiView->ImportEventRPhi(top);
 
   gMultiView->DestroyEventRhoZ();
   gMultiView->ImportEventRhoZ(top);
 
   gEve->Redraw3D();
}
 
 
//==============================================================================
// GUI stuff
//------------------------------------------------------------------------------
class EvNavHandler
{
public:
   void Fwd()
   {
      pythia_next_event();
   }
   void Bck()
   {}
};
 
//______________________________________________________________________________
void pythia_make_gui()
{
   // Create minimal GUI for event navigation.
 
   TEveBrowser* browser = gEve->GetBrowser();
   browser->StartEmbedding(TRootBrowser::kLeft);
 
   TGMainFrame* frmMain = new TGMainFrame(gClient->GetRoot(), 1000, 600);
   frmMain->SetWindowName("XX GUI");
   frmMain->SetCleanup(kDeepCleanup);
 
   TGHorizontalFrame* hf = new TGHorizontalFrame(frmMain);
   {
 
      TString icondir( Form("%s/icons/", gSystem->Getenv("ROOTSYS")) );
      TGPictureButton* b = 0;
      EvNavHandler    *fh = new EvNavHandler;
 
      b = new TGPictureButton(hf, gClient->GetPicture(icondir+"GoBack.gif"));
      b->SetEnabled(kFALSE);
      b->SetToolTipText("Go to previous event - not supported.");
      hf->AddFrame(b);
      b->Connect("Clicked()", "EvNavHandler", fh, "Bck()");
 
      b = new TGPictureButton(hf, gClient->GetPicture(icondir+"GoForward.gif"));
      b->SetToolTipText("Generate new event.");
      hf->AddFrame(b);
      b->Connect("Clicked()", "EvNavHandler", fh, "Fwd()");
   }
   frmMain->AddFrame(hf);
 
   frmMain->MapSubwindows();
   frmMain->Resize();
   frmMain->MapWindow();
 
   browser->StopEmbedding();
   browser->SetTabTitle("Event Control", 0);
}
 
#endif

/*
 Pythia 6

    status 1: Stable final-state particle
    status 2: Unstable particle
    status 10902: Exactly the same as status 2 above
    status 3: Documentary particle; Often a process generated outside pythia, then passed to it for showering 

Pythia 8

    Negative vs. Positive: A particle which decays is given a negative status; the final state only consists of positive-status particles
    status 1: Final-state particle
    status 11-19: Beam particles
    status 21-29: Particles from the hardest subprocess
    status 31-39: Particles from subsequent subprocesses in multiple interactions
    status 41-49: Particles produced by initial-state showers (ISR, or generally particles not from the final state of the hard process)
    status 51-59: Particles produced by final-state showers
    status 61-69: Particles produced by beam-remnant treatment
    status 71-79: Particles about to be hadronized (input partons to a hadron)
    status 81-89: Primary output of hadronization process (first level of hadrons)
    status 91-99: Particles produced in final decay process, or by Bose-Einstein effects (?) 
*/
