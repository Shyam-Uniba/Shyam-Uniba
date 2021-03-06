#include "MyD0Analysis.h"

#include <trackbase_historic/SvtxTrack.h>
#include <trackbase_historic/SvtxTrackMap.h>
#include <trackbase_historic/SvtxTrack_FastSim.h>
#include <trackbase_historic/SvtxVertex.h>  // for SvtxVertex
#include <trackbase_historic/SvtxVertexMap.h>

#include <g4main/PHG4Hit.h>
#include <g4main/PHG4HitContainer.h>
#include <g4main/PHG4Particle.h>
#include <g4main/PHG4TruthInfoContainer.h>
#include <g4main/PHG4VtxPoint.h>

#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/PHTFileServer.h>
#include <fun4all/SubsysReco.h>  // for SubsysReco

#include <phool/getClass.h>
#include <phool/phool.h>

#include <TH2.h>
#include <TSystem.h>
#include <TTree.h>
#include <TVector3.h>

#include <cassert>
#include <cmath>
#include <iostream>
#include <map>      // for _Rb_tree_const_ite...
#include <utility>  // for pair

#define LogError(exp) std::cout << "ERROR: " << __FILE__ << ": " << __LINE__ << ": " << exp << "\n"
#define LogWarning(exp) std::cout << "WARNING: " << __FILE__ << ": " << __LINE__ << ": " << exp << "\n"

using namespace std;

//----------------------------------------------------------------------------//
//-- Constructor:
//--  simple initialization
//----------------------------------------------------------------------------//
MyD0Analysis::MyD0Analysis(const string &name, const string &filename, const string &trackmapname)
  : SubsysReco(name)
  , _outfile_name(filename)
  , _trackmapname(trackmapname)
  , _event(0)
  , tracks(0)
  , _flags(NONE)
  , _eval_tree_tracks(nullptr)
  , _eval_tree_vertex(nullptr)
  , _h2d_Delta_mom_vs_truth_mom(nullptr)
  , _h2d_Delta_mom_vs_truth_eta(nullptr)
  , _truth_container(nullptr)
  , _trackmap(nullptr)
  , _vertexmap(nullptr)
{
  reset_variables();
}

//----------------------------------------------------------------------------//
//-- Init():
//--   Intialize all histograms, trees, and ntuples
//----------------------------------------------------------------------------//
int MyD0Analysis::Init(PHCompositeNode *topNode)
{
  cout << PHWHERE << " Openning file " << _outfile_name << endl;
  PHTFileServer::get().open(_outfile_name, "RECREATE");

  // create TTree
  _eval_tree_tracks = new TTree("D0Analysis", "D0Analysis");
  _eval_tree_tracks->Branch("event", &event, "event/I");
  _eval_tree_tracks->Branch("tracks", &tracks, "tracks/I");
  _eval_tree_tracks->Branch("gtrackID", &gtrackID, "gtrackID[tracks]/I");
  _eval_tree_tracks->Branch("gflavor", &gflavor, "gflavor[tracks]/I");
  _eval_tree_tracks->Branch("gpx", &gpx, "gpx[tracks]/F");
  _eval_tree_tracks->Branch("gpy", &gpy, "gpy[tracks]/F");
  _eval_tree_tracks->Branch("gpz", &gpz, "gpz[tracks]/F");
  _eval_tree_tracks->Branch("gvx", &gvx, "gvx[tracks]/F");
  _eval_tree_tracks->Branch("gvy", &gvy, "gvy[tracks]/F");
  _eval_tree_tracks->Branch("gvz", &gvz, "gvz[tracks]/F");
  _eval_tree_tracks->Branch("gvt", &gvt, "gvt[tracks]/F");
  _eval_tree_tracks->Branch("trackID", &trackID, "trackID[tracks]/I");
  _eval_tree_tracks->Branch("charge", &charge, "charge[tracks]/I");
  _eval_tree_tracks->Branch("nhits", &nhits, "nhits[tracks]/I");
  _eval_tree_tracks->Branch("px", &px, "px[tracks]/F");
  _eval_tree_tracks->Branch("py", &py, "py[tracks]/F");
  _eval_tree_tracks->Branch("pz", &pz, "pz[tracks]/F");
  _eval_tree_tracks->Branch("pcax", &pcax, "pcax[tracks]/F");
  _eval_tree_tracks->Branch("pcay", &pcay, "pcay[tracks]/F");
  _eval_tree_tracks->Branch("pcaz", &pcaz, "pcaz[tracks]/F");
  _eval_tree_tracks->Branch("dca2d", &dca2d, "dca2d[tracks]/F");
  _eval_tree_tracks->Branch("phi", &phi, "phi[tracks]/F");
  _eval_tree_tracks->Branch("eta", &eta, "eta[tracks]/F");
  _eval_tree_tracks->Branch("chi2", &chi2, "chi2[tracks]/F");
  _eval_tree_tracks->Branch("ndf", &ndf, "ndf[tracks]/F");

  const string xyz[3] = {"x", "y", "z"};
  for (map<string, int>::const_iterator iter = m_ProjectionNameMap.begin(); iter != m_ProjectionNameMap.end(); ++iter)
  {
    for (int i = 0; i < 3; i++)
    {
      string bname = iter->first + "_" + xyz[i];
      string bdef = bname + "/F";
      _eval_tree_tracks->Branch(bname.c_str(), &ref[i][iter->second], bdef.c_str());
    }
    for (int i = 0; i < 3; i++)
    {
      string bname = iter->first + "_p" +  xyz[i];
      string bdef = bname + "/F";
      _eval_tree_tracks->Branch(bname.c_str(), &ref_p[i][iter->second], bdef.c_str());
    }
    for (int i = 0; i < 3; i++)
    {
      string bname = iter->first + "_proj_" + xyz[i];
      string bdef = bname + "/F";
      _eval_tree_tracks->Branch(bname.c_str(), &proj[i][iter->second], bdef.c_str());
    }
    for (int i = 0; i < 3; i++)
    {
      string bname = iter->first + "_proj_p" + xyz[i];
      string bdef = bname + "/F";
      _eval_tree_tracks->Branch(bname.c_str(), &proj_p[i][iter->second], bdef.c_str());
    }
  }
  _h2d_Delta_mom_vs_truth_eta = new TH2D("_h2d_Delta_mom_vs_truth_eta",
                                         "#frac{#Delta p}{truth p} vs. truth #eta", 54, -4.5, +4.5, 1000, -1,
                                         1);

  _h2d_Delta_mom_vs_truth_mom = new TH2D("_h2d_Delta_mom_vs_truth_mom",
                                         "#frac{#Delta p}{truth p} vs. truth p", 41, -0.5, 40.5, 1000, -1,
                                         1);

  // create TTree - vertex
  _eval_tree_vertex = new TTree("vertex", "FastSim Eval => vertces");
  _eval_tree_vertex->Branch("event", &event, "event/I");
  _eval_tree_vertex->Branch("gvx", &gvx, "gvx/F");
  _eval_tree_vertex->Branch("gvy", &gvy, "gvy/F");
  _eval_tree_vertex->Branch("gvz", &gvz, "gvz/F");
  _eval_tree_vertex->Branch("gvt", &gvt, "gvt/F");
  _eval_tree_vertex->Branch("vx", &vx, "vx/F");
  _eval_tree_vertex->Branch("vy", &vy, "vy/F");
  _eval_tree_vertex->Branch("vz", &vz, "vz/F");
  _eval_tree_vertex->Branch("deltavx", &deltavx, "deltavx/F");
  _eval_tree_vertex->Branch("deltavy", &deltavy, "deltavy/F");
  _eval_tree_vertex->Branch("deltavz", &deltavz, "deltavz/F");
  _eval_tree_vertex->Branch("gID", &gtrackID_vtx, "gID/I");
  _eval_tree_vertex->Branch("ID", &trackID_vtx, "ID/I");
  _eval_tree_vertex->Branch("ntracks", &ntracks, "ntracks/I");
  _eval_tree_vertex->Branch("n_from_truth", &n_from_truth, "n_from_truth/I");

  return Fun4AllReturnCodes::EVENT_OK;
}

//----------------------------------------------------------------------------//
//-- process_event():
//--   Call user instructions for every event.
//--   This function contains the analysis structure.
//----------------------------------------------------------------------------//
int MyD0Analysis::process_event(PHCompositeNode *topNode)
{
  _event++;
  if (Verbosity() >= 2 and _event % 1000 == 0)
    cout << PHWHERE << "Events processed: " << _event << endl;

  //std::cout << "Opening nodes" << std::endl;
  GetNodes(topNode);

  //std::cout << "Filling trees" << std::endl;
  tracks = 0; // Initialize tracks to 0 in each event
  fill_track_tree(topNode);
  fill_vertex_tree(topNode);
  //std::cout << "DONE" << std::endl;

  return Fun4AllReturnCodes::EVENT_OK;
}

//----------------------------------------------------------------------------//
//-- End():
//--   End method, wrap everything up
//----------------------------------------------------------------------------//
int MyD0Analysis::End(PHCompositeNode *topNode)
{
  PHTFileServer::get().cd(_outfile_name);

  _eval_tree_tracks->Write();
  _eval_tree_vertex->Write();

  _h2d_Delta_mom_vs_truth_eta->Write();
  _h2d_Delta_mom_vs_truth_mom->Write();

  //PHTFileServer::get().close();

  return Fun4AllReturnCodes::EVENT_OK;
}

//----------------------------------------------------------------------------//
//-- fill_tree():
//--   Fill the trees with truth, track fit, and cluster information
//----------------------------------------------------------------------------//
void MyD0Analysis::fill_track_tree(PHCompositeNode *topNode)
{
  // Make sure to reset all the TTree variables before trying to set them.
  int count_tracks = 0;
  if (!_truth_container)
  {
    LogError("_truth_container not found!");
    return;
  }

  if (!_trackmap)
  {
    LogError("_trackmap not found!");
    return;
  }

  PHG4TruthInfoContainer::ConstRange range =
      _truth_container->GetParticleRange();
  //std::cout << "A2" << std::endl;
  for (PHG4TruthInfoContainer::ConstIterator truth_itr = range.first;
       truth_itr != range.second; ++truth_itr)
  {
    reset_variables();
    //std::cout << "A1" << std::endl;
    event = _event;

    PHG4Particle *g4particle = truth_itr->second;
    if (!g4particle)
    {
      continue;
    }
    //std::cout << "B1" << std::endl;

    SvtxTrack_FastSim *track = nullptr;

    //std::cout << "TRACKmap size " << _trackmap->size() << std::endl;
    for (SvtxTrackMap::ConstIter track_itr = _trackmap->begin();
         track_itr != _trackmap->end();
         track_itr++)
    {
      //std::cout << "TRACK * " << track_itr->first << std::endl;
      SvtxTrack_FastSim *temp = dynamic_cast<SvtxTrack_FastSim *>(track_itr->second);
      if (!temp)
      {
        std::cout << "ERROR CASTING PARTICLE!" << std::endl;
        continue;
      }
      //std::cout << " PARTICLE!" << std::endl;

      if ((temp->get_truth_track_id() - g4particle->get_track_id()) == 0)
      {
        track = temp;
      }
    }

    int pdg_track = fabs(g4particle->get_pid());
    if (pdg_track!=211 && pdg_track!=321) continue;
    //std::cout << "B2" << std::endl;
    gtrackID[tracks] = g4particle->get_track_id();
    gflavor[tracks] = g4particle->get_pid();

    gpx[tracks] = g4particle->get_px();
    gpy[tracks] = g4particle->get_py();
    gpz[tracks] = g4particle->get_pz();
    gvx = NAN;
    gvy = NAN;
    gvz = NAN;
    gvt = NAN;
    PHG4VtxPoint *vtx = _truth_container->GetVtx(g4particle->get_vtx_id());
    if (vtx)
    {
      gvx = vtx->get_x();
      gvy = vtx->get_y();
      gvz = vtx->get_z();
      gvt = vtx->get_t();
    }

    if (track)
    {
      //std::cout << "C1" << std::endl;
      trackID[tracks] = track->get_id();
      charge[tracks] = track->get_charge();
      nhits[tracks] = track->size_clusters();

      px[tracks] = track->get_px();
      py[tracks] = track->get_py();
      pz[tracks] = track->get_pz();
      pcax[tracks] = track->get_x();
      pcay[tracks] = track->get_y();
      pcaz[tracks] = track->get_z();
      dca2d[tracks] = track->get_dca2d();
      phi[tracks] = track->get_phi();
      eta[tracks] = track->get_eta();
      chi2[tracks] = track->get_chisq();
      ndf[tracks] = track->get_ndf();

      TVector3 truth_mom(gpx[tracks], gpy[tracks], gpz[tracks]);
      TVector3 reco_mom(px[tracks], py[tracks], pz[tracks]);
      //std::cout << "C2" << std::endl;

      _h2d_Delta_mom_vs_truth_mom->Fill(truth_mom.Mag(), (reco_mom.Mag() - truth_mom.Mag()) / truth_mom.Mag());
      _h2d_Delta_mom_vs_truth_eta->Fill(truth_mom.Eta(), (reco_mom.Mag() - truth_mom.Mag()) / truth_mom.Mag());
      // find projections
      for (int k = 0; k < 3; k++)
      {
        for (int j = 0; j < nproj; j++)
        {
          proj[k][j] = -9999;
          proj_p[k][j] = -9999;
          ref[k][j] = -9999;
          ref_p[k][j] = -9999;
        }
      }
      for (SvtxTrack::ConstStateIter trkstates = track->begin_states();
           trkstates != track->end_states();
           ++trkstates)
      {
        //	cout << "checking " << trkstates->second->get_name() << endl;
        map<string, int>::const_iterator iter = m_ProjectionNameMap.find(trkstates->second->get_name());
        if (iter != m_ProjectionNameMap.end())
        {
          //	  cout << "found " << trkstates->second->get_name() << endl;
          // setting the projection (xyz and pxpypz)
          proj[0][iter->second] = trkstates->second->get_x();
          proj[1][iter->second] = trkstates->second->get_y();
          proj[2][iter->second] = trkstates->second->get_z();
          proj_p[0][iter->second] = trkstates->second->get_px();
          proj_p[1][iter->second] = trkstates->second->get_py();
          proj_p[2][iter->second] = trkstates->second->get_pz();

          string nodename = "G4HIT_" + trkstates->second->get_name();
          PHG4HitContainer *hits = findNode::getClass<PHG4HitContainer>(topNode, nodename);
          if (!hits)
          {
            cout << "could not find " << nodename << endl;
            continue;
          }
          //	  cout << "number of hits: " << hits->size() << endl;
          PHG4HitContainer::ConstRange hit_range = hits->getHits();
          for (PHG4HitContainer::ConstIterator hit_iter = hit_range.first; hit_iter != hit_range.second; hit_iter++)
          {
            //	    cout << "checking hit id " << hit_iter->second->get_trkid() << " against " << track->get_truth_track_id() << endl;
            if (hit_iter->second->get_trkid() - track->get_truth_track_id() == 0)
            {
              //	      cout << "found hit with id " << hit_iter->second->get_trkid() << endl;
              if (iter->second >= nproj)
              {
                cout << "bad index: " << iter->second << endl;
                gSystem->Exit(1);
              }
              ref[0][iter->second] = hit_iter->second->get_x(0);
              ref[1][iter->second] = hit_iter->second->get_y(0);
              ref[2][iter->second] = hit_iter->second->get_z(0);
              ref_p[0][iter->second] = hit_iter->second->get_px(0);
              ref_p[1][iter->second] = hit_iter->second->get_py(0);
              ref_p[2][iter->second] = hit_iter->second->get_pz(0);
            }
          }
        }
      }
    }
    tracks++; 
    //std::cout << "B3" << std::endl;
  }
  //std::cout << "A3" << std::endl;
    _eval_tree_tracks->Fill();

  return;
}

//----------------------------------------------------------------------------//
//-- fill_tree():
//--   Fill the trees with truth, track fit, and cluster information
//----------------------------------------------------------------------------//
void MyD0Analysis::fill_vertex_tree(PHCompositeNode *topNode)
{
  if (!_truth_container)
  {
    LogError("_truth_container not found!");
    return;
  }

  if (!_trackmap)
  {
    LogError("_trackmap not found!");
    return;
  }

  if (!_vertexmap)
  {
    return;
  }

  for (SvtxVertexMap::Iter iter = _vertexmap->begin();
       iter != _vertexmap->end();
       ++iter)
  {
    SvtxVertex *vertex = iter->second;

    // Make sure to reset all the TTree variables before trying to set them.
    reset_variables();
    //std::cout << "A1" << std::endl;
    event = _event;

    if (!vertex)
    {
      continue;
    }

    //std::cout << "C1" << std::endl;
    trackID_vtx = vertex->get_id();
    ntracks = vertex->size_tracks();

    vx = vertex->get_x();
    vy = vertex->get_y();
    vz = vertex->get_z();
    deltavx = sqrt(vertex->get_error(1, 1));
    deltavy = sqrt(vertex->get_error(2, 2));
    deltavz = sqrt(vertex->get_error(3, 3));

    // best matched vertex
    PHG4VtxPoint *best_vtx = nullptr;
    int best_n_match = -1;
    map<PHG4VtxPoint *, int> vertex_match_map;
    for (auto iter = vertex->begin_tracks(); iter != vertex->end_tracks(); ++iter)
    {
      const auto &trackID = *iter;
      const auto trackIter = _trackmap->find(trackID);

      if (trackIter == _trackmap->end()) continue;

      SvtxTrack_FastSim *temp = dynamic_cast<SvtxTrack_FastSim *>(trackIter->second);

      if (!temp) continue;

      const auto g4trackID = temp->get_truth_track_id();
      const PHG4Particle *g4particle = _truth_container->GetParticle(g4trackID);
      assert(g4particle);
      PHG4VtxPoint *vtx = _truth_container->GetVtx(g4particle->get_vtx_id());

      int n_match = ++vertex_match_map[vtx];

      if (n_match > best_n_match)
      {
        best_n_match = n_match;
        best_vtx = vtx;
      }
    }
    if (best_vtx)
    {
      gvx = best_vtx->get_x();
      gvy = best_vtx->get_y();
      gvz = best_vtx->get_z();
      gvt = best_vtx->get_t();

      n_from_truth = best_n_match;
      gtrackID_vtx = best_vtx->get_id();
    }
    _eval_tree_vertex->Fill();
  }
  //std::cout << "B3" << std::endl;

  return;
}

//----------------------------------------------------------------------------//
//-- reset_variables():
//--   Reset all the tree variables to their default values.
//--   Needs to be called at the start of every event
//----------------------------------------------------------------------------//
void MyD0Analysis::reset_variables()
{
  event= -9999;

  //-- truth
  gtrackID[tracks] = -9999;
  gflavor[tracks] = -9999;
  gpx[tracks] = NAN;
  gpy[tracks] = NAN;
  gpz[tracks] = NAN;

  gvx = NAN;
  gvy = NAN;
  gvz = NAN;
  gvt = NAN;

  //-- reco
  trackID[tracks] = -9999;
  charge[tracks] = -9999;
  nhits[tracks] = -9999;
  px[tracks] = NAN;
  py[tracks] = NAN;
  pz[tracks] = NAN;
  pcax[tracks] = NAN;
  pcay[tracks] = NAN;
  pcaz[tracks] = NAN;
  dca2d[tracks] = NAN;
  phi[tracks] = -9999;
  eta[tracks] = -9999;
  chi2[tracks] =-9999;
  ndf[tracks] =-9999;

  vx = NAN;
  vy = NAN;
  vz = NAN;
  deltavx = NAN;
  deltavy = NAN;
  deltavz = NAN;
  ntracks = -9999;
  n_from_truth = -9999;
}

//----------------------------------------------------------------------------//
//-- GetNodes():
//--   Get all the all the required nodes off the node tree
//----------------------------------------------------------------------------//
int MyD0Analysis::GetNodes(PHCompositeNode *topNode)
{
  //DST objects
  //Truth container
  _truth_container = findNode::getClass<PHG4TruthInfoContainer>(topNode,
                                                                "G4TruthInfo");
  if (!_truth_container && _event < 2)
  {
    cout << PHWHERE << " PHG4TruthInfoContainer node not found on node tree"
         << endl;
    return Fun4AllReturnCodes::ABORTEVENT;
  }

  _trackmap = findNode::getClass<SvtxTrackMap>(topNode,
                                               _trackmapname);
  //std::cout << _trackmapname.c_str() << std::endl;
  if (!_trackmap)
  {
    cout << PHWHERE << "SvtxTrackMap node with name "
         << _trackmapname
         << " not found on node tree"
         << endl;
    return Fun4AllReturnCodes::ABORTEVENT;
  }

  _vertexmap = findNode::getClass<SvtxVertexMap>(topNode, "SvtxVertexMap");
  if (!_vertexmap && Verbosity())
  {
    cout << PHWHERE << "SvtxTrackMap node with name SvtxVertexMap not found on node tree. Will not build the vertex eval tree"
         << endl;
  }

  return Fun4AllReturnCodes::EVENT_OK;
}

void MyD0Analysis::AddProjection(const string &name)
{
  m_ProjectionNameMap.insert(make_pair(name, m_ProjectionNameMap.size()));
  return;
}
