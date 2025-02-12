# alphacamm-readouts

Implements AlphaCAMM readouts, different readout parameters and decoding files are implemented:

- microbulkReadout.rml/microbulkReadout.dec: implements AlphaCAMM readout based on new TREX-DM Micromegas which are meant to be used in AlphaCAMM. To update the decoding while using the AFTER electronics.
- bulkReadout.rml/bulkReadout.dec: implements AlphaCAMM readout based in bulk Micromegas used as prototype for AlphaCAMM.

#### Generating readout

A macro is provided to help to generate the readout ´generateReadout.C´, which generates the readout using the rml files described above:
```
restRoot
generateReadout ("AlphaCAMMReadout.root","microbulkReadout.rml","alphaCAMMReadout")
```

Further help on readout generation and visualization can be found in https://lfna.unizar.es/gifna/basic-readouts

**? WARNING: REST is under continous development.** This README is offered to you by the REST community. Your HELP is needed to keep this file up to date. You are very welcome to contribute fixing typos, updating information or adding new contributions. See also our [Contribution Guide](https://lfna.unizar.es/rest-development/REST_v2/-/blob/master/CONTRIBUTING.md).


