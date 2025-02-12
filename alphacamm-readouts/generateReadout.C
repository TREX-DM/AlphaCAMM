


int generateReadout (std::string fileName="AlphaCAMMReadout.root", std::string rmlName ="microbulkReadout.rml", std::string readoutName="alphaCAMMReadout") {

TFile *f = new TFile(fileName.c_str(),"UPDATE");
TRestDetectorReadout *r = new TRestDetectorReadout(rmlName.c_str());
r->Write(readoutName.c_str());
f->Close();

return 0;

}

