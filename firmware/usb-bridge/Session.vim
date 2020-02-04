let SessionLoad = 1
if &cp | set nocp | endif
let s:so_save = &so | let s:siso_save = &siso | set so=0 siso=0
let v:this_session=expand("<sfile>:p")
silent only
silent tabonly
cd ~/chromation/dev-kit-mike/firmware/usb-bridge
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
set shortmess=aoO
argglobal
%argdel
set stal=2
tabnew
tabnew
tabnew
tabnew
tabrewind
edit ~/chromation/dev-kit-mike/firmware/lib/src/Usb.h
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd _ | wincmd |
split
1wincmd k
wincmd w
wincmd w
set nosplitbelow
wincmd t
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe '1resize ' . ((&lines * 17 + 19) / 38)
exe 'vert 1resize ' . ((&columns * 73 + 73) / 147)
exe '2resize ' . ((&lines * 16 + 19) / 38)
exe 'vert 2resize ' . ((&columns * 73 + 73) / 147)
exe 'vert 3resize ' . ((&columns * 73 + 73) / 147)
argglobal
setlocal fdm=syntax
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
206
normal! zo
206
normal! zc
234
normal! zo
235
normal! zo
242
normal! zo
243
normal! zo
242
normal! zc
278
normal! zo
278
normal! zc
let s:l = 249 - ((14 * winheight(0) + 8) / 17)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
249
normal! 0
wincmd w
argglobal
if bufexists("~/chromation/dev-kit-mike/firmware/lib/test/test_runner.c") | buffer ~/chromation/dev-kit-mike/firmware/lib/test/test_runner.c | else | edit ~/chromation/dev-kit-mike/firmware/lib/test/test_runner.c | endif
setlocal fdm=syntax
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
502
normal! zo
504
normal! zo
515
normal! zo
517
normal! zo
524
normal! zo
526
normal! zo
533
normal! zo
535
normal! zo
543
normal! zo
545
normal! zo
550
normal! zo
552
normal! zo
561
normal! zo
563
normal! zo
let s:l = 537 - ((9 * winheight(0) + 8) / 16)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
537
normal! 06|
wincmd w
argglobal
if bufexists("~/chromation/dev-kit-mike/firmware/lib/test/test_Usb.c") | buffer ~/chromation/dev-kit-mike/firmware/lib/test/test_Usb.c | else | edit ~/chromation/dev-kit-mike/firmware/lib/test/test_Usb.c | endif
setlocal fdm=syntax
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
let s:l = 397 - ((57 * winheight(0) + 17) / 34)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
397
normal! 0
wincmd w
3wincmd w
exe '1resize ' . ((&lines * 17 + 19) / 38)
exe 'vert 1resize ' . ((&columns * 73 + 73) / 147)
exe '2resize ' . ((&lines * 16 + 19) / 38)
exe 'vert 2resize ' . ((&columns * 73 + 73) / 147)
exe 'vert 3resize ' . ((&columns * 73 + 73) / 147)
tabnext
edit build/temp.avra
set splitbelow splitright
wincmd _ | wincmd |
split
1wincmd k
wincmd w
set nosplitbelow
wincmd t
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe '1resize ' . ((&lines * 17 + 19) / 38)
exe '2resize ' . ((&lines * 16 + 19) / 38)
argglobal
setlocal fdm=syntax
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
let s:l = 3 - ((2 * winheight(0) + 8) / 17)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
3
normal! 0
wincmd w
argglobal
if bufexists("build/usb-bridge.avra") | buffer build/usb-bridge.avra | else | edit build/usb-bridge.avra | endif
setlocal fdm=syntax
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
let s:l = 110 - ((4 * winheight(0) + 8) / 16)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
110
normal! 0
wincmd w
exe '1resize ' . ((&lines * 17 + 19) / 38)
exe '2resize ' . ((&lines * 16 + 19) / 38)
tabnext
edit ~/chromation/dev-kit-mike/python/use-api-example.py
set splitbelow splitright
set nosplitbelow
wincmd t
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
argglobal
setlocal fdm=expr
setlocal fde=SimpylFold#FoldExpr(v:lnum)
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
3
normal! zo
119
normal! zo
let s:l = 112 - ((25 * winheight(0) + 17) / 34)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
112
normal! 0
tabnext
edit ~/chromation/ref-dev-kit-mike/firmware/lib/test/test_Ft1248.c
set splitbelow splitright
set nosplitbelow
wincmd t
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
argglobal
setlocal fdm=syntax
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
177
normal! zo
let s:l = 175 - ((70 * winheight(0) + 17) / 34)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
175
normal! 0
tabnext
edit ~/chromation/dev-kit-mike/README.md
set splitbelow splitright
wincmd _ | wincmd |
split
1wincmd k
wincmd w
set nosplitbelow
wincmd t
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe '1resize ' . ((&lines * 17 + 19) / 38)
exe '2resize ' . ((&lines * 16 + 19) / 38)
argglobal
setlocal fdm=expr
setlocal fde=StackedMarkdownFolds()
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
2904
normal! zo
2924
normal! zo
2955
normal! zo
2974
normal! zo
2991
normal! zo
3003
normal! zo
3466
normal! zo
4311
normal! zo
let s:l = 3039 - ((14 * winheight(0) + 8) / 17)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
3039
normal! 05|
wincmd w
argglobal
if bufexists("~/chromation/dev-kit-mike/README.md") | buffer ~/chromation/dev-kit-mike/README.md | else | edit ~/chromation/dev-kit-mike/README.md | endif
setlocal fdm=expr
setlocal fde=StackedMarkdownFolds()
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
1
normal! zo
189
normal! zo
235
normal! zo
278
normal! zo
316
normal! zo
420
normal! zo
444
normal! zo
451
normal! zo
504
normal! zo
540
normal! zo
562
normal! zo
608
normal! zo
623
normal! zo
806
normal! zo
859
normal! zo
904
normal! zo
1081
normal! zo
2749
normal! zo
2751
normal! zo
2768
normal! zo
2774
normal! zo
2780
normal! zo
2793
normal! zo
2814
normal! zo
2853
normal! zo
3245
normal! zo
3262
normal! zo
3267
normal! zo
3277
normal! zo
3297
normal! zo
3363
normal! zo
3395
normal! zo
4038
normal! zo
let s:l = 2808 - ((15 * winheight(0) + 8) / 16)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
2808
normal! 0
wincmd w
exe '1resize ' . ((&lines * 17 + 19) / 38)
exe '2resize ' . ((&lines * 16 + 19) / 38)
tabnext 1
set stal=1
badd +149 ~/chromation/ref-dev-kit-mike/firmware/lib/src/Ft1248.c
badd +1 build/temp.avra
badd +1 ~/chromation/dev-kit-mike/python/use-api-example.py
badd +1 ~/chromation/ref-dev-kit-mike/firmware/lib/test/test_Ft1248.c
badd +1 ~/chromation/dev-kit-mike/README.md
badd +117 ~/chromation/ref-dev-kit-mike/firmware/lib/src/Usb.c
badd +234 ~/chromation/dev-kit-mike/firmware/lib/src/Usb.h
badd +583 ~/chromation/dev-kit-mike/firmware/lib/test/test_runner.c
badd +1 build/usb-bridge.avra
badd +36 ~/chromation/ref-dev-kit-mike/firmware/usb-bridge/src/usb-bridge.c
badd +58 ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/vis-spi-out.c
badd +16 ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/Hardware.h
badd +51 src/usb-bridge.c
badd +9 NERD_tree_6
badd +1 ~/chromation/dev-kit-mike/.git/info/exclude
badd +104 Makefile
badd +1 ~/chromation/ref-dev-kit-mike/firmware/usb-bridge/git-commits.md
badd +1 ~/chromation/ref-dev-kit-mike/firmware/usb-bridge/inline-files.md
badd +1 ~/chromation/dev-kit-mike/firmware/vis-spi-out/Makefile
badd +2 old-Makefile
badd +7 ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/Example.c
badd +7 src/Example.c
badd +1 ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/Example.h
badd +2 src/Example.h
badd +9 ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/BiColorLed-Hardware.h
badd +9 src/BiColorLed-Hardware.h
badd +20 ~/chromation/ref-dev-kit-mike/firmware/usb-bridge/src/BiColorLed-Hardware.h
badd +737 ~/.vimrc-old
badd +83 ~/chromation/dev-kit-mike/firmware/vis-spi-out/test/test_runner.c
badd +1 test/test_runner.c
badd +2 test/test_Example.c
badd +1 ~/chromation/dev-kit-mike/firmware/vis-spi-out/test/test_Example.c
badd +1 make:\ \*\*\*\ \[Makefile
badd +9 test/test_Example.h
badd +1 ~/chromation/dev-kit-mike/firmware/vis-spi-out/test/test_Example.h
badd +14 src/Hardware.h
badd +2 ~/chromation/dev-kit-mike/firmware/lib/src/Cmd.h
badd +10 ~/chromation/dev-kit-mike/firmware/lib/test/BiColorLed-HardwareFake.h
badd +59 ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/vis-spi-out--with-look-up-table.c
badd +16 ~/chromation/dev-kit-mike/firmware/lib/src/BiColorLed.h
badd +1 ~/chromation/dev-kit-mike/firmware/lib/test/Ft1248-HardwareFake.h
badd +16 ~/chromation/dev-kit-mike/firmware/lib/test/Usb-HardwareFake.h
badd +21 ~/chromation/dev-kit-mike/firmware/lib/src/Usb.c
badd +84 ~/chromation/dev-kit-mike/firmware/lib/Makefile
badd +389 ~/chromation/dev-kit-mike/firmware/lib/test/test_Usb.c
badd +52 ~/chromation/dev-kit-mike/firmware/lib/test/test_Usb.h
badd +1 ~/chromation/dev-kit-mike/firmware/lib/make:\ \*\*\*\ \[Makefile
badd +26 ~/chromation/ref-dev-kit-mike/firmware/lib/test/fake/Ft1248-Hardware.h
badd +4 ~/chromation/dev-kit-mike/firmware/lib/test/Lis-HardwareFake.h
badd +57 ~/chromation/dev-kit-mike/firmware/lib/src/Lis.h
badd +9 ~/chromation/dev-kit-mike/firmware/lib/test/HardwareFake.h
badd +3 ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/Lis-Hardware.h
badd +74 src/Usb-Hardware.h
badd +10 ~/chromation/ref-dev-kit-mike/firmware/lib/src/Ft1248.h
badd +30 ~/chromation/ref-dev-kit-mike/firmware/usb-bridge/src/Ft1248-Hardware.h
badd +17 ~/chromation/dev-kit-mike/firmware/lib/src/ReadWriteBits.h
badd +1 ~/.vimrc.swp
badd +5462 ~/.vimrc
badd +1 build/TestSuite-results.md
badd +343 ~/chromation/ref-dev-kit-mike/firmware/lib/test/test_runner.c
badd +115 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/mock-c/test/unity/unity.h
badd +73 ~/chromation/dev-kit-mike/firmware/lib/test/test_Lis.c
badd +94 ~/chromation/dev-kit-mike/firmware/lib/test/Usb_faked.c
badd +4 ~/chromation/dev-kit-mike/firmware/lib/test/Usb_faked.h
badd +121 ~/chromation/dev-kit-mike/firmware/lib/test/Lis_faked.c
badd +17 ~/chromation/dev-kit-mike/firmware/lib/test/Lis_faked.h
badd +1 build/Usb.o:/home/Mike/chromation/dev-kit-mike/firmware/lib/src/Usb.h
badd +14 ~/chromation/dev-kit-mike/firmware/lib/src/LisConfig.h
badd +18 ~/chromation/dev-kit-mike/firmware/lib/src/LisConfigs.h
badd +12 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/mock-c/include/RecordedArg.h
badd +1 /usr/include/usb.h
badd +23 ~/chromation/ref-dev-kit-mike/firmware/lib/build/TestSuite-results.md
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20 shortmess=filnxtToOS
set winminheight=1 winminwidth=1
let s:sx = expand("<sfile>:p:r")."x.vim"
if file_readable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &so = s:so_save | let &siso = s:siso_save
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
