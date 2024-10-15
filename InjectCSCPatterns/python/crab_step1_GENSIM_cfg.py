from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'ste1_GENSIM'
config.General.workArea = 'crab_projects'
config.General.transferLogs=False
config.section_("JobType")
config.JobType.pluginName = 'PrivateMC'
config.JobType.psetName = 'step_1_cfg.py'
#config.JobType.scriptExe = ''
config.JobType.inputFiles = [] #hadd nano will not be needed once nano tools are in cmssw
config.JobType.sendPythonFolder	 = True
config.JobType.allowUndistributedCMSSW = True
config.JobType.maxMemoryMB = 4000
config.JobType.numCores = 8
config.section_("Data")
config.Data.inputDataset = None
##event based splitting for step1 
config.Data.splitting = "EventBased"
config.Data.unitsPerJob = 1000
config.Data.totalUnits = 5000
config.Data.publication = True
config.Data.outputDatasetTag = config.General.requestName
username = "tahuang" ## replaced with your lxplus user name
config.Data.outLFNDirBase = '/store/user/%s/MuonSampleProduction/%s/'%(username, config.General.requestName)
config.section_("Site")
config.Site.storageSite = "T3_CH_CERNBOX" ## or T3_US_FNALLPC

