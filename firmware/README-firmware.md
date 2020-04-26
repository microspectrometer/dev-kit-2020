# Developer's Guide

# Search code within Vim

## cscope

The Vim package from Cygwin is built with `cscope`. I'd imagine
most Vim builds are built with `cscope` because it is so tightly
integrated with Vim.

See `cscope` configuration in my `~/.vim/vimrc`:

```vim
if has("cscope")
    set nocscopetag
    set cscopequickfix=s-,d-,c-,t-,e-,i-,a-
```

Make a cscope database in the `./firmware` folder.

```bash
$ cd ./firmware/
$ cscope -R -b
```

This creates/overwrites the `cscope.out` file in `./firmware`.
I update the `cscope.out` file regularly. I made shortcut `;cu`.
The shortcut updates my tags file as well.

In Vim, updated connection to the new `cscope.out` file:

```vim
: cscope reset
```

The first time `cscope.out` is made, create the connection and
kill any existing connections. There is a bit to do here. I made
a `;cs` shortcut for this. See details in my `~/.vim/vimrc`:

```
function! CscopeCreateConnection()
```


Search with cscope using `:cscope find`.

- 's' symbol: find all references to `symbol_name`

```vim
:cscope find s symbol_name
```

As a Vim shortcut:

```vim
nnoremap <C-\>s :cs find s <cword><CR>
```

- 'g' global: find the global definition of `<cword>` under the cursor

```vim
:cscope find g symbol_name
```

```vim
nnoremap <C-\>g :cs find g <cword><CR>
```

- 'c' calls: find all calls to the function name under cursor
- 'd' dependencies: find functions called by function under cursor
- 't' text: find all instances of the text under cursor
- 'e' egrep: egrep search for the word under cursor
- 'f' file: open the filename under cursor
- 'i' includes: find files that include the filename under cursor

Then use the Vim quickfix window to navigate through the `cscope`
results.

```vim
:copen
```

## tags

Make a tags file in the `./firmware` folder.

```bash
$ cd ./firmware/
$ ctags --c-kinds=+l --exclude=Makefile -R .
```

- `--c-kinds=+l` includes local variables in the `cscope.out` file
- `-R` recursive
- `.` start in the current folder

Update this file with the same command to overwrite it.

I use Vim shortcut `;cu` which also updates the `cscope.out`
file.

## vimgrep

```vim
:vimgreap /search_term/ files_to_search
```

Example:

```vim
:vimgrep /SetBit(Flag_SpiFlags/ lib/**/*.[ch]
```

Find all occurrences of `SetBit(Flag_SpiFlags` in every `.c` and
`.h` file in the `lib/` folder, including sub-folders.

Open a quickfix window and navigate the results.

```vim
:copen
```

Similar to using `cscope`.

Use `cscope` to when the symbol name is known but its
purpose/definition/dependencies are unknown.

This is the scenario:

- I am looking at a specific line of code
- I see a symbol I want to know more about

Use `vimgrep` when the symbol name itself is not known yet.

This is the scenario:

- I want to determine a variable naming convention
- or I just want to check if a certain variable name is already
  used

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


# TASKS
# Build for dev-kit-mike

## clean Makefiles

- [ ] pull common code into top-level Makefile `common.mk`
- [ ] add help targets
- [ ] setup test framework
- [ ] turn test framework and `libs` into static libraries

## setup tools

- [ ] avra Python scripts
- [ ] ctags, cscope
- [x] Doxygen

## firmware

- [ ] block diagrams starting with `main.c`
- [ ] why is UartSpi-Hardware.h in the `vis-spi-out.c` folder?
    - `vis-spi-out.c` does not include this file
    - who does?
    - [ ] how do I grep for `#include "UartSpi-Hardware.h` across
      the whole project?

# DONE
## protocol defined in json

- [x] find the copy of the `chromaspec.json` file I was editing
  to track my changes to the protocol
    - paste in `dev-kit-mike/firmware` folder

