
#include <TRestTrackLineAnalysisProcess.h>
#include <TRestGeant4Event.h>
#include <TRestAnalysisTree.h>

int CompareSim(const std::string &fileName){

  TRestRun* run = new TRestRun(fileName);

  run->PrintMetadata();

  TRestTrackLineAnalysisProcess *trackAlphaAna = (TRestTrackLineAnalysisProcess *)run->GetMetadataClass("TRestTrackLineAnalysisProcess");

      if(trackAlphaAna == nullptr){
        std::cout<<"ERROR: No TRestTrackLineAnalysisProcess found in "<<fileName<<" please, provide a valid file"<<std::endl;
        return -1;
      }

  TRestEvent* g4Event = REST_Reflection::Assembly("TRestGeant4Event");
  run->SetInputEvent(g4Event);
  

  TRestAnalysisTree* anaTree = run->GetAnalysisTree();
  int n = anaTree->GetNbranches();

  TH1F *origDiffX = new TH1F ("origDiffX","origDiffX",100,-10,10);
  TH1F *origDiffY = new TH1F ("origDiffY","origDiffY",100,-10,10);
  TH1F *origDiff = new TH1F ("origDiff","origDiff",100,0,10);
  TH1F *endDiffX = new TH1F ("endDiffX","endDiffX",100,-10,10);
  TH1F *endDiffY = new TH1F ("endDiffY","endDiffY",100,-10,10);
  TH1F *endDiff = new TH1F ("endDiff","endDiff",100,0,10);

  TH1F *angDiff = new TH1F ("angDiff","angDiff",500,-3.1416,3.1416);
  TH1F *ang = new TH1F ("ang","ang",100,0,3.1416);
  TH1F *ang2 = new TH1F ("ang2","ang2",100,0,3.1416);

  std::cout<<"Entries "<<anaTree->GetEntries()<<std::endl;

    for(int i=0;i<run->GetEntries();i++){
      run->GetEntry(i);
      
      std::vector <double> orig;
      std::vector<std::string> oName = {"alphaTrackAna_originX","alphaTrackAna_originY","alphaTrackAna_originZ"}; 
      for (const auto &v : oName){
        if(anaTree->GetObservableID(v) ==-1){
          std::cout<<"Warning observable "<< v <<"not found"<<std::endl;
          return -1;
        }
        double val = anaTree->GetObservableValue<Double_t>(v);
        orig.emplace_back(val);
      }

      TVector3 anaOrigin = orig.data();

      TVector3 g4Origin = static_cast<TRestGeant4Event *> (g4Event)->GetFirstPositionInVolume(0);

      double dX = anaOrigin.X() - g4Origin.X();
      double dY = anaOrigin.Y() - g4Origin.Y();
      double dist = TMath::Sqrt(dX*dX+dY*dY);
      origDiffX->Fill(dX);
      origDiffY->Fill(dY);
      origDiff->Fill(dist);

      std::vector <double> end;
      std::vector<std::string> eName = {"alphaTrackAna_endX","alphaTrackAna_endY","alphaTrackAna_endZ"}; 
      for (const auto &v : eName){
        if(anaTree->GetObservableID(v) ==-1){
          std::cout<<"Warning observable "<< v <<"not found"<<std::endl;
          return -1;
        }
        double val = anaTree->GetObservableValue<Double_t>(v);
        end.emplace_back(val);
      }

      TVector3 anaEnd = orig.data();

      TVector3 g4End = static_cast<TRestGeant4Event *> (g4Event)->GetLastPositionInVolume(0);

      dX = anaEnd.X() - g4End.X();
      dY = anaEnd.Y() - g4End.Y();
      dist = TMath::Sqrt(dX*dX+dY*dY);
      endDiffX->Fill(dX);
      endDiffY->Fill(dY);
      endDiff->Fill(dist);

      double angle2 = TMath::Pi() - anaTree->GetObservableValue<Double_t>("g4Ana_thetaPrimary");
      double angle = anaTree->GetObservableValue<Double_t>("alphaTrackAna_angle");

      angDiff->Fill(angle-angle2);
      ang2->Fill(angle2);
      ang->Fill(angle);

    }

   TCanvas *origDiffXCanvas = new TCanvas("origDiffXCanvas", "origDiffXCanvas");
   origDiffX->Draw();
   TCanvas *origDiffYCanvas = new TCanvas("origDiffYCanvas", "origDiffYCanvas");
   origDiffY->Draw();
   TCanvas *origDiffCanvas = new TCanvas("origDiffCanvas", "origDiffCanvas");
   origDiff->Draw();

   TCanvas *endDiffXCanvas = new TCanvas("endDiffXCanvas", "endDiffXCanvas");
   endDiffX->Draw();
   TCanvas *endDiffYCanvas = new TCanvas("endDiffYCanvas", "endDiffYCanvas");
   endDiffY->Draw();
   TCanvas *endDiffCanvas = new TCanvas("endDiffCanvas", "endDiffCanvas");
   endDiff->Draw();

   TCanvas *angleCanvas = new TCanvas("angleCanvas", "angleCanvas");
   ang->Draw();
   ang2->SetLineColor(kRed);
   ang2->Draw("SAME");

   TCanvas *angleDiffCanvas = new TCanvas("angleDiffCanvas", "angleDiffCanvas");
   angDiff->Draw();

  return 0;

}
