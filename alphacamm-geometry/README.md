# alphacamm-geometry 

Implements AlphaCAMM geometry, this project contains two rml files which implements alphaCAMM geometry:

- geometry.gdml: Implements AlphaCAMM geometry.
- setup.gdml: Defines different variables to define AlphaCAMM geometry.

#### Visualizing the geometry using TGeoManager

REST implements a class named `TRestGDMLParser` to help reading this modified GDML file and translate it to be compatible with ROOT. The class responsible to read a GDML geometry in ROOT is a `TGeoManager`. See also the [TGeoManager documentation](https://root.cern/doc/v606/classTGeoManager.html) from ROOT pages. Afterwards create a `TGeoManager` to visualize the geometry using root.

```
restRoot
[0] TRestGDMLParser *g = new TRestGDMLParser();
[1] TGeoManager *geo = g->GetGeoManager("setup.gdml");
[2] geo->GetTopVolume()->Draw("ogl");
```

#### Remote materials file

We use a common materials file that is located at the [Git materials project](https://lfna.unizar.es/rest-development/materials). The materials file is shared through the [following remote http location](https://sultan.unizar.es/materials/materials.xml) being the remote synchronized with the Gitlab project.

The materials file is included in the geometry using the following line:

```
<!ENTITY materials SYSTEM "http://sultan.unizar.es/materials/materials.xml">
```

We will be allowed to use any material included in that file for our geometrical volumes. If the material you need is not found we encourage to upload that material to the [Gitlab materials project](https://lfna.unizar.es/rest-development/materials) and share it with anyone else.

**? WARNING: REST is under continous development.** This README is offered to you by the REST community. Your HELP is needed to keep this file up to date. You are very welcome to contribute fixing typos, updating information or adding new contributions. See also our [Contribution Guide](https://lfna.unizar.es/rest-development/REST_v2/-/blob/master/CONTRIBUTING.md).


