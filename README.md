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

## protocol defined in json
- [ ] find the copy of the json file I was editing to track my
  changes to the protocol
