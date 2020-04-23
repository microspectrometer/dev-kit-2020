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

## protocol defined in json

- [ ] find the copy of the json file I was editing to track my
  changes to the protocol

# Doxygen

One top level Doxyfile

- `./Doxyfile`

A doxygen folder at any level to document with Doxygen

- `./doxygen`
- `./firmware/doxygen`

Optionally, add a second `Doxyfile`:

- `./firmware/Doxyfile`

I decided to add a second `Doxyfile` to have different page
titles and main page files for the `./doxygen` and
`./firmware/doxygen`.

## Doxyfile customization

Generate the default Doxyfile:

```bash
$ doxygen -g
```

The `./Doxyfile` is the default Doxyfile with the following
changes:

```
PROJECT_NAME           = "dev-kit"
PROJECT_BRIEF          = "Python-compatible Chromation Spectrometer dev-kit"
OUTPUT_DIRECTORY       = ./doxygen
RECURSIVE              = YES
OPTIMIZE_OUTPUT_FOR_C  = YES
EXTRACT_STATIC         = YES
OPTIMIZE_OUTPUT_JAVA   = NO
EXCLUDE                = doc/ \
                        hardware-drawings/ \
                        labview/ \
                        system-design-sketches/ \
                        temp/ \
                        firmware/temp/ \
                        firmware/AssemblyExample.md \
                        firmware/Earhart-snippet.c \
                        firmware/README-firmware.md
EXTRACT_ALL            = NO
WARN_IF_UNDOCUMENTED   = YES
USE_MDFILE_AS_MAINPAGE = README.md
HTML_EXTRA_STYLESHEET  = "doxygen/doxygen-dark-theme/custom.css" \
                         "doxygen/doxygen-dark-theme/custom_dark_theme.css"
```

The `./firmware/Doxyfile` is the `./Doxyfile` with the following
changes:

```
PROJECT_NAME           = "firmware"
PROJECT_NUMBER         = "v0"
PROJECT_BRIEF          = "Protocol compatible with chromaspec-sean"
EXCLUDE                = doc/ \
                        temp/
USE_MDFILE_AS_MAINPAGE = README-firmware.md
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
- `USE_MDFILE_AS_MAINPAGE`
    - `README.md`: use the `README.md` file in the same folder
      that as the `doxygen` folder
    - this means the `doxygen` for the top-level and the
      `doxygen` for `./firmware` have their own `README.md` files
      which appear as the main page

Use Vim shortcuts `;DU` to create/update doxygen and `;DV` to
view in the browser.

`;DU` is equivalent to `bash` command:

```bash
$ doxygen ./firmware/Doxyfile
```

`;DV` is equivalent to `bash` command:

```bash
$ explorer "$(cygpath -wa ./firmware/doxygen/html/index.html)"
```

## Create a doxygen folder before running doxygen

The repo *does not contain* a `doxygen` folder after the initial
clone. Create empty `doxygen` folders before running `;DU`.

```bash
$ mkdir ./doxygen # top-level documentation
$ mkdir ./firmware/doxygen # documentation for firmware only
```

The importance of making a `doxygen` folder is that my Doxyfile
looks for this folder:

```
OUTPUT_DIRECTORY       = ./doxygen
```

If a `doxygen` folder does not exist in the `pwd`, doxygen does
not generate documentation.

## Minimum to proceed

At a minimum, have `./Doxyfile` and `./firmware/doxygen/`. Then
the `bash` command to make the documentation is simply:

```bash
$ doxygen Doxyfile
```

## Two versions of the documentation

I created the top-level `./doxygen` to get a top-level version of
the documentation that excluded details in the `firmware`
folder's markdown files. This is for the wider Chromation
audience.

I created the `./doxygen/Doxyfile` to make a distinct `main page`
and `title` for the `./firmware/doxygen` firmware-specific
version of the documentation. This is for me during development
of the firmware.

