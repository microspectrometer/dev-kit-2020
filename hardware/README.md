# Chromation dev-kit design and manufacturing files

## Complete file list

The complete list of files is below. Some files listed below are
not publicly available yet but will become available soon.

```
hardware
├── manufacture
│   ├── usb-bridge
│   │   ├── DrillFiles
│   │   ├── Gerber
│   │   └── usb-bridge-xyrs-bom.csv
│   ├── vis-analog-out
│   │   ├── DrillFiles
│   │   ├── Gerber
│   │   └── vis-analog-out-xyrs-bom.csv
│   └── vis-spi-out
│       ├── DrillFiles
│       ├── Gerber
│       └── vis-spi-out-xyrs-bom.csv
└── design
    ├── CUVV-45-1-1-1-SMT
    │   ├── LICENSE.md
    │   ├── CUVV-45-1-1-1-SMT_technical-note-1.0.pdf
    │   └── step
    │       ├── CUVV-45-1-1-1-SMT v1.f3d
    │       └── CUVV-45-1-1-1-SMT v1.step
    ├── pdf
    │   ├── usb-bridge.pdf
    │   ├── vis-analog-out.pdf
    │   └── vis-spi-out.pdf
    ├── usb-bridge
    │   ├── datasheets
    │   ├── eagle.epf
    │   ├── hardware-design.md
    │   ├── lbr
    │   ├── usb-bridge.brd
    │   ├── usb-bridge.sch
    │   └── usb-bridge-xyrs-bom.csv
    ├── vis-analog-out
    │   ├── CAM
    │   ├── eagle.epf
    │   ├── hardware-design.md
    │   ├── lbr
    │   ├── step
    │   ├── vis-analog-out.brd
    │   ├── vis-analog-out.sch
    │   └── vis-analog-out-xyrs-bom.csv
    └── vis-spi-out
        ├── CAM
        ├── datasheets
        ├── eagle.epf
        ├── hardware-design.md
        ├── lbr
        ├── vis-spi-out.brd
        ├── vis-spi-out.sch
        └── vis-spi-out-xyrs-bom.csv
```

## CAD files

### Spectrometer chip package CAD files and documentation

This documentation is intended for PCB layout and opto-mechanical
alignment.

*Chromation does not provide design and manufacturing files for
the internal design of the spectrometer chip itself.*

#### Chip package render

The chip package should look like this after importing into 3D
CAD software:

![CUVV-45-1-1-1-SMT
render](design/CUVV-45-1-1-1-SMT/render/CUVV-45-1-1-1-SMT.png)

#### 3D CAD files

```
hardware
└── design
    └── CUVV-45-1-1-1-SMT
       └── step
            ├── CUVV-45-1-1-1-SMT v1.f3d
            └── CUVV-45-1-1-1-SMT v1.step
```

The chip package is designed in Fusion360.

- if you use Fusion360, import the `.f3d` file
- if you use different 3D CAD software, import the `.step` file
- if you do not have 3D CAD software, you can still view the part
  and measure features [using this link](https://a360.co/2CVHmGy)

See
`hardware/design/CUVV-45-1-1-1-SMT/CUVV-45-1-1-1-SMT_technical-note-1.0.pdf` for additional information about the spectrometer chip.

### PCB CAD files and documentation

The PCBs (printed circuit boards) are designed in EAGLE.

The EAGLE files are in the `hardware/design` folder. The
following files are available now, additional design files are
coming soon.

```
├── usb-bridge
│   ├── datasheets/
│   ├── eagle.epf
│   ├── lbr/
│   ├── usb-bridge.brd
│   ├── usb-bridge.sch
│   └── usb-bridge-xyrs-bom.csv
├── vis-analog-out
│   ├── render
│   │   └── vis-analog-out.png
│   ├── eagle.epf
│   ├── lbr/
│   ├── vis-analog-out.brd
│   ├── vis-analog-out.sch
│   └── vis-analog-out-xyrs-bom.csv
└── vis-spi-out
    ├── datasheets/
    ├── eagle.epf
    ├── lbr/
    ├── vis-spi-out.brd
    ├── vis-spi-out.sch
    └── vis-spi-out-xyrs-bom.csv
```

- if you have EAGLE, simply open the project (this reads the
  `.epf` file to determine which files to open)
- if you do not have EAGLE but are able to import EAGLE files:
    - the schematic is the `.sch`
    - the board is the `.brd`
    - the BOM-and-pick-and-place is the `-xyrs-bom.csv`
- if you do not have EAGLE and are unable to import EAGLE files:
    - PDFs of the circuit schematics are in `hardware/design/pdf`

- [ ] hardware-design.md is electronics design calculations and
  related documentation
- [ ] The manufacturing files (Gerbers and Drill) are in the
  `hardware/manufacture` folder.

- view the Gerbers with a Gerber viewer such as `gerbv`
- the PCBs are 4-layer
- `gerbv` screenshots of each layer-view
  (top/bottom/inner1/inner2) are in `hardware/manufacture/img`

