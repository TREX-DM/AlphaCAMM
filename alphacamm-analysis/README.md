# alphacamm-analysis

Implement AlphaCAMM analysis by implementing different rml files to perform AlphaCAMM analysis:

- g4ToSignal.rml: Transform alphaCAMM G4 events to rawSignal events while performing the electron diffusion and shapping of the signal. It requires Garfield to retrieve gas properties.
- rawToSignal.rml: Transform raw events to signal events using new TREXDM Micromegas readouts.
- alphaAna.rml: Performs the analysis of alpha tracks using a novel track linearization method. A rawSignalEvent is used as input, however at this stage simulated or real data can be used.

#### Running analysis

You shoud define a data path changing the mainDataPath under `common/globals.xml` or defining a global variable named `REST_DATA_PATH` e.g. `export REST_DATA_PATH=~/mydataPath/`. Afterwards, just use `restManager` to launch the desired analysis e.g.:
```
restManager --c g4ToSignal.rml
```

## Generating plots

Provides the tools to generate some plots after performing alphaCAMM analysis:

- summary.rml: Provides a summary of the AlphaCAMM analysis.
- trackVis.rml: Provides visualization of alphaTracks in which the origin and end of the track is shown

**? WARNING: REST is under continous development.** This README is offered to you by the REST community. Your HELP is needed to keep this file up to date. You are very welcome to contribute fixing typos, updating information or adding new contributions. See also our [Contribution Guide](https://lfna.unizar.es/rest-development/REST_v2/-/blob/master/CONTRIBUTING.md).


