import FWCore.ParameterSet.Config as cms
import os

## default location of high-PU filelist
path = os.getenv( "CMSSW_BASE" ) + "/src/GEMCode/SimMuL1/test/"
def_filelist = path + "filelist_minbias_61M_good.txt"

def addPileUpRun3(process):
    ## with default pu setting in the configuraiton
    process.mix.input.fileNames = cms.untracked.vstring(['/store/relval/CMSSW_12_2_0_pre3/RelValMinBias_14TeV/GEN-SIM/122X_mcRun3_2021_realistic_v5-v1/2580000/2ac335b2-dfa1-4854-bbfc-5e2ab9ae1f5f.root', '/store/relval/CMSSW_12_2_0_pre3/RelValMinBias_14TeV/GEN-SIM/122X_mcRun3_2021_realistic_v5-v1/2580000/2e3043dd-57fa-4f76-bada-baf6d7d1aa24.root', '/store/relval/CMSSW_12_2_0_pre3/RelValMinBias_14TeV/GEN-SIM/122X_mcRun3_2021_realistic_v5-v1/2580000/44df08a8-232e-4664-a6dd-fbba971819f8.root', '/store/relval/CMSSW_12_2_0_pre3/RelValMinBias_14TeV/GEN-SIM/122X_mcRun3_2021_realistic_v5-v1/2580000/5bcd2054-dc63-411f-8a8d-0bf8a85ea7b8.root', '/store/relval/CMSSW_12_2_0_pre3/RelValMinBias_14TeV/GEN-SIM/122X_mcRun3_2021_realistic_v5-v1/2580000/a26908fb-aa1c-4afd-afb3-d07d0ec4e835.root', '/store/relval/CMSSW_12_2_0_pre3/RelValMinBias_14TeV/GEN-SIM/122X_mcRun3_2021_realistic_v5-v1/2580000/bd4d2205-c3e9-4990-9848-539b1cc65f3e.root', '/store/relval/CMSSW_12_2_0_pre3/RelValMinBias_14TeV/GEN-SIM/122X_mcRun3_2021_realistic_v5-v1/2580000/de1790dc-cefe-4f25-93f9-6104e1a7a053.root'])
    process.mix.digitizers = cms.PSet(process.theDigitizersValid)
    return process

## helper function for pile-up from local filelist
def addPileUpLocal(process, pu = 140, filelist = def_filelist):
    ff = open(filelist, "r")
    pu_files = ff.read().split('\n')
    ff.close()
    pu_files = filter(lambda x: x.endswith('.root'),  pu_files)

    process.mix.input = cms.SecSource("PoolSource",
        nbPileupEvents = cms.PSet(
            averageNumber = cms.double(pu)
        ),
        type = cms.string('poisson'),
        sequential = cms.untracked.bool(False),
        fileNames = cms.untracked.vstring(*pu_files)
    )
    return process

## helper for files on dCache/EOS (LPC)
def useInputDir(process, inputDir):
    theInputFiles = []
    ## list the files in this directory
    output = os.popen('xrdfs root://cmseos.fnal.gov ls ' + inputDir).readlines()
    for x in output:
        stripx = x.strip()
        if stripx.endswith('root'):
            theInputFiles.extend(['file:root://cmsxrootd.fnal.gov/' + stripx])
    process.source.fileNames = cms.untracked.vstring(*theInputFiles)
    return process
