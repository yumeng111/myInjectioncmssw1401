# Pattern injection test with emulator board
Document and code to generate comaprator digi and CLCT+LCT information for patten injection test


## Generate txt file with CSC L1 trigger emulator

  - step1: checkout cmssw and CSC L1 trigger emulator. 
  ```
cmsrel CMSSW_12_5_0_pre4
cd CMSSW_12_5_0_pre4/src
cmsenv
git cms-addpkg L1Trigger/CSCTriggerPrimitives
  ```
  Here is the link to CSCTrigger emulator in CMSSW:  https://github.com/cms-sw/cmssw/tree/master/L1Trigger/CSCTriggerPrimitives
  
  And here is the part for CLCT emulation: https://github.com/cms-sw/cmssw/blob/master/L1Trigger/CSCTriggerPrimitives/src/CSCCathodeLCTProcessor.cc 
  - step2: apply the changes to CSC L1 trigger emulator and compile cmssw. 
  ```
git clone https://github.com/tahuang1991/InjectCSCPatterns.git
cp InjectCSCPatterns/CSCTriggerPrimitives/src/* L1Trigger/CSCTriggerPrimitives/src/
cp InjectCSCPatterns/CSCTriggerPrimitives/interface/*  L1Trigger/CSCTriggerPrimitives/interface/
cp InjectCSCPatterns/CSCTriggerPrimitives/plugins/* L1Trigger/CSCTriggerPrimitives/plugins/
scram b -j 9
  ```
  One thing worth attention is that the above commands are to over-write the old files in L1Trigger/CSCTriggerPrimitives.  If L1Trigger/CSCTriggerPrimitives package is updated with some changes that InjectCSCPatterns/CSCTriggerPrimitives does not include, then over-write the old files may not work.  
  
  - step3: run CSC L1 trigger emulation to get txt file. Replace the inputFiles with sample you want to process and set maxEvents to the number of events you need
  ```
  cd L1Trigger/CSCTriggerPrimitives/test
  rm ComparatorDigi_CLCT_ME*.txt
  cmsRun runCSCTriggerPrimitiveProducer_cfg.py mc=True run3=True \
  inputFiles="file:/afs/cern.ch/user/t/tahuang/public/RelValSample1000GeVTest/27a95851-6358-485b-b15b-619f3404d795.root" \
  maxEvents=10 saveEdmOutput=False l1=True runME11ILT=True runCCLUTOTMB=True
  ```

  
The output files generated from CSC L1 trigger emulator include:
  - ComparatorDigi_CLCT_ME11.txt for ME11 chamber type, with CCLUT and GEMCSC algorithm on
  - ComparatorDigi_CLCT_ME21.txt for ME21 chamber type, with CCLUT on
  - ComparatorDigi_CLCT_ME3141.txt for ME3141 chamber type, with CCLUT on

Three example txt files from 10 events are included under data/

Everytime you run above program,  it would append the new printouts to the exist output files. Make sure that old files are removed if you want to creat new txt files

#### Files changed to print out comparator digis, GEM clusters, CLCT and LCTs
The printout code changes is summarized in the following commits:
  -  [a881941d8b459926564a0873621c55aae9090ca0](https://github.com/tahuang1991/InjectCSCPatterns/commit/a881941d8b459926564a0873621c55aae9090ca0)
  -  [c3fd3ace655739a7d86e099e63b35dc412310338](https://github.com/tahuang1991/InjectCSCPatterns/commit/c3fd3ace655739a7d86e099e63b35dc412310338)
  -  [e72bbca7013ad13fc0eec6d379f82c43af95df51](https://github.com/tahuang1991/InjectCSCPatterns/commit/e72bbca7013ad13fc0eec6d379f82c43af95df51)
 
The safe way to include printout code is applying the printout code changes to L1Trigger/CSCTriggerPrimitives by hand.The reason is because with newer CMSSW version, the CSC trigger emulator code might be modified for other reasons and you do not want to overwrite these changes.

What you need to do is apply the changes in 
  - CSCTriggerPrimitives/interface/CSCCathodeLCTProcessor.h, 
  - CSCTriggerPrimitives/interface/CSCBaseboard.h,
  - CSCTriggerPrimitives/interface/CSCTriggerPrimitivesBuilder.h, 
  - CSCTriggerPrimitives/src/CSCCathodeLCTProcessor.cc, 
  - CSCTriggerPrimitives/src/CSCMotherboard.cc, 
  - CSCTriggerPrimitives/src/CSCGEMMotherboard.cc, 
  - CSCTriggerPrimitives/src/CSCTriggerPrimitivesBuilder.cc, 
  - CSCTriggerPrimitives/plugins/CSCTriggerPrimitivesProducer.cc 
  
to the corresponding files under L1Trigger/CSCTriggerPrimitives/

## Conventions of txt file generated from CSC L1 trigger emulator
The typical printout for one chamber with comparator digi from one event is showed in the following:

Start with "Run number + Event number + CSCChamber with Comparatordigi:" + detector information (endcap=1 means postive endcap and =2 means negative endcap)
>```
>Run 1 Event 2006 CSCChamber with Comparatordigi: (end,station,ring,chamber) = 2, 1, 1, 7 
>```

Comparator digi part: ranked by BX and layer
>```
>Comparatordigi BX 7 Layer 1 halfstrip 67 
>Comparatordigi BX 7 Layer 2 halfstrip 67
>Comparatordigi BX 7 Layer 4 halfstrip 67
>Comparatordigi BX 7 Layer 5 halfstrip 67
>Comparatordigi BX 8 Layer 0 halfstrip 67
>Comparatordigi BX 8 Layer 3 halfstrip 67
>```

CLCT part: CLCTs in this chamber, up to two CLCTs per BX, ranked by BX
>```
>CSC CLCT #1: Valid = 1 BX = 7 Run-2 Pattern = 10 Run-3 Pattern = 4 Quality = 6 Comp Code 4095 Bend = 1  
>Slope = 0 CFEB = 2 Strip = 2 KeyHalfStrip = 66 KeyQuartStrip = 132 KeyEighthStrip = 265
>```

GEM clusters from GE11 in same endcap and with same chamber number as CSC: ranked by layer and BX
>```
>GEMCluster in GE11 layer1: bx 8 gemPad 26 size 3 roll 6 converted into CSC coordination: wiregroup 15 halfstrip 21
>GEMCluster in GE11 layer1: bx 8 gemPad 87 size 2 roll 6 converted into CSC coordination: wiregroup 15 halfstrip 58
>GEMCluster in GE11 layer2: bx 8 gemPad 43 size 3 roll 6 converted into CSC coordination: wiregroup 15 halfstrip 31
>```

LCT part: LCTs in this chamber, up to two LCTs per BX,  ranked by BX
>```
>CSC LCT #1: Valid = 1 BX = 8 Run-2 Pattern = 10 Run-3 Pattern = 4 Quality = 3 Bend = 1 Slope = 0   
>KeyHalfStrip = 66 KeyQuartStrip = 132 KeyEighthStrip = 265 KeyWireGroup = 104 Type (SIM) = 1 MPC Link = 0
>```


## Generate txt file with GEMCode

To be included as an optional way to dump comparator digis and CLCT+LCTs to txt file  from GEMCode package. 

##  MC Sample Production: GEN-SIM-DIGI-L1
The full chain of MC sample production in CMSSW includes severl steps like GEN-SIM-DIGI-L1-DIGIRAW-HLT-RAW2DIGI-RECO etc.  Here we only
need to focus on GEN-SIM-DIGI-L1 and we use cmsDriver to generate the configuration for MC production. More detail about MC production and 
cmsDriver is in this twiki: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideCmsDriver

The example scripts are under python/ directory.

### step1: GEN-SIM
First step is to generate muon particles from physics process or muon gun and then do Geant4 detector simuation. The following cmsDriver command is used to generate the cmsRun configuration to produce muon particles from muon gun with fixed pt 10GeV and absolute eta less than 2.8  and detector simulation step:
```
cmsDriver.py SingleMuPt10_Eta2p85_cfi --beamspot Run3RoundOptics25ns13TeVLowSigmaZ \
--conditions auto:phase1_2022_realistic --datatier GEN-SIM --era Run3 \
--eventcontent FEVTDEBUG --fileout file:step1.root --geometry DB:Extended \
--nStreams 2 --nThreads 8 --no_exec --number 10 --python_filename step_1_cfg.py --step GEN,SIM
```
   - SingleMuPt10_Eta2p85_cfi is taken from standard cmssw gerenerator fragment under Configuration/Generator/python. https://github.com/cms-sw/cmssw/tree/master/Configuration/Generator/python
   - the steps included GEN and SIM
   - datatier is GEN-SIM, which would collections related to GEN-SIM by default
   - beamspot is current default Run3 beam spot condition
   - gloal condition is auto:phase1_2022_realistic.  here is the twiki document gloabl tag used in different cmssw and different conditions: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions
   - era is Run3
   - geometry condition is DB:Extended
   - Keep full event contents with FEVTDEBUG.  We also can customize the collection to keep the parts we care about
   - output file is step1.root
   - generated configuration file from this command is step_1_cfg.py
   - number of threasd is 8, should be consistent with numer of cores when crab job is used to produce a data using this configuration
   
 Usually we only want to select the gen muons flying through the endcap region and therefore we can add a filter in this step_1_cfg.py by including the following lines in the bottom:
 ```
from randomizeMuonGun import randomizeMuonGunEndcap 
process = randomizeMuonGunEndcap(process)
 ```
 
 To run the cmsRun configuration under cmssw:
 ```
 cmsRun step_1_cfg.py
 ```
 
 ### step2: DIGI-L1
 
 The second step is to do digitiztion and L1 trigger emulation and the following is the cmsDriver command to generate the cmsRun configuration
 ```
cmsDriver.py step2 --conditions auto:phase1_2022_realistic --datatier GEN-SIM-DIGI --era Run3 \
--eventcontent FEVTDEBUGHLT --filein file:step1.root --fileout file:step2.root \
--geometry DB:Extended --nStreams 2 --nThreads 8 --no_exec --number 10 \
--python_filename step_2_cfg.py --step DIGI:pdigi_valid,L1
 ```
  - Here the steps include pdigi_valid, L1, which represents the digitization and L1 emulation
  - datatier is GEN-SIM-DIGI
  - conditions,era,geometry should be the same as step1
  - Keep full event contents with FEVTDEBUG.  We also can customize the collection to keep the parts we care about so that sample size would be more slimmed 
  - step2 configuration takes the step1 output (step1.root) as input and the output file name is step2.root
  
   
 To run the cmsRun configuration under cmssw:
 ```
 cmsRun step_2_cfg.py
 ```
 
 The step2.root also can be used as input for runCSCTriggerPrimitiveProducer_cfg.py to rerun L1 CSC trigger emualution, which is a part of L1 trigger emulation.
 ```
 cmsRun runCSCTriggerPrimitiveProducer_cfg.py mc=True run3=True inputFiles="file:step2.root"
 ```
  
 ### Pileup mixing 
 Here is the twiki to explain the pileup and how to mix pileups with events: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFastSimPileUp
 
 The cmsDriver command to generate sample with Run3 PU settings in the DIGI step:
 ```
 cmsDriver.py step2 --conditions auto:phase1_2022_realistic --datatier GEN-SIM-DIGI --era Run3 \
 --eventcontent FEVTDEBUGHLT --filein file:step1.root --fileout file:step2_PU.root \
 --geometry DB:Extended --nStreams 2 --nThreads 8 --no_exec --number 10 \
 --pileup Run3_Flat55To75_PoissonOOTPU \
 --pileup_input das:/RelValMinBias_14TeV/CMSSW_12_2_0_pre3-122X_mcRun3_2021_realistic_v5-v1/GEN-SIM \
 --python_filename step_2_Run3PU_cfg.py --step DIGI:pdigi_valid,L1
 ```
  - pileup configuration is Run3_Flat55To75_PoissonOOTPU
  - datapath for pileup samples (also called minibias sample): /RelValMinBias_14TeV/CMSSW_12_2_0_pre3-122X_mcRun3_2021_realistic_v5-v1/GEN-SIM
  - the cmsRun configuration file is step_2_Run3PU_cfg.py and the output file from this configuration is step2_PU.root
  
 The minibias sample can be found on [CMSDAS](https://cmsweb.cern.ch/das/) by searching the key word like "dataset= /RelValMinBias_14TeV/CMSSW_12*mcRun3*/GEN-SIM" and here is the link for [avaialble run3 minibsas samples produced through CMS RelVal](https://cmsweb.cern.ch/das/request?input=dataset%3D+%2FRelValMinBias_14TeV%2FCMSSW_12%2AmcRun3%2A%2FGEN-SIM&instance=prod/global&idx=50&limit=50)
 
 Also keep in mind that running the step2 configuration with PU would take much longer time than with the one without PU as mixing PU events requires much more memory and CPU time. 
 
 
 ### Batch job submission with Crab
 Crab is the system to submit jobs to remote sites to produce the huge amount samples.  The twikis for crab are https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3ConfigurationFile and https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookCRAB3Tutorial.
 
 We had two example crab configuraitons, one for step1 and one for step2, under python/ directory.  crab_step1_GENSIM_cfg.py is used to generate GEN-SIM samples using step1 cmsRun configuration and 'PrivateMC' is used for jobtype. No input data path for step1 as it is genarating particles from genearator. crab_step2_DIGI_L1.py is used to generate digi+L1 samples using step2 cmsRun configuration. The step2 crab configuration need to take the published data path from step1 crab job. 
 
   
### Others
Edm command is used to check what is inside sample root file:
```
edmEventSize -a -v step2.root
```


## CSC event display
The package used for CSC event display is here: https://github.com/tahuang1991/gifDisplay

CSC event display requires the run number (which is always 1 for MC sample), event number and chamber number as inputs
and then produces display plots. 

example to display CSC chamber ME+1/1/34 for run1 and event 2002 is to add 
>```
> 1 2002 1 1 1 34
>```
to the eventList.txt file and then run script
>```
> ./runDisplay.sh
>```

## Twiki for documenting the samples

https://github.com/tahuang1991/InjectCSCPatterns/wiki/Twiki-for-Muon-Samples
