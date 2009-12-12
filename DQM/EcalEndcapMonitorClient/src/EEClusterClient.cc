/*
 * \file EEClusterClient.cc
 *
 * $Date: 2009/10/28 08:18:23 $
 * $Revision: 1.60 $
 * \author G. Della Ricca
 * \author E. Di Marco
 *
*/

#include <memory>
#include <iostream>
#include <fstream>

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DQMServices/Core/interface/DQMStore.h"

#include "DQM/EcalCommon/interface/UtilsClient.h"
#include "DQM/EcalCommon/interface/Numbers.h"

#include <DQM/EcalEndcapMonitorClient/interface/EEClusterClient.h>

using namespace cms;
using namespace edm;
using namespace std;

EEClusterClient::EEClusterClient(const ParameterSet& ps) {

  // cloneME switch
  cloneME_ = ps.getUntrackedParameter<bool>("cloneME", true);

  // verbose switch
  verbose_ = ps.getUntrackedParameter<bool>("verbose", true);

  // debug switch
  debug_ = ps.getUntrackedParameter<bool>("debug", false);

  // prefixME path
  prefixME_ = ps.getUntrackedParameter<string>("prefixME", "");

  // enableCleanup_ switch
  enableCleanup_ = ps.getUntrackedParameter<bool>("enableCleanup", false);

  // vector of selected Super Modules (Defaults to all 18).
  superModules_.reserve(18);
  for ( unsigned int i = 1; i <= 18; i++ ) superModules_.push_back(i);
  superModules_ = ps.getUntrackedParameter<vector<int> >("superModules", superModules_);

  h01_[0] = 0;
  h01_[1] = 0;
  h01_[2] = 0;

  for(int iEE=0;iEE<2;iEE++) {
    for(int i=0;i<3;++i) {
      h04_[i][iEE] = 0;
      h02ProjEta_[i][iEE] = 0;
      h02ProjPhi_[i][iEE] = 0;
    }
  }

  h03_[0] = 0;
  h03ProjEta_[0] = 0;
  h03ProjPhi_[0] = 0;

  h03_[1] = 0;
  h03ProjEta_[1] = 0;
  h03ProjPhi_[1] = 0;

  i01_[0] = 0;
  i01_[1] = 0;
  i01_[2] = 0;

  s01_[0] = 0;
  s01_[1] = 0;
  s01_[2] = 0;

}

EEClusterClient::~EEClusterClient() {

}

void EEClusterClient::beginJob(void) {

  dqmStore_ = Service<DQMStore>().operator->();

  if ( debug_ ) cout << "EEClusterClient: beginJob" << endl;

  ievt_ = 0;
  jevt_ = 0;

}

void EEClusterClient::beginRun(void) {

  if ( debug_ ) cout << "EEClusterClient: beginRun" << endl;

  jevt_ = 0;

  this->setup();

}

void EEClusterClient::endJob(void) {

  if ( debug_ ) cout << "EEClusterClient: endJob, ievt = " << ievt_ << endl;

  this->cleanup();

}

void EEClusterClient::endRun(void) {

  if ( debug_ ) cout << "EEClusterClient: endRun, jevt = " << jevt_ << endl;

  this->cleanup();

}

void EEClusterClient::setup(void) {

  dqmStore_->setCurrentFolder( prefixME_ + "/EEClusterClient" );

}

void EEClusterClient::cleanup(void) {

  if ( ! enableCleanup_ ) return;

  if ( cloneME_ ) {
    if ( h01_[0] ) delete h01_[0];
    if ( h01_[1] ) delete h01_[1];
    if ( h01_[2] ) delete h01_[2];

    for(int iEE=0;iEE<2;iEE++) {
      for(int i=0;i<3;++i) {
        if(h04_[i][iEE]) delete h04_[i][iEE];
        if(h02ProjEta_[i][iEE]) delete h02ProjEta_[i][iEE];
        if(h02ProjPhi_[i][iEE]) delete h02ProjPhi_[i][iEE];
      }
    }

    if(h03_[0]) delete h03_[0];
    if(h03ProjEta_[0]) delete h03ProjEta_[0];
    if(h03ProjPhi_[0]) delete h03ProjPhi_[0];

    if(h03_[1]) delete h03_[1];
    if(h03ProjEta_[1]) delete h03ProjEta_[1];
    if(h03ProjPhi_[1]) delete h03ProjPhi_[1];

    if(i01_[0]) delete i01_[0];
    if(i01_[1]) delete i01_[1];
    if(i01_[2]) delete i01_[2];

    if(s01_[0]) delete s01_[0];
    if(s01_[1]) delete s01_[1];
    if(s01_[2]) delete s01_[2];

  }

  h01_[0] = 0;
  h01_[1] = 0;
  h01_[2] = 0;

  for(int iEE=0;iEE<2;iEE++) {
    for(int i=0;i<3;++i) {
      h04_[i][iEE] = 0;
      h02ProjEta_[i][iEE] = 0;
      h02ProjPhi_[i][iEE] = 0;
    }
  }

  h03_[0] = 0;
  h03ProjEta_[0] = 0;
  h03ProjPhi_[0] = 0;

  h03_[1] = 0;
  h03ProjEta_[1] = 0;
  h03ProjPhi_[1] = 0;

  i01_[0] = 0;
  i01_[1] = 0;
  i01_[2] = 0;

  s01_[0] = 0;
  s01_[1] = 0;
  s01_[2] = 0;

}

bool EEClusterClient::writeDb(EcalCondDBInterface* econn, RunIOV* runiov, MonRunIOV* moniov, bool& status) {

  status = true;

  return true;

}

void EEClusterClient::analyze(void) {

  ievt_++;
  jevt_++;
  if ( ievt_ % 10 == 0 ) {
    if ( debug_ ) cout << "EEClusterClient: ievt/jevt = " << ievt_ << "/" << jevt_ << endl;
  }

  char histo[200];

  MonitorElement* me;

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC energy").c_str());
  me = dqmStore_->get(histo);
  h01_[0] = UtilsClient::getHisto<TH1F*>( me, cloneME_, h01_[0] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC size").c_str());
  me = dqmStore_->get(histo);
  h01_[1] = UtilsClient::getHisto<TH1F*>( me, cloneME_, h01_[1] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC number").c_str());
  me = dqmStore_->get(histo);
  h01_[2] = UtilsClient::getHisto<TH1F*>( me, cloneME_, h01_[2] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC energy map EE -").c_str());
  me = dqmStore_->get(histo);
  h04_[0][0] = UtilsClient::getHisto<TProfile2D*>( me, cloneME_, h04_[0][0] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC number map EE -").c_str());
  me = dqmStore_->get(histo);
  h03_[0] = UtilsClient::getHisto<TH2F*>( me, cloneME_, h03_[0] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC ET map EE -").c_str());
  me = dqmStore_->get(histo);
  h04_[1][0] = UtilsClient::getHisto<TProfile2D*>( me, cloneME_, h04_[1][0] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC size map EE -").c_str());
  me = dqmStore_->get(histo);
  h04_[2][0] = UtilsClient::getHisto<TProfile2D*>( me, cloneME_, h04_[2][0] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC energy projection eta EE -").c_str());
  me = dqmStore_->get(histo);
  h02ProjEta_[0][0] = UtilsClient::getHisto<TProfile*>( me, cloneME_, h02ProjEta_[0][0] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC energy projection phi EE -").c_str());
  me = dqmStore_->get(histo);
  h02ProjPhi_[0][0] = UtilsClient::getHisto<TProfile*>( me, cloneME_, h02ProjPhi_[0][0] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC number projection eta EE -").c_str());
  me = dqmStore_->get(histo);
  h03ProjEta_[0] = UtilsClient::getHisto<TH1F*>( me, cloneME_, h03ProjEta_[0] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC number projection phi EE -").c_str());
  me = dqmStore_->get(histo);
  h03ProjPhi_[0] = UtilsClient::getHisto<TH1F*>( me, cloneME_, h03ProjPhi_[0] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC ET projection eta EE -").c_str());
  me = dqmStore_->get(histo);
  h02ProjEta_[1][0] = UtilsClient::getHisto<TProfile*>( me, cloneME_, h02ProjEta_[1][0] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC ET projection phi EE -").c_str());
  me = dqmStore_->get(histo);
  h02ProjPhi_[1][0] = UtilsClient::getHisto<TProfile*>( me, cloneME_, h02ProjPhi_[1][0] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC size projection eta EE -").c_str());
  me = dqmStore_->get(histo);
  h02ProjEta_[2][0] = UtilsClient::getHisto<TProfile*>( me, cloneME_, h02ProjEta_[2][0] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC size projection phi EE -").c_str());
  me = dqmStore_->get(histo);
  h02ProjPhi_[2][0] = UtilsClient::getHisto<TProfile*>( me, cloneME_, h02ProjPhi_[2][0] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC energy map EE +").c_str());
  me = dqmStore_->get(histo);
  h04_[0][1] = UtilsClient::getHisto<TProfile2D*>( me, cloneME_, h04_[0][1] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC number map EE +").c_str());
  me = dqmStore_->get(histo);
  h03_[1] = UtilsClient::getHisto<TH2F*>( me, cloneME_, h03_[1] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC ET map EE +").c_str());
  me = dqmStore_->get(histo);
  h04_[1][1] = UtilsClient::getHisto<TProfile2D*>( me, cloneME_, h04_[1][1] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC size map EE +").c_str());
  me = dqmStore_->get(histo);
  h04_[2][1] = UtilsClient::getHisto<TProfile2D*>( me, cloneME_, h04_[2][1] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC energy projection eta EE +").c_str());
  me = dqmStore_->get(histo);
  h02ProjEta_[0][1] = UtilsClient::getHisto<TProfile*>( me, cloneME_, h02ProjEta_[0][1] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC energy projection phi EE +").c_str());
  me = dqmStore_->get(histo);
  h02ProjPhi_[0][1] = UtilsClient::getHisto<TProfile*>( me, cloneME_, h02ProjPhi_[0][1] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC number projection eta EE +").c_str());
  me = dqmStore_->get(histo);
  h03ProjEta_[1] = UtilsClient::getHisto<TH1F*>( me, cloneME_, h03ProjEta_[1] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC number projection phi EE +").c_str());
  me = dqmStore_->get(histo);
  h03ProjPhi_[1] = UtilsClient::getHisto<TH1F*>( me, cloneME_, h03ProjPhi_[1] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC ET projection eta EE +").c_str());
  me = dqmStore_->get(histo);
  h02ProjEta_[1][1] = UtilsClient::getHisto<TProfile*>( me, cloneME_, h02ProjEta_[1][1] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC ET projection phi EE +").c_str());
  me = dqmStore_->get(histo);
  h02ProjPhi_[1][1] = UtilsClient::getHisto<TProfile*>( me, cloneME_, h02ProjPhi_[1][1] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC size projection eta EE +").c_str());
  me = dqmStore_->get(histo);
  h02ProjEta_[2][1] = UtilsClient::getHisto<TProfile*>( me, cloneME_, h02ProjEta_[2][1] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT BC size projection phi EE +").c_str());
  me = dqmStore_->get(histo);
  h02ProjPhi_[2][1] = UtilsClient::getHisto<TProfile*>( me, cloneME_, h02ProjPhi_[2][1] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT SC energy").c_str());
  me = dqmStore_->get(histo);
  i01_[0] = UtilsClient::getHisto<TH1F*>( me, cloneME_, i01_[0] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT SC size").c_str());
  me = dqmStore_->get(histo);
  i01_[1] = UtilsClient::getHisto<TH1F*>( me, cloneME_, i01_[1] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT SC number").c_str());
  me = dqmStore_->get(histo);
  i01_[2] = UtilsClient::getHisto<TH1F*>( me, cloneME_, i01_[2] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT s1s9").c_str());
  me = dqmStore_->get(histo);
  s01_[0] = UtilsClient::getHisto<TH1F*>( me, cloneME_, s01_[0] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT s9s25").c_str());
  me = dqmStore_->get(histo);
  s01_[1] = UtilsClient::getHisto<TH1F*>( me, cloneME_, s01_[1] );

  sprintf(histo, (prefixME_ + "/EEClusterTask/EECLT dicluster invariant mass Pi0").c_str());
  me = dqmStore_->get(histo);
  s01_[2] = UtilsClient::getHisto<TH1F*>( me, cloneME_, s01_[2] );

}

