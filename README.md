This is a work in progress. I am transitioning from GitHub repo
`dev-kit-2019` to GitHub repo `dev-kit`.

Local copy of the two repos:

- `~/chromation/dev-kit-2019/` (old)
- `~/chromation/dev-kit-mike/` (new)

Sara is using `dev-kit-2019` to make kits that work with the old
LabVIEW GUI. The firmware in that repo is the old protocol.

The firmware in this repo is the new protocol. It works with the
Python low-level API in repo
https://rainbots@bitbucket.org/eruciform/chromaspec-sean

Local copy of Python API repo:

- `/home/mike/.local/lib/python3.7/site-packages/chromaspec-sean/`

The low-level protocol definitions are in `cfg/chromaspec.json`.

# Tasks

## clean Makefiles

- [ ] pull common code into top-level Makefile `common.mk`
- [ ] add help targets
- [ ] setup test framework
- [ ] turn test framework and `libs` into static libraries

## setup tools

- [ ] avra Python scripts
- [ ] ctags, cscope
- [ ] Doxygen

# Doxygen

One top level Doxyfile

- `./Doxyfile`

A doxygen folder at any level to document with Doxygen

- `./doxygen`
- `./firmware/doxygen`

## Doxyfile customization

The Doxyfile is the default with the following changes.

Generate the default:

```bash
$ doxygen -g
```

Changes:

```
PROJECT_NAME           = "dev-kit"
PROJECT_BRIEF          = "Python-compatible Chromation Spectrometer dev-kit."
OUTPUT_DIRECTORY       = ./doxygen
RECURSIVE              = YES
OPTIMIZE_OUTPUT_FOR_C  = YES
EXTRACT_STATIC         = YES
OPTIMIZE_OUTPUT_JAVA   = NO
EXCLUDE                = doc/ \
                        hardware-drawings/ \
                        labview/ \
                        system-design-sketches/ \
                        temp/
EXTRACT_ALL            = NO
WARN_IF_UNDOCUMENTED   = YES
USE_MDFILE_AS_MAINPAGE = README.md
HTML_EXTRA_STYLESHEET  = "doxygen/doxygen-dark-theme/custom.css" \
                         "doxygen/doxygen-dark-theme/custom_dark_theme.css"
```

- `PROJECT_NAME`
    - one-word project name
    - appears as title on every page
- `PROJECT_BRIEF`
    - optional one-liner
    - appears at the top of every page
- `RECURSIVE`
    - `YES`: look inside project folders for source code
- `EXTRACT_STATIC`
    - include documentation for `static` functions
- `OPTIMIZE_OUTPUT_JAVA`
    - `YES`: if project only contains Python code or Java code

## protocol defined in json

- [ ] find the copy of the json file I was editing to track my
  changes to the protocol
