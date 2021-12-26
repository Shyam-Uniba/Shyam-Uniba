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
 
const Double_t kR_min = 240;
const Double_t kR_max = 250;
const Double_t kZ_d   = 300;
 
// Solenoid field along z, in Tesla.
const Double_t kMagField = 4;
 
// Color for Higgs, Zs and muons
const Color_t  kColors[3] = { kRed, kGreen, kYellow };
 
 
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
 
   P.ReadString("HardQCD:all = on");
   P.ReadString("Random:setSeed = on");
   // use a reproducible seed: always the same results for the tutorial.
   P.ReadString("Random:seed = 42");
 
 
// Initialize
 
   P.Initialize(2212 /* p */, 2212 /* p */, 14000. /* TeV */);
 
   //========================================================================
   // Create views and containers.
   //========================================================================
 
   TEveManager::Create();
 
   TEveElementList *fake_geom = new TEveElementList("Geometry");
 
   TEveGeoShape *b;
 
   b = new TEveGeoShape("Barell 1");
   b->SetShape(new TGeoTube(kR_min, kR_max, kZ_d));
   b->SetMainColor(kCyan);
   b->SetMainTransparency(80);
   fake_geom->AddElement(b);
 
   b = new TEveGeoShape("Barell 2");
   b->SetShape(new TGeoTube(2*kR_min, 2*kR_max, 2*kZ_d));
   b->SetMainColor(kPink-3);
   b->SetMainTransparency(80);
   fake_geom->AddElement(b);
 
   gEve->AddGlobalElement(fake_geom);
 
 
   gMultiView = new MultiView;
 
   gMultiView->ImportGeomRPhi(fake_geom);
   gMultiView->ImportGeomRhoZ(fake_geom);
 
   gEve->GetBrowser()->GetTabRight()->SetTab(1);
 
   gTrackList = new TEveTrackList("Pythia Tracks");
   gTrackList->SetMainColor(kYellow);
   gTrackList->SetMarkerColor(kRed);
   gTrackList->SetMarkerStyle(4);
   gTrackList->SetMarkerSize(0.5);
   gEve->AddElement(gTrackList);
 
   TEveTrackPropagator* trkProp = gTrackList->GetPropagator();
   trkProp->SetMagField(kMagField);
   trkProp->SetMaxR(2*kR_max);
   trkProp->SetMaxZ(2*kZ_d);
 
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
      if (ist <= 0) continue;
      Int_t pdg = p.GetPdgCode();
      Float_t charge = TDatabasePDG::Instance()->GetParticle(pdg)->Charge();
      if (charge == 0.) continue;
 
      TEveTrack* track = new TEveTrack(&pb, i, trkProp);
      track->SetName(Form("%s [%d]", pb.GetName(), i));
      track->SetStdTitle();
      track->SetAttLineAttMarker(gTrackList);
      if (i == 0)
         track->SetLineColor(kColors[0]);
      else if (i <= 2)
         track->SetLineColor(kColors[1]);
 
      gTrackList->AddElement(track);
 
      /*
        printf("%d - %d %d %d %d %d %d\n", i,
        p.GetKF(), p.GetKS(), 0, 0,
        p.GetFirstChild()-nh-1, p.GetLastChild()-nh-1);
        printf("%d - %f %f %f %f\n", i,
        p.GetPx(), p.GetPy(), p.GetPz(), p.GetEnergy(),
        printf("%d - %f %f %f %f\n", i,
        p.GetVx(), p.GetVy(), p.GetVz(), p.GetTime());
      */
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
