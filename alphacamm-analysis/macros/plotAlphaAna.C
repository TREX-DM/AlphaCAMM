//
//
//
#include <TRestRun.h>
#include <TRestTrackLineAnalysisProcess.h>
#include <TRestDetectorReadout.h>
#include <TRestTrackEvent.h>
#include <TRestAnalysisTree.h>
#include <TRestRawSignalEvent.h>

#include "dirent.h"

TPad *DrawPulses ( TRestRawSignalEvent *rawSignalEvent, TRestDetectorReadout *fReadout) {

  TRestDetectorReadoutPlane* plane = &(*fReadout)[0];

  std::vector<Int_t> sXIDs;
  std::vector<Int_t> sYIDs;

  Int_t numberOfSignals = rawSignalEvent->GetNumberOfSignals();
  Int_t readoutChannel = -1, readoutModule, planeID=0;
  int maxX=0, maxY=0;
    for (int i = 0; i < numberOfSignals; i++) {
      TRestRawSignal* sgnl = rawSignalEvent->GetSignal(i);
      Int_t signalID = sgnl->GetSignalID();
      //if(sgnl->GetMaxValue()<100)continue;
      fReadout->GetPlaneModuleChannel(signalID, planeID, readoutModule, readoutChannel);
      if(readoutChannel == -1)continue;
      Double_t x = plane->GetX(readoutModule, readoutChannel);
      Double_t y = plane->GetY(readoutModule, readoutChannel);
        if (TMath::IsNaN(x)) {
          sYIDs.emplace_back(signalID);
        } else if (TMath::IsNaN(y)) {
          sXIDs.emplace_back(signalID);
        }
    }

  TPad *fPad = new TPad("RawSignalEvent", " ", 0, 0, 1, 1);
  fPad->Divide(2);
  fPad->Draw();
  fPad->cd(1);
  rawSignalEvent->DrawSignals((TPad*)fPad->cd(1),sXIDs);
  fPad->Update();
  fPad->cd(2);
  rawSignalEvent->DrawSignals((TPad*)fPad->cd(2),sYIDs);
  fPad->Update();

 return fPad;

}

std::vector <string> getFiles(const std::string &fileName){

  std::vector <string> files;
  std::string fName=fileName;
  if(fName.back() == '*')fName.pop_back();

  size_t found = fName.find_last_of("/\\");
  std::string path = "./";
  std::string file = fName;
    if(found != std::string::npos){
      path = fName.substr(0,found+1);
      fName = fName.substr(found+1);
    }

  DIR *directory = opendir(path.c_str());
  struct dirent *direntStruct;

    if (directory != NULL) {
        while ( (direntStruct = readdir(directory)) ) {
          std::string fCheck = direntStruct->d_name;
          if(fCheck.find(fName)==0)files.push_back(path+fCheck);
        }
    } else return files;
  closedir(directory);

  std::sort(files.begin(),files.end());

  return files;

}


void UpdateRate(const double& currentTimeEv, double& oldTime, const int& currentEventCount, int& oldEventCount, int& rateGraphCounter, TGraphErrors* rateGraph, bool lastPoint = false) {

  if(!lastPoint)
    if ( currentTimeEv - oldTime < 3600 ) return;

  double instantRate = (currentEventCount - oldEventCount) / (currentTimeEv - oldTime);
  double instantRateError = TMath::Sqrt(currentEventCount - oldEventCount) / (currentTimeEv - oldTime);
  double rootTime = 2871763200 + (currentTimeEv + oldTime) / 2.;
  
  rateGraph->SetPoint(rateGraphCounter, rootTime, instantRate);
  
  rateGraph->SetPointError (rateGraphCounter, (currentTimeEv - oldTime) / 2.,instantRateError);

  oldEventCount = currentEventCount;
  oldTime = currentTimeEv;
  rateGraphCounter++;

}

int plotAlphaAna(const std::string &fileName, bool interactive = false){

  auto files = getFiles(fileName);
    if(files.empty()){
      std::cout<<"No files found "<<fileName<<" check that the file(s) exist"<<endl;
      return -1;
    }

  TH1F *spectraD = nullptr,*spectraU = nullptr, *angle = nullptr;
  TH2F *endH = nullptr, *origH = nullptr,*trackL = nullptr;

  TCanvas *hitmapCanvas = nullptr, *projectionCanvas = nullptr, *signalCanvas = nullptr;

  TCanvas *spectraCanvas = new TCanvas("spectraCanvas", "spectraCanvas");
  TCanvas *originCanvas = new TCanvas("originCanvas", "originCanvas");
  TCanvas *endCanvas = new TCanvas("endCanvas", "endCanvas");
  TCanvas *trackLengthCanvas = new TCanvas("trackLengthCanvas", "trackLengthCanvas");
  TCanvas *trackAngleCanvas = new TCanvas("trackAngleCanvas", "trackAngleCanvas");
  
  if(interactive){
    hitmapCanvas = new TCanvas("hitmapCanvas", "hitmapCanvas");
    projectionCanvas = new TCanvas("projectionCanvas", "projectionCanvas");
    signalCanvas = new TCanvas("signalCanvas", "signalCanvas");
  }

   TLegend *legOrEnd = nullptr;

  double oldTimeEv =0,duration=0,lastTimeStamp=0;
  int currentEventCount=0,oldEventCount=0,rateGraphCounter=0;
  TGraphErrors* rateGraph = new TGraphErrors();


  int c=0;

  for(auto f:files){

    TRestRun* run = new TRestRun(f);

    if(c==0){
      run->PrintMetadata();
      oldTimeEv = run->GetStartTimestamp();
    }

    TRestTrackLineAnalysisProcess *trackAlphaAna = (TRestTrackLineAnalysisProcess *)run->GetMetadataClass("TRestTrackLineAnalysisProcess");

      if(trackAlphaAna == nullptr){
        std::cout<<"ERROR: No TRestTrackLineAnalysisProcess found in "<<f<<" please, provide a valid file"<<std::endl;
        return -1;
      }

    TRestAnalysisTree* anaTree = run->GetAnalysisTree();
    int n = anaTree->GetNbranches();

    std::cout<<"Entries "<<anaTree->GetEntries()<<std::endl;

    std::string planeStr ="";

    TRestDetectorReadout *fReadout = (TRestDetectorReadout* ) run->GetMetadataClass("TRestDetectorReadout");

    double xmin=0,xmax=0,ymin=0,ymax=0;
      if(fReadout) {
        TRestDetectorReadoutPlane* plane = &(*fReadout)[0];
        plane->GetBoundaries(xmin, xmax, ymin, ymax);
        planeStr = std::to_string((int)std::abs(xmax-xmin))+","+std::to_string(xmin)+","+std::to_string(xmax)+","
        +std::to_string((int)std::abs(ymax-ymin))+","+std::to_string(ymin)+","+std::to_string(ymax);
      }

    spectraCanvas->cd();
    anaTree->Draw("alphaTrackAna_totalEnergy>>sD(200,0,1000000)","alphaTrackAna_downwards");
    TH1F *sD = (TH1F *)gDirectory->Get("sD");
    if(spectraD==nullptr)spectraD=sD;
    else if (sD) spectraD->Add(sD);

    anaTree->Draw("alphaTrackAna_totalEnergy>>sU(200,0,1000000)","!alphaTrackAna_downwards");
    TH1F *sU = (TH1F *)gDirectory->Get("sU");
    if(spectraU==nullptr)spectraU=sU;
    else if (sU) spectraU->Add(sU);

    if(c==0)spectraU->SetLineColor(kRed);
    spectraD->Draw();
    spectraCanvas->cd();
    spectraU->Draw("SAME");
    spectraCanvas->Update();

    originCanvas->cd();
    std::string cmd = "alphaTrackAna_originY:alphaTrackAna_originX>>orig("+planeStr +")";
    anaTree->Draw(cmd.c_str());
    TH2F *orig = (TH2F*)gDirectory->Get("orig");
    if(origH==nullptr)origH=orig;
    else if (orig) origH->Add(orig);
    originCanvas->cd();
    origH->Draw("COLZ");
    originCanvas->Update();


    endCanvas->cd();
    cmd = "alphaTrackAna_endY:alphaTrackAna_endX>>end("+planeStr +")";
    anaTree->Draw(cmd.c_str());
    TH2F *end = (TH2F*)gDirectory->Get("end");
    if(endH==nullptr)endH=end;
    else if (end) endH->Add(end);
    endCanvas->cd();
    endH->Draw("COLZ");
    endCanvas->Update();

    trackLengthCanvas->cd();
    anaTree->Draw("alphaTrackAna_totalEnergy:alphaTrackAna_length>>tckL(100,0,100,100,0,1000000)");
    TH2F *tckL = (TH2F*)gDirectory->Get("tckL");
    if(trackL==nullptr)trackL=tckL;
    else if (tckL) trackL->Add(tckL);
    trackLengthCanvas->cd();
    trackL->Draw("COLZ");
    trackLengthCanvas->Update();

    trackAngleCanvas->cd();
    anaTree->Draw("alphaTrackAna_angle>>ang(100,0,3.1416)");
    TH1F *ang = (TH1F*)gDirectory->Get("ang");
    if(angle==nullptr)angle=ang;
    else if (ang) angle->Add(ang);
    trackAngleCanvas->cd();
    angle->Draw();
    trackAngleCanvas->Update();

    TRestEvent* trackEvent = REST_Reflection::Assembly("TRestTrackEvent");
    TRestEvent* rawSignalEvent = REST_Reflection::Assembly("TRestRawSignalEvent");

    run->SetInputEvent(trackEvent);
    run->SetInputEvent(rawSignalEvent);

    std::vector<TGraph *>  originGr {nullptr,nullptr};
    std::vector<TGraph *>  endGr {nullptr,nullptr};
    std::vector<TLegend *> legOrEnd {nullptr,nullptr};

    for(int i=0;i<run->GetEntries();i++){
      run->GetEntry(i);

      const double en = anaTree->GetObservableValue<Double_t>("totalEnergy");
      const double balance = anaTree->GetObservableValue<Double_t>("trackBalance");
      const double origX = anaTree->GetObservableValue<Double_t>("originX");
      const double origY = anaTree->GetObservableValue<Double_t>("originY");
      const double fid = (origX-128)*(origX-128) + (origY-128)*(origY-128);

      //if(  en <100000 || rad >10000)continue;

      UpdateRate(rawSignalEvent->GetTime(), oldTimeEv, currentEventCount, oldEventCount, rateGraphCounter, rateGraph);
      lastTimeStamp = rawSignalEvent->GetTime();
      currentEventCount++;

      if(!interactive)continue;

      std::cout<<"Event ID: "<<anaTree->GetEventID()<<std::endl;
      
      static_cast<TRestTrackEvent *>(trackEvent)->GetOriginEnd(originGr, endGr, legOrEnd);
      
      hitmapCanvas->cd();
      TPad *hitsPad = static_cast<TRestTrackEvent *>(trackEvent)->DrawHits();
      static_cast<TRestTrackEvent *>(trackEvent)->DrawOriginEnd(hitsPad, originGr, endGr, legOrEnd);
      hitmapCanvas->Update();

      projectionCanvas->cd();
      TPad *trackPad =static_cast<TRestTrackEvent *>(trackEvent)->DrawEvent();
      static_cast<TRestTrackEvent *>(trackEvent)->DrawOriginEnd(trackPad, originGr, endGr, legOrEnd);
      projectionCanvas->Update();

      signalCanvas->cd();
      DrawPulses( (TRestRawSignalEvent *)rawSignalEvent, fReadout);
      signalCanvas->Update();

      char str[200];
      bool ext = false;
      std::unique_ptr<TTimer> timer (new TTimer("gSystem->ProcessEvents();", 50, kFALSE) );
        do {
          std::cout<<"Type 'n' for next event or 'q' to quit interactive mode "<<std::endl;
          timer->TurnOn();
          timer->Reset();
          scanf("%s",str);
          timer->TurnOff();
          if(strcmp("n",str) == 0)ext=true;
          if(strcmp("q",str) == 0){ext=true;interactive=false;}
        } while (!ext);
      }

    duration += run->GetEndTimestamp() - run->GetStartTimestamp();
    std::cout<<f<<" Duration "<<run->GetEndTimestamp() - run->GetStartTimestamp()<<std::endl;
    c += run->GetEntries();
  }

  UpdateRate(lastTimeStamp, oldTimeEv, currentEventCount, oldEventCount, rateGraphCounter, rateGraph, true);
  TCanvas *rateCanvas = new TCanvas("rateCanvas", "rateCanvas");
  rateGraph->GetXaxis()->SetTimeDisplay(1);
  rateGraph->GetXaxis()->SetTitle("Date");
  rateGraph->GetYaxis()->SetTitle("Rate (Hz)");
  rateGraph->SetTitle("Rate");
  rateGraph->Draw("ALP");
  rateCanvas->Update();

  std::cout<<"Duration "<<duration/3600.<<" hours"<<std::endl;
  std::cout<<"Entries "<<currentEventCount<<" Downwards "<<spectraD->GetEntries()<<" Upwards "<<spectraU->GetEntries()<<std::endl;
  std::cout<<"Average rate "<<currentEventCount/duration<<" Hz "<<std::endl;

  return 0;

}
