#ifndef L1Trigger_CSCTriggerPrimitives_CSCTriggerPrimitivesBuilder_h
#define L1Trigger_CSCTriggerPrimitives_CSCTriggerPrimitivesBuilder_h

/** class CSCTriggerPrimitivesBuilder
 *
 * Algorithm to build anode, cathode, and correlated LCTs from wire and
 * comparator digis in endcap muon CSCs by implementing a 'build' function
 * required by CSCTriggerPrimitivesProducer.
 *
 * Configured via the Producer's ParameterSet.
 *
 * author Slava Valuev, UCLA.
 *
 * The builder was expanded to use GEM pad or GEM pad clusters
 * In addition the builder can produce GEM coincidence pads in
 * case an upgrade scenario with GEMs is run.
 *
 * authors: Sven Dildick (TAMU), Tao Huang (TAMU)
 */

#include "CondFormats/CSCObjects/interface/CSCBadChambers.h"
#include "CondFormats/CSCObjects/interface/CSCL1TPLookupTableCCLUT.h"
#include "CondFormats/CSCObjects/interface/CSCL1TPLookupTableME21ILT.h"
#include "CondFormats/CSCObjects/interface/CSCL1TPLookupTableME11ILT.h"
#include "DataFormats/CSCDigi/interface/CSCComparatorDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCWireDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCALCTDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCCLCTDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCALCTPreTriggerDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCCLCTPreTriggerDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCCLCTPreTriggerCollection.h"
#include "DataFormats/CSCDigi/interface/CSCShowerDigiCollection.h"
#include "DataFormats/GEMDigi/interface/GEMPadDigiClusterCollection.h"
#include "DataFormats/GEMDigi/interface/GEMCoPadDigiCollection.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

class CSCDBL1TPParameters;
class CSCMotherboard;
class CSCMuonPortCard;
class CSCGeometry;
class GEMGeometry;
class CSCTriggerPrimitivesBuilder {
public:
  /** Configure the algorithm via constructor.
   *  Receives ParameterSet percolated down from EDProducer which owns this
   *  Builder.
   */
  explicit CSCTriggerPrimitivesBuilder(const edm::ParameterSet&);

  ~CSCTriggerPrimitivesBuilder();

  struct BuildContext {
    const CSCL1TPLookupTableCCLUT* cclut_;
    const CSCL1TPLookupTableME11ILT* me11ilt_;
    const CSCL1TPLookupTableME21ILT* me21ilt_;
    const CSCGeometry* cscgeom_;
    const GEMGeometry* gemgeom_;
    const CSCDBL1TPParameters* parameters_;
  };

  // set run number and event number
  void setRunNumber(int runNumber) {runNumber_ = runNumber;}
  void setEventNumber(int evtNumber) {evtNumber_ = evtNumber;}

  // Build anode, cathode, and correlated LCTs in each chamber and fill them
  // into output collections.  Pass collections of wire and comparator digis
  // to Trigger MotherBoard (TMB) processors, which, in turn, pass them to
  // ALCT and CLCT processors.  Up to 2 anode and 2 cathode LCTs can be found
  // in each chamber during any bunch crossing.  The 2 projections are then
  // combined into three-dimensional "correlated" LCTs in the TMB.  Finally,
  // MPC processor sorts up to 18 LCTs from 9 TMBs and writes collections of
  // up to 3 best LCTs per (sub)sector into Event (to be used by the Sector
  // Receiver).
  void build(const CSCBadChambers* badChambers,
             const CSCWireDigiCollection* wiredc,
             const CSCComparatorDigiCollection* compdc,
             const GEMPadDigiClusterCollection* gemPadClusters,
             const BuildContext& context,
             CSCALCTDigiCollection& oc_alct,
             CSCCLCTDigiCollection& oc_clct,
             CSCALCTPreTriggerDigiCollection& oc_alctpretrigger,
             CSCCLCTPreTriggerDigiCollection& oc_clctpretrigger,
             CSCCLCTPreTriggerCollection& oc_pretrig,
             CSCCorrelatedLCTDigiCollection& oc_lct,
             CSCCorrelatedLCTDigiCollection& oc_sorted_lct,
             CSCShowerDigiCollection& oc_shower_anode,
             CSCShowerDigiCollection& oc_shower_cathode,
             CSCShowerDigiCollection& oc_shower,
             GEMCoPadDigiCollection& oc_gemcopad);

  /** Max values of trigger labels for all CSCs; used to construct TMB
   *  processors. */
  enum trig_cscs { MAX_ENDCAPS = 2, MAX_STATIONS = 4, MAX_SECTORS = 6, MAX_SUBSECTORS = 2, MAX_CHAMBERS = 9 };

private:
  /** template function to put data in the output
      helps to reduce the large amount of code duplication!
   */
  template <class T, class S>
  void put(const T&, S&, const CSCDetId&, std::string comment);

  /** Min and max allowed values for various CSC elements, defined in
   *  CSCDetId and CSCTriggerNumbering classes. */
  static const int min_endcap;  // endcaps
  static const int max_endcap;
  static const int min_station;  // stations per endcap
  static const int max_station;
  static const int min_sector;  // trigger sectors per station
  static const int max_sector;
  static const int min_subsector;  // trigger subsectors per sector
  static const int max_subsector;
  static const int min_chamber;  // chambers per trigger subsector
  static const int max_chamber;

  // debug
  int infoV;

  /// a flag whether to skip chambers from the bad chambers map
  bool checkBadChambers_;

  /** Phase2: special configuration parameters for ME11 treatment. */
  bool runPhase2_;

  /** Phase2: special switch for disabling ME42 */
  bool disableME42_;

  /** Phase2: individual switches */
  bool runME11Up_;
  bool runME21Up_;

  /** Phase2: special switch for the upgrade ME1/1 TMB */
  bool runME11ILT_;

  /** Phase2: special switch for the upgrade ME2/1 TMB */
  bool runME21ILT_;

  /*run number and evt number for debugging*/
  int runNumber_;
  int evtNumber_;

  /** Selected chambers to run */
  std::vector<std::string> selectedChambers_;

  /** Pointers to TMB processors for all possible chambers. */
  std::unique_ptr<CSCMotherboard> tmb_[MAX_ENDCAPS][MAX_STATIONS][MAX_SECTORS][MAX_SUBSECTORS][MAX_CHAMBERS];

  /** Pointer to MPC processors. */
  std::unique_ptr<CSCMuonPortCard> mpc_[MAX_ENDCAPS][MAX_STATIONS][MAX_SECTORS];
};

template <class T, class S>
void CSCTriggerPrimitivesBuilder::put(const T& t, S& s, const CSCDetId& detid, std::string comment) {
  if (!t.empty()) {
    LogTrace("L1CSCTrigger") << "Put " << t.size() << comment << ((t.size() > 1) ? "s " : " ") << "in collection\n";
    s.put(std::make_pair(t.begin(), t.end()), detid);
  }
}

#endif
