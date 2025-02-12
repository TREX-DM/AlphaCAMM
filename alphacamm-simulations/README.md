# alphacamm-simulations

Implement AlphaCAMM simulations and provides different rml files to perform different simulations:

- 210PoFromSample.rml: Simulate alphas stemming from 210Po from the area in which the sample is located.
- 222RnFromGas.rml: Simulate alphas stemming from 222Rn inside the sensitive volume, e.g. volumetric contamination of 222Rn.
- 218PoFromMylar.rml: Simulate alphas stemming 218Po inside the mylar volume.
- 214PoFromMylar.rml: Same as before but for the 214Po isotope.
- 241AmFromSource.rml: Simulate 241Am source in the middle of the cathode.


#### Running simulations

You shoud define a data path changing the mainDataPath under `common/globals.xml` or defining a global variable named `REST_DATA_PATH` e.g. `export REST_DATA_PATH=~/mydataPath/`. Afterwards, just use `restG4` to launch the desired simulation e.g.:
```
restG4 210PoFromSample.rml
```

**? WARNING: REST is under continous development.** This README is offered to you by the REST community. Your HELP is needed to keep this file up to date. You are very welcome to contribute fixing typos, updating information or adding new contributions. See also our [Contribution Guide](https://lfna.unizar.es/rest-development/REST_v2/-/blob/master/CONTRIBUTING.md).


