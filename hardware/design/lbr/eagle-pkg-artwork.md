# Create package artwork for EAGLE library package

This is how Chromation converts part CUVV-45-1-1-1-SMT from its
Fusion360 .`f3d` format into an EAGLE `.lbr` package. It is
tedious but not error-prone: there are many manual steps in
Fusion360, DraftSight, and EAGLE, but the details (i.e, the exact
coordinates and dimensions) are mostly automated.

## Make DXF from sketch in Fusion

Open CUVV-45-1-1-1-SMT v1.f3d in Fusion360.

**Do the following work at the top-level component.**

*This makes the origin the top-level component origin, not any
sub-component origin.*

**Project top view of model to a sketch.**

- choose the sketch plane when looking from the top of the component
    - rotate the part to get the desired orientation
    - the sketch is in the orientation of the Fusion360 view
    - for example, if the sketch plane is chosen while viewing
      from the bottom, the sketch is mirrored in the top view

**Project features to the sketch plane.**

- now it is OK to view the features from the bottom (the pads)
- the pads project correctly for use in a top-view (as a top-side
  component)

**Export as DXF.**

- export the sketch as two DXF files:
    - filename: `pads.dxf`
    - filename: `pkg.dxf`

## Trace over DXFs in 2D CAD software

- save `DWG` version in project `lbr` `VIS-PKG` folder
    - `pads.dwg`
    - `pkg.dwg`
- create `package.dwg`
    - attach the two drawings as XREFs on layers `xref-pkg` and `xref-pads`
- draw the 51 tDocu (reference) and 21 tPlace (silkscreen) artwork
    - offset silkscreen artwork from pads by 0.17 mm:
      `0.004*25.4 + 0.127/2 = 0.1651`
- draw pin 1 dot on silkscreen
- show spectrometer's light input aperture on silkscreen
- place SMT pads as blocks, each pad on its own layer
    - Python DXF parsing script uses block name to get pad size
      and layer name to get pin number

## Convert `package.dxf` to `package.scr`

- run Python script `pkgscripts.py` to generate the EAGLE script
  `package.scr`
    - `package.scr` draws the package and pad layout in EAGLE

## Run `package.scr` in EAGLE

- open EAGLE library `VIS-PKG.lbr`
    - right-click on the library name in the EAGLE Control Panel
      and select Open
- Select `Library -> Manage packages`
    - create a new package (or open the existing footprint `VIS-PKG`)
- run `package.scr`
    - type command `scr` to open a dialog window to pick the script to run
    - pick `package.scr`

## Create SYMBOL and DEVICE in EAGLE

No automation here. This is the usual process of making library
components in EAGLE.
