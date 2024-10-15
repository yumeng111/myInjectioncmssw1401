import FWCore.ParameterSet.Config as cms

def randomizeMuonGunEndcap(process):
    ## filter for gen level
    process.genMuons = cms.EDFilter("PdgIdCandViewSelector",
        src = cms.InputTag("genParticles"),
        pdgId = cms.vint32(13, -13)
    )

    ## filter for sim level
    process.genMuonsEndcap = cms.EDFilter("CandViewSelector",
        src = cms.InputTag("genMuons"),
        ##endcap muon filter: 0.9<abs(eta)<2.8
        ##if ME0 is not used, then 0.9<abs(eta)<2.5
        ##ring=1 only, then 1.6<abs(eta)<2.5
        cut = cms.string("pt > 0.5 & abs(eta)<2.5 & abs(eta)>0.9"),   
        filter = cms.bool(True)
    )

    ## filter sequence
    process.gen_mu_select = cms.Sequence(process.genMuons * process.genMuonsEndcap)
    process.generation_step += process.gen_mu_select
    process.simulation_step += process.gen_mu_select
    return process
