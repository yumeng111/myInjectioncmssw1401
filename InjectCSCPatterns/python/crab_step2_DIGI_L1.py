from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'step2_DIGI_L1'
config.General.workArea = 'crab_projects'
config.General.transferLogs=False
config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'step_2_cfg.py'
#config.JobType.scriptExe = ''
config.JobType.inputFiles = [] #hadd nano will not be needed once nano tools are in cmssw
config.JobType.sendPythonFolder	 = True
config.JobType.allowUndistributedCMSSW = True
config.JobType.maxMemoryMB = 4000
config.section_("Data")
#config.Data.inputDataset = '/RelValSingleMuPt10/CMSSW_12_4_0_pre3-123X_mcRun3_2021_realistic_v14-v1/GEN-SIM-DIGI-RAW'
#config.Data.inputDBS = 'global'
config.Data.inputDataset = 'Replaced_with_publish_datapath_from_step1_crabjob'
config.Data.inputDBS='phys03'
config.Data.splitting = 'FileBased'
#config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 1
#config.Data.unitsPerJob = 100
#config.Data.totalUnits = 2000
config.Data.publication = True
config.Data.outputDatasetTag = config.General.requestName
username = "tahuang" ## replaced with your lxplus user name
config.Data.outLFNDirBase = '/store/user/%s/CSCEmulation/%s/'%(username, config.General.requestName)
config.section_("Site")
config.Site.storageSite = "T3_CH_CERNBOX" ## or T3_US_FNALLPC

