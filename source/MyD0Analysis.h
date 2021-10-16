// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef MYD0ANALYSIS_H
#define MYD0ANALYSIS_H

#include <fun4all/SubsysReco.h>
#include <map>
#include <set>
#include <string>

//Forward declarations
class PHCompositeNode;
class PHG4TruthInfoContainer;
class SvtxTrackMap;
class SvtxVertexMap;
class TTree;
class TH2D;

//Brief: basic ntuple and histogram creation for sim evaluation
class MyD0Analysis : public SubsysReco
{
 public:
  //Default constructor
  MyD0Analysis(const std::string& name = "MyD0Analysis",
                   const std::string& filename = "g4eval.root",
                   const std::string& trackmapname = "SvtxTrackMap");

  //Initialization, called for initialization
  int Init(PHCompositeNode*);

  //Process Event, called for each event
  int process_event(PHCompositeNode*);

  //End, write and close files
  int End(PHCompositeNode*);

  //Change output filename
  void set_filename(const char* file)
  {
    if (file) _outfile_name = file;
  }

  //Flags of different kinds of outputs
  enum Flag
  {
    //all disabled
    NONE = 0,
  };

  //Set the flag
  //Flags should be set like set_flag(MyD0Analysis::TRUTH, true) from macro
  void set_flag(const Flag& flag, const bool& value)
  {
    if (value)
      _flags |= flag;
    else
      _flags &= (~flag);
  }

  //User modules
  void reset_variables();
  void AddProjection(const std::string& name);

 private:
  void fill_track_tree(PHCompositeNode*);
  void fill_vertex_tree(PHCompositeNode*);

  //output filename
  std::string _outfile_name;

  //name of SvtxTrackMap collection
  std::string _trackmapname;

  //Event counter
  int _event;

  //Get all the nodes
  int GetNodes(PHCompositeNode*);

  //flags
  unsigned int _flags;

  //TTrees
  TTree* _eval_tree_tracks;
  TTree* _eval_tree_vertex;
  static const int arr_size = 10000;
  int event;
  int tracks;
  //-- truth
  int gtrackID[arr_size];
  int gflavor[arr_size];
  float gpx[arr_size];
  float gpy[arr_size];
  float gpz[arr_size];
  float gvx;
  float gvy;
  float gvz;
  float gvt;

  //-- reco
  int trackID[arr_size];
  int charge[arr_size];
  int nhits[arr_size];
  float px[arr_size];
  float py[arr_size];
  float pz[arr_size];
  float pcax[arr_size];
  float pcay[arr_size];
  float pcaz[arr_size];
  float dca2d[arr_size];
  float phi[arr_size];
  float eta[arr_size];
  float chi2[arr_size];
  float ndf[arr_size];


  static const int nproj = 3;
  // projections hits/mom
  float proj[3][nproj];
  float proj_p[3][nproj];
  // hits/mom at reference
  float ref[3][nproj];
  float ref_p[3][nproj];

  //vertex
  int gtrackID_vtx;
  int trackID_vtx;
  float vx;
  float vy;
  float vz;
  float deltavx;
  float deltavy;
  float deltavz;
  int ntracks;
  int n_from_truth;

  //Histos
  TH2D* _h2d_Delta_mom_vs_truth_mom;
  TH2D* _h2d_Delta_mom_vs_truth_eta;

  //Node pointers
  PHG4TruthInfoContainer* _truth_container;
  SvtxTrackMap* _trackmap;
  SvtxVertexMap* _vertexmap;

  std::map<std::string, int> m_ProjectionNameMap;
};

#endif  //* TRACKFASTSIMEVAL_H *//
