# AlphaCAMM

Implements all the necesary submodules for AlphaCAMM simulations and analysis. To clone this proyect do not forget to append `--recusive`
```
git clone git@github.com:trex-dm/AlphaCAMM.git --recursive
```

## Simulations

Implement AlphaCAMM geometry and provides different rml files to perform different simulations:

- 210PoFromSample.rml: Simulate alphas stemming from 210Po from the area in which the sample is located.
- 222RnFromGas.rml: Simulate alphas stemming from 222Rn inside the sensitive volume, e.g. volumetric contamination of 222Rn.
- 218PoFromMylar.rml: Simulate alphas stemming 218Po inside the mylar volume.
- 214PoFromMylar.rml: Same as before but for the 214Po isotope.
- 241AmFromSource.rml: Simulate 241Am source in the middle of the cathode.

## Analysis

Provides different rml files for AlphaCAMM analysis:

- g4ToSignal.rml: Transform alphaCAMM G4 events to rawSignal events while performing the electron diffusion and shapping of the signal. It requires Garfield to retrieve gas properties.
- rawToSignal.rml: Transform raw events to signal events using new TREXDM Micromegas readouts.
- alphaAna.rml: Performs the analysis of alpha tracks using a novel track linearization method. A rawSignalEvent is used as input, however at this stage simulated or real data can be used.

### Plots

Provides the tools to generate some plots after performing alphaCAMM analysis:

- summary.rml: Provides a summary of the AlphaCAMM analysis.
- trackVis.rml: Provides visualization of alphaTracks in which the origin and end of the track is shown

## Readouts

Provides the tools to create AlphaCAMM readout. Current readout is based on new TREXDM Micromegas readouts because AlphaCAMM is meant to use this kind of readouts. However the decoding has to be changed while moving to the AFTER electronics.


## Instructions

You just have to define REST_DATA_PATH variable e.g.:
export REST_DATA_PATH="~/mydataPath"

Afterwards you can run analysis/simulations using standard REST programs.




** WARNING: REST is under continous development.** This README is offered to you by the REST community. Your HELP is needed to keep this file up to date. You are very welcome to contribute fixing typos, updating information or adding new contributions. See also our [Contribution Guide](https://lfna.unizar.es/rest-development/REST_v2/-/blob/master/CONTRIBUTING.md).
