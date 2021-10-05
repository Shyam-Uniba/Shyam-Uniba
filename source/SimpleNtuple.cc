#include "SimpleNtuple.h"

#include <g4main/PHG4Hit.h>
#include <g4main/PHG4HitContainer.h>

#include <fun4all/Fun4AllHistoManager.h>
#include <fun4all/SubsysReco.h>  // for SubsysReco
#include <g4main/PHG4Particle.h>
#include <g4main/PHG4TruthInfoContainer.h>
#include <phool/getClass.h>

#include <TFile.h>
#include <TH1.h>
#include <TNtuple.h>
#include "TTree.h"

#include <sstream>
#include <utility>  // for pair

using namespace std;

SimpleNtuple::SimpleNtuple(const std::string &name, const std::string &filename)
  : SubsysReco(name)
  , m_HistoManager(nullptr)
  , m_Ntup_tr(nullptr)
  , m_Outfile(nullptr)
  , m_Filename(filename)
  , _truth_container(nullptr)
  , _event(0)
  , tree(nullptr)
{
}

SimpleNtuple::~SimpleNtuple()
{
  delete m_HistoManager;
}

int SimpleNtuple::Init(PHCompositeNode *)
{
  m_HistoManager = new Fun4AllHistoManager(Name());
  m_Outfile = new TFile(m_Filename.c_str(), "RECREATE");
   // Create a TTree of Proton properties
  tree = new TTree("Hits","Basic Properties of Tracks");
  tree->Branch("DetId",&detId);
  tree->Branch("LayerId",&layerId);
  tree->Branch("TrackId",&trackId);
  tree->Branch("X0",&x0);
  tree->Branch("Y0",&y0);
  tree->Branch("Z0",&z0);
  tree->Branch("X1",&x1); 
  tree->Branch("Y1",&y1); 
  tree->Branch("Z1",&z1); 
  tree->Branch("Eloss",&edep); 

  m_Ntup_tr = new TNtuple("Tracks", "G4Tracks", "px:py:pz:trackId");
  //  ntup->SetDirectory(0);
  TH1 *h1 = new TH1F("edep1GeV", "edep 0-1GeV", 1000, 0, 1);
  m_ElossVec.push_back(h1);
  h1 = new TH1F("edep100GeV", "edep 0-100GeV", 1000, 0, 100);
  m_ElossVec.push_back(h1);
  return 0;
}

int SimpleNtuple::process_event(PHCompositeNode *topNode)
{
   _event++;
  if (_event % 1000 == 0)
  cout <<"\033[1;34m Events processed:\033[0m " << _event << endl;

  // Clearing the vectors

   detId.clear(); layerId.clear(); trackId.clear();
   x0.clear();    y0.clear();   z0.clear();
   x1.clear();    y1.clear();   z1.clear(); edep.clear();

	
  // Make sure to reset all the TTree variables before trying to set them.
  //DST objects
  //Truth container
  _truth_container = findNode::getClass<PHG4TruthInfoContainer>(topNode,
                                                                "G4TruthInfo");

  if (!_truth_container)
  {
   cout <<"\033[1;34m _truth_container not found! \033[0m\n";
    return 0;
  }

  Int_t truth_trackId;
  PHG4TruthInfoContainer::ConstRange range =
      _truth_container->GetPrimaryParticleRange();
  //std::cout << "A2" << std::endl;
  for (PHG4TruthInfoContainer::ConstIterator truth_itr = range.first;
       truth_itr != range.second; ++truth_itr)
{
     PHG4Particle *g4particle = truth_itr->second;
    if (!g4particle)
    {
      continue;
    }
     if (g4particle->get_parent_id()>0) continue; // Skipping Secondaries
     truth_trackId = g4particle->get_track_id();
     m_Ntup_tr->Fill(g4particle->get_px(),
                        g4particle->get_py(),
                        g4particle->get_pz(),
                        truth_trackId);

 }
  ostringstream nodename;
  set<string>::const_iterator iter;
  vector<TH1 *>::const_iterator eiter;
  for (iter = m_NodePostfixSet.begin(); iter != m_NodePostfixSet.end(); ++iter)
  {
    int detid = (m_DetIdMap.find(*iter))->second;
    nodename.str("");
    nodename << "G4HIT_" << *iter;
    
    PHG4HitContainer *hits = findNode::getClass<PHG4HitContainer>(topNode, nodename.str());
    if (hits)
    {
      double esum = 0;
      PHG4HitContainer::ConstRange hit_range = hits->getHits();
      for (PHG4HitContainer::ConstIterator hit_iter = hit_range.first; hit_iter != hit_range.second; hit_iter++)

      {
      	if (truth_trackId!=hit_iter->second->get_trkid()) continue; // Storing hits correspond to track
        esum += hit_iter->second->get_edep();
        detId.push_back(detid); layerId.push_back((int) hit_iter->second->get_layer()); 
        trackId.push_back((int) hit_iter->second->get_trkid()); 
        x0.push_back(hit_iter->second->get_x(0)); y0.push_back(hit_iter->second->get_y(0));  z0.push_back(hit_iter->second->get_z(0));
        x1.push_back(hit_iter->second->get_x(1)); y1.push_back(hit_iter->second->get_y(1));  z1.push_back(hit_iter->second->get_z(1));
        edep.push_back(hit_iter->second->get_edep());
      }
      for (eiter = m_ElossVec.begin(); eiter != m_ElossVec.end(); ++eiter)
      {
        (*eiter)->Fill(esum);
      }
    }
  } // hits
        tree->Fill();
  return 0;
}

int SimpleNtuple::End(PHCompositeNode *topNode)
{
  m_Outfile->cd();
  m_Ntup_tr->Write();
  tree->Write();
  m_Outfile->Write();
  m_Outfile->Close();
  delete m_Outfile;
  m_HistoManager->dumpHistos(m_Filename, "UPDATE");
  return 0;
}

void SimpleNtuple::AddNode(const std::string &name, const int detid)
{
  m_NodePostfixSet.insert(name);
  m_DetIdMap[name] = detid;
  return;
}
