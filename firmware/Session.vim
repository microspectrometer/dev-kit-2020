let SessionLoad = 1
if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
nmap  <Plug>(RepeatRedo)
vnoremap  v
nnoremap  v
nnoremap a :cs find a <cword>
nnoremap i :cs find i <cfile>
nnoremap f :cs find f <cfile>
nnoremap e :cs find e <cword>
nnoremap t :cs find t <cword>
nnoremap c :cs find c <cword>
nnoremap d :cs find d <cword>
nnoremap g :cs find g <cword>
nnoremap s :cs find s <cword>
vnoremap - zvdpzvV']
nnoremap - zvddp
nmap . <Plug>(RepeatDot)
nnoremap ;di :call CleanExpandedIncludes()
vmap ;p?=s <Plug>PyCalcAppendWithEqAndSum
vmap ;p?s <Plug>PyCalcAppendWithSum
vmap ;p?= <Plug>PyCalcAppendWithEq
vmap ;p? <Plug>PyCalcAppend
vmap ;;p?s <Plug>PyCalcReplaceWithSum
vmap ;;p? <Plug>PyCalcReplace
vmap ;v?=s <Plug>VimCalcAppendWithEqAndSum
vmap ;v?s <Plug>VimCalcAppendWithSum
vmap ;v?= <Plug>VimCalcAppendWithEq
vmap ;v? <Plug>VimCalcAppend
vmap ;;v?s <Plug>VimCalcReplaceWithSum
vmap ;;v? <Plug>VimCalcReplace
vmap ;b?=s <Plug>BcCalcAppendWithEqAndSum
vmap ;b?s <Plug>BcCalcAppendWithSum
vmap ;b?= <Plug>BcCalcAppendWithEq
vmap ;b? <Plug>BcCalcAppend
vmap ;;b?s <Plug>BcCalcReplaceWithSum
vmap ;;b? <Plug>BcCalcReplace
vmap ;?=s <Plug>AutoCalcAppendWithEqAndSum
vmap ;?s <Plug>AutoCalcAppendWithSum
vmap ;?= <Plug>AutoCalcAppendWithEq
vmap ;? <Plug>AutoCalcAppend
vmap ;;?s <Plug>AutoCalcReplaceWithSum
vmap ;;? <Plug>AutoCalcReplace
nnoremap ;pc /^[^ \t#]
nnoremap <silent> ;_ :call SpaceTo_()
vnoremap <silent> ;_ :call SpaceTo_InVisualSelection()
vnoremap <silent> ;_  :call UnderscoreToSpaceInVisualSelection()
nnoremap ;CS :call ColorschemeToggle()
nnoremap ;md :cd %:h:!/cygdrive/c/Windows/explorer.exe "$(cygpath -w -a %:r.md)"
nnoremap ;html :cd %:h:!/cygdrive/c/Windows/explorer.exe "$(cygpath -w -a %:r.html)"
nnoremap ;exd :!/cygdrive/c/Windows/explorer.exe "C:\chromation-dropbox\Dropbox\technical references\datasheets\electronics"
nnoremap ;exf :cd %:h:!/cygdrive/c/Windows/explorer.exe "$(cygpath -w -a %)"
nnoremap ;exh :cd %:h:!/cygdrive/c/Windows/explorer.exe "$(cygpath -w -a %:h)"
nnoremap ;cw ^v$h"+y:!cygpath -w "+" | clip"+pky^jP
nnoremap ;cp ^v$h"+y:!cygpath "+" | clip"+pky^jP
nnoremap ;%tr :let @" = expand("%:t:r"):echomsg 'let @"='.@"
nnoremap ;%t :let @" = expand("%:t"):echomsg 'let @"='.@"
nnoremap ;%r :let @" = expand("%:r"):echomsg 'let @"='.@"
nnoremap ;%h :let @" = expand("%:h"):echomsg 'let @"='.@"
nnoremap ;% :let @" = expand("%"):echomsg 'let @"='.@"
nnoremap ;%ph :let @" = expand("%:p:h"):echomsg 'let @"='.@"
nnoremap ;%p :let @" = expand("%:p"):echomsg 'let @"='.@"
nnoremap ;cd :cd %:h:pwd
nnoremap ;ea :exec "edit ".PATH_earhart_programmer
nnoremap ;ei :exec "edit " . PATH_2017EvalSpectrometer_inventory
nnoremap ;ep  :exec "edit ".PATH_ps_profile
nnoremap ;erv :vertical split ~/programming-reference.md:loadview
nnoremap ;ers :split ~/programming-reference.md:loadview
nnoremap ;ert :tabedit ~/programming-reference.md:loadview
nnoremap ;er  :edit ~/programming-reference.md:loadview
nnoremap ;ecv :exec "vertical split ".PATH_tasks_chromation:loadview
nnoremap ;ecs :exec "split ".PATH_tasks_chromation:loadview
nnoremap ;ect :exec "tabedit ".PATH_tasks_chromation:loadview
nnoremap ;ec  :exec "edit ".PATH_tasks_chromation:loadview
nnoremap ;env :exec "vertical split ".PATH_notes_for_update:loadview
nnoremap ;ens :exec "split ".PATH_notes_for_update:loadview
nnoremap ;ent :exec "tabedit ".PATH_notes_for_update:loadview
nnoremap ;en  :exec "edit ".PATH_notes_for_update:loadview
nnoremap ;evv :vertical split $MYVIMRCzv
nnoremap ;evs :split $MYVIMRCzv
nnoremap ;evt :tabedit $MYVIMRCzv
nnoremap ;ev  :edit $MYVIMRC
nnoremap ;sh :source $VIMRUNTIME/syntax/hitest.vim
nnoremap ;so :call SourceCurrentFile()
nnoremap ;sv :source $MYVIMRC
nnoremap ;ta /\[\ \]/e-1zv
nnoremap ;gq :call SoftWrapToggle()
nnoremap ;hw :call WhitespaceToggle()
nnoremap ;hc :call CommentToggle()
nnoremap ;trn :TabulousRename =expand("%:p:t")
nnoremap ;trh :TabulousRename =expand("%:p:h:t")
nnoremap ;tr  :call g:tabulous#renameTab()
nnoremap ;trwn :windo TabulousRename =expand("%:p:t:r")
nnoremap ;trwh :windo TabulousRename =expand("%:p:h:t")
nnoremap ;trw  :windo call g:tabulous#renameTab()
nnoremap ;n  :call nerdtree_#RefreshActiveFolder()
nmap ;nR :call RefreshNERDTreeWindow()
nmap ;nr :call nerdtree_#RefreshFolder('build')
nnoremap ;nb :NERDTreeFromBookmark 
nnoremap ;ne :NERDTree 
nnoremap <silent> ;nt :NERDTreeToggle
nnoremap ;nf :NERDTreeFind
nnoremap ;ncd :call ChangeDirectoryAndTreeToCurrentFile()
nnoremap ;eol :call RemoveEolCaretM()
nnoremap ;w 
nnoremap <silent> ;-b I| A |yyP^v$gr-jyyp^v$gr-k
nnoremap <silent> ;=s I=====[ A ]=====F[w
nnoremap <silent> ;-s I---A---bb
nnoremap <silent> ;-u yyp^v$gr-k
nnoremap <silent> ;-U yyP^v$gr-j
nnoremap ;a :call InsertArithmeticResult()
nnoremap ;pwd :redir @+|pwd|redir END$"+pj:execute "left"indent(line('.')-1)
nnoremap ;cl :call BuildThisCfileWith("clang")
nnoremap ;c+ :call BuildThisCfileWith("g++")
nnoremap ;cc :call BuildThisCfileWith("gcc")
nnoremap ;co :!clang -Wall -Wextra -pedantic -c % -o %:r.o
nnoremap ;Tl :belowright vertical terminal
nnoremap ;Tj :belowright terminal
nnoremap ;Th :aboveleft vertical terminal
nnoremap ;Tk :aboveleft terminal
nnoremap ;TH :topleft vertical terminal
nnoremap ;TK :topleft terminal
nnoremap ;TL :botright vertical terminal
nnoremap ;TJ :botright terminal
nnoremap ;T  :terminal
nnoremap ;bb :call ToggleCursorbind()
nnoremap ;bo :set noscb nocrb
nnoremap ;do :diffoff
nnoremap ;dt :diffthis
nnoremap ;eK :topleft split 
nnoremap ;eJ :botright split 
nnoremap ;eL :botright vertical split 
nnoremap ;eH :topleft vertical split 
nnoremap ;ek :aboveleft split 
nnoremap ;ej :belowright split 
nnoremap ;el :belowright vertical split 
nnoremap ;eh :aboveleft vertical split 
vnoremap <silent> ;gt :call VisualSelectionToTitlecase()
nnoremap <silent> ;gt :s/\w\+/\L\u\0/g:noh
nnoremap ;W  :call RemoveCharsAroundWord()
nnoremap ;x  :call RemoveCharsAroundChar()
nnoremap ;s  :call SurroundCharWithSpace()
nnoremap ;zi :call SetFoldmethodIndent()
nnoremap ;zx :call SetFoldmethodExpr()
nnoremap ;zm :call SetFoldmethodManual()
nnoremap ;FD :call FindPrevFunctionDefinition()
nnoremap ;fd :call FindNextFunctionDefinition()
nnoremap ;fs :call CopyCFuncSigToReg()
nnoremap ;[M :call GotoPrevEndOfFunction()
nnoremap ;]M :call GotoNextEndOfFunction()
nnoremap ;[m :call GotoPrevStartOfFunction()
nnoremap ;]m :call GotoNextStartOfFunction()
nnoremap ;pkg :call PackageScripts()
nnoremap ;pan :!pandoc -f markdown %:r.md -s -o %:r.html -c pandoc.css --toc --toc-depth=3
nnoremap ;4 :call BufAutoHeight()
nnoremap ;te :call CloseTestResults():call ReadTestResults()
nnoremap ;t  :cclose:call CloseTestResults()
nnoremap ;f  :cclose:call avrmake#CloseLogfileWindows()
nnoremap ;r  :cclose:call CloseExampleOutputWindows()
nnoremap ;fa :call DownloadFlash()
nnoremap ;re :call ReadOutput(expand("%:t:r"))
nnoremap ;rf :call ReadFailedOutput(expand("%:t:r"))
nnoremap ;co+ :call CompileObject("g++")
nnoremap ;col :call CompileObject("clang")
nnoremap ;coc :call CompileObject("gcc")
nnoremap ;mr+ :call MakeDefaultTargetWithGplusplusAndReadOutput(expand("%:t:r"))
nnoremap ;mrl :call MakeDefaultTargetWithClangAndReadOutput(expand("%:t:r"))
nnoremap ;mrc :call MakeDefaultTargetAndReadOutput(expand("%:t:r"), 'gcc')
nnoremap ;mfa :call BuildAndDownloadFlash()
nnoremap ;k :call TestLineBreak()
nnoremap ;mtwc+ :call CloseTestResults():make compiler=x86_64-w64-mingw32-clang++:call MakeQuickfixAndTest()
nnoremap ;mtc+ :call CloseTestResults():make compiler=clang++:call MakeQuickfixAndTest()
nnoremap ;mtwcl :call CloseTestResults():make compiler=x86_64-w64-mingw32-clang:call MakeQuickfixAndTest()
nnoremap ;mtcl :call CloseTestResults():make compiler=clang:call MakeQuickfixAndTest()
nnoremap ;mtwg+ :call CloseTestResults():make compiler=x86_64-w64-mingw32-g++:call MakeQuickfixAndTest()
nnoremap ;mtg+ :call CloseTestResults():make compiler=g++:call MakeQuickfixAndTest()
nnoremap ;mtwgc :call CloseTestResults():make compiler=x86_64-w64-mingw32-gcc:call MakeQuickfixAndTest()
nnoremap ;mtgc :call CloseTestResults():make compiler=gcc:call MakeQuickfixAndTest()
nnoremap ;mtu :call CloseTestResults():make TestUnityExtensions compiler=clang:call MakeQuickfixAndTest()
nnoremap ;mk+ :call MakeDefaultTargetWithGplusplus(expand("%:t:r"))
nnoremap ;mkl :call MakeDefaultTargetWithClang(expand("%:t:r"))
nnoremap ;mktc+ :call MakeUnittestTargetWith('clang++')
nnoremap ;mktcl :call MakeUnittestTargetWith('clang')
nnoremap ;mktg+ :call MakeUnittestTargetWith('g++')
nnoremap ;mktgc :call MakeUnittestTargetWith('gcc')
nnoremap ;mkwc+ :call MakeDefaultTargetWith('x86_64-w64-mingw32-clang++')
nnoremap ;mkc+ :call MakeDefaultTargetWith('clang++')
nnoremap ;mkwcl :call MakeDefaultTargetWith('x86_64-w64-mingw32-clang')
nnoremap ;mkcl :call MakeDefaultTargetWith('clang')
nnoremap ;mkwg+ :call MakeDefaultTargetWith('x86_64-w64-mingw32-g++')
nnoremap ;mkg+ :call MakeDefaultTargetWith('g++')
nnoremap ;mkwgc :call MakeDefaultTargetWith('x86_64-w64-mingw32-gcc')
nnoremap ;mkgc :call MakeDefaultTargetWith('gcc')
nnoremap ;mkfgc :call MakeDefaultTargetForFileStemWith('gcc')
nnoremap ;mkv :make display_target_voltage
nnoremap ;mkp :make test_programmer_is_connected
nnoremap ;mna :make avr-target compiler=avr-gcc -n
nnoremap ;mka :call CloseTestResults():make avr-target compiler=avr-gcc:call MakeQuickfix()
nnoremap ;mkf :make compiler=fake -n unit-test
nnoremap ;mca :call MakeCleanAllBuilds()
nnoremap ;mC :call CloseTestResults():make clean-test-and-lib-builds:call MakeQuickfix()
nnoremap ;mct :call MakeCleanTestSuite()
nnoremap ;ve :call ExampleCheckingIfBuildFailed()
nnoremap ;yy :call AddTest()
nnoremap ;hg :call AddIfndefGuardsInHeader()
nnoremap ;yh :call expectmocking#AddFuncDeclToHeader(add_silently)
nnoremap ;yt :call AddTestToHeaderAndRunner()
nnoremap ;ys :call AddStubToMockAndTest()
nnoremap ;me :call MockExpectedCall()
nnoremap ;edh :call expectmocking#AddExternDeclToHeader(add_silently)
nnoremap ;fh :call expectmocking#AddFuncDeclToHeader(add_silently)
nnoremap ;ym :call YankIntoMock()
nnoremap ;ck :call RemoveCscopeDatabase()
nnoremap ;cb :call CreateCscopeDatabase()
nnoremap ;cu :call UpdateTagsAndCscope()
nnoremap ;cU :call ctags#UpdateLibTags()
nnoremap ;cs :call ctags#Update():call CscopeCreateConnection()
nnoremap ;cfpy :!find . -name '*.py' > cscope.files
nnoremap ;tp :!ctags -R --languages=python
nnoremap ;rs :silent redraw!
nnoremap ;rh :syntax sync fromstart
nnoremap ;opt :call OmniPreviewToggle()
nnoremap <silent> ;hh :execute 'match Search /\%'.line('.').'l/'mh
nnoremap <silent> ;h  :match
nnoremap ;  :noh
nnoremap ;H :set hlsearch
nnoremap ;ms  :call WriteThisSession()
nnoremap ;ms? :echo "This session: " . v:this_session
nnoremap ;ls :source ~/sessions/
nnoremap ;mv :mkview
nnoremap ;lv :loadview
nnoremap ;xdc :call DocstringFromCTests()
nnoremap ;xcsv :call CsvToVimTable()
nnoremap <silent> ;WR :call ColorWipRip()
nnoremap ;gl  :call GitLogInVim('')
nnoremap ;glw :call GitLogInVim('WIP')
nnoremap ;gl: :!git log --pretty=format:"- \%ci \%>(15)\%ar \%s" | grep --color ""
nnoremap ;Gh :!git log | grep commit -m 1 | perl -pe 's/commit //;' | perl -pe 's/\n//;' | clip:echomsg "Latest git commit: ".@+.". Paste from Vim register '+'."
nnoremap ;Gl :!git log --decorate --graph --pretty=format:"- \%C(yellow)\%ci \%C(blue)\%>(15)\%ar \%C(reset)\%s" 
nnoremap ;Gb :!git branch 
nnoremap ;GR :!git rebase -i HEAD~
nnoremap ;Gr :!git remote -v
nnoremap ;Gca :!git commit --amend
nnoremap ;Gcm :!git commit -m 
nnoremap ;GcM :!git commit
nnoremap ;Gw :!git add %
nnoremap ;Ga :!git add .
nnoremap ;Gd :!git diff 
nnoremap ;Gs :!git status 
nnoremap ;G: :!git 
nnoremap ;gr :!grep "inline" --color -rn . --include=*.[ch]
nnoremap ;ro :set readonly
nnoremap ;, ;
xmap S <Plug>VSurround
nmap U <Plug>(RepeatUndoLine)
nnoremap Y y$
vnoremap _ zvdkPzvV']
nnoremap _ zvddkP
nmap cS <Plug>CSurround
nmap cs <Plug>Csurround
nmap ds <Plug>Dsurround
xmap gS <Plug>VgSurround
nmap gcu <Plug>Commentary<Plug>Commentary
nmap gcc <Plug>CommentaryLine
omap gc <Plug>Commentary
nmap gc <Plug>Commentary
xmap gc <Plug>Commentary
vmap gx <Plug>NetrwBrowseXVis
nmap gx <Plug>NetrwBrowseX
nnoremap <silent> gcs :call SurroundWithEqAndSqBrackThenComment()
nnoremap gL :call GitLogWithTimesToFile()
nnoremap gl :call GitLogWithTimesToConsole()
nmap ga <Plug>(EasyAlign)
xmap ga <Plug>(EasyAlign)
vnoremap scp "sy:%s/\<"\>//nc
vnoremap sc  "sy:%s/"//nc
vnoremap sp "sy/\<"\>zv
vnoremap s  "sy/"zv
nmap u <Plug>(RepeatUndo)
vnoremap v 
nnoremap v 
nmap ySS <Plug>YSsurround
nmap ySs <Plug>YSsurround
nmap yss <Plug>Yssurround
nmap yS <Plug>YSurround
nmap ys <Plug>Ysurround
nnoremap <silent> <Plug>(RepeatRedo) :call repeat#wrap("\<C-R>",v:count)
nnoremap <silent> <Plug>(RepeatUndoLine) :call repeat#wrap('U',v:count)
nnoremap <silent> <Plug>(RepeatUndo) :call repeat#wrap('u',v:count)
nnoremap <silent> <Plug>(RepeatDot) :exe repeat#run(v:count)
nnoremap <silent> <Plug>SurroundRepeat .
nmap <silent> <Plug>CommentaryUndo :echoerr "Change your <Plug>CommentaryUndo map to <Plug>Commentary<Plug>Commentary"
vnoremap <silent> <Plug>PyCalcAppendWithEqAndSum :call HowMuch#HowMuch(1,1,1,'py')
vnoremap <silent> <Plug>PyCalcAppendWithSum :call HowMuch#HowMuch(1,0,1,'py')
vnoremap <silent> <Plug>PyCalcAppendWithEq :call HowMuch#HowMuch(1,1,0,'py')
vnoremap <silent> <Plug>PyCalcAppend :call HowMuch#HowMuch(1,0,0,'py')
vnoremap <silent> <Plug>PyCalcReplaceWithSum :call HowMuch#HowMuch(0,0,1,'py')
vnoremap <silent> <Plug>PyCalcReplace :call HowMuch#HowMuch(0,0,0,'py')
vnoremap <silent> <Plug>VimCalcAppendWithEqAndSum :call HowMuch#HowMuch(1,1,1,'vim')
vnoremap <silent> <Plug>VimCalcAppendWithSum :call HowMuch#HowMuch(1,0,1,'vim')
vnoremap <silent> <Plug>VimCalcAppendWithEq :call HowMuch#HowMuch(1,1,0,'vim')
vnoremap <silent> <Plug>VimCalcAppend :call HowMuch#HowMuch(1,0,0,'vim')
vnoremap <silent> <Plug>VimCalcReplaceWithSum :call HowMuch#HowMuch(0,0,1,'vim')
vnoremap <silent> <Plug>VimCalcReplace :call HowMuch#HowMuch(0,0,0,'vim')
vnoremap <silent> <Plug>BcCalcAppendWithEqAndSum :call HowMuch#HowMuch(1,1,1,'bc')
vnoremap <silent> <Plug>BcCalcAppendWithSum :call HowMuch#HowMuch(1,0,1,'bc')
vnoremap <silent> <Plug>BcCalcAppendWithEq :call HowMuch#HowMuch(1,1,0,'bc')
vnoremap <silent> <Plug>BcCalcAppend :call HowMuch#HowMuch(1,0,0,'bc')
vnoremap <silent> <Plug>BcCalcReplaceWithSum :call HowMuch#HowMuch(0,0,1,'bc')
vnoremap <silent> <Plug>BcCalcReplace :call HowMuch#HowMuch(0,0,0,'bc')
vnoremap <silent> <Plug>AutoCalcAppendWithEqAndSum :call HowMuch#HowMuch(1,1,1,'auto')
vnoremap <silent> <Plug>AutoCalcAppendWithSum :call HowMuch#HowMuch(1,0,1,'auto')
vnoremap <silent> <Plug>AutoCalcAppendWithEq :call HowMuch#HowMuch(1,1,0,'auto')
vnoremap <silent> <Plug>AutoCalcAppend :call HowMuch#HowMuch(1,0,0,'auto')
vnoremap <silent> <Plug>AutoCalcReplaceWithSum :call HowMuch#HowMuch(0,0,1,'auto')
vnoremap <silent> <Plug>AutoCalcReplace :call HowMuch#HowMuch(0,0,0,'auto')
vnoremap <silent> <Plug>NetrwBrowseXVis :call netrw#BrowseXVis()
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#BrowseX(netrw#GX(),netrw#CheckIfRemote(netrw#GX()))
nnoremap <BS> :call SpecialBufferToggle()
nnoremap <C-Down> :wincmd+
nnoremap <C-Up> :wincmd-
nnoremap <C-Left> :wincmd<
nnoremap <C-Right> :wincmd>
vnoremap <M-Down> :call LeftAlignWithBelow()
nnoremap <M-Down> :execute "left ".expand(indent(line('.')+1))
vnoremap <M-Up> :call LeftAlignWithAbove()
nnoremap <M-Up> :execute "left ".expand(indent(line('.')-1))
imap S <Plug>ISurround
imap s <Plug>Isurround
imap  <Plug>Isurround
nnoremap ¿ :cpreviouszv
nnoremap ¯ :cnextzv
nnoremap ò 
nnoremap ë :call NextNonCommentLineOfC('k')
nnoremap ê :call NextNonCommentLineOfC('j')
cnoremap jk 
inoremap jk 
iabbr mmm TEST_ASSERT_TRUE_MESSAGE(<BS>RanAsHoped(mock),           // If this is false,WhyDidItFail(mock)          // print this message.);
iabbr ttt TEST_FAIL_MESSAGE("Implement test.");
iabbr ppp #!/usr/bin/env python3# -*- coding: utf-8 -*-
let &cpo=s:cpo_save
unlet s:cpo_save
set keymap=symbols
set autoindent
set background=dark
set backspace=indent,eol,start
set cmdheight=2
set completeopt=menuone,preview
set cscopeprg=/usr/bin/cscope
set cscopequickfix=s-,d-,c-,t-,e-,i-,a-
set cscopeverbose
set display=truncate
set expandtab
set fileencodings=ucs-bom,utf-8,default,latin1
set guicursor=n-v-c:block,o:hor50,i-ci:hor15,r-cr:hor30,sm:block,a:blinkon0
set helplang=en
set hlsearch
set ignorecase
set incsearch
set nojoinspaces
set langnoremap
set nolangremap
set laststatus=2
set listchars=tab:>-,trail:·,space:·
set nrformats=bin,hex
set operatorfunc=<SNR>49_go
set path=.,/usr/include,,,./**,/usr/include/glib-2.0,/cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/mock-c/include,/cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/mock-c/test,/cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/mock-c/test/unity,/cygdrive/c/Program\\\ Files\\\ (x86)/Atmel/Studio/7.0/toolchain/avr8/avr8-gnu-toolchain/avr/include,/usr/lib/clang/5.0.1/include,/usr/lib/gcc/x86_64-pc-cygwin/6.4.0/include,/usr/lib/gcc/x86_64-pc-cygwin/6.4.0/include/c++,/usr/lib/gcc/x86_64-pc-cygwin/6.4.0/include/c++/tr1,/usr/lib/gcc/x86_64-pc-cygwin/6.4.0/include/c++/x86_64-pc-cygwin,~/.local/lib/python3.6/site-packages,/usr/lib/python3.7/,/usr/lib/python3.7/site-packages/
set previewheight=6
set ruler
set runtimepath=~/.vim,~/.vim/pack/bundle/start/vim-surround,~/.vim/pack/bundle/start/vim-repeat,~/.vim/pack/bundle/start/vim-markdown,~/.vim/pack/bundle/start/vim-easy-align,~/.vim/pack/bundle/start/vim-commentary,~/.vim/pack/bundle/start/tabulous,~/.vim/pack/bundle/start/nerdtree,~/.vim/pack/bundle/start/HowMuch,~/.vim/pack/bundle/start/echodoc.vim,~/.vim/pack/bundle/start/deol.nvim,/usr/share/vim/vimfiles,/usr/share/vim/vim81,/usr/share/vim/vimfiles/after,~/.vim/after,~/.vim/pack/bundle/dev
set scrollopt=hor
set shell=bash
set shiftwidth=4
set showcmd
set showmatch
set smartcase
set softtabstop=4
set splitright
set synmaxcol=160
set tabline=%!Tabulous()
set tabstop=4
set tags=./tags,tags,lib-tags,/cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/mock-c/tags
set textwidth=80
set notimeout
set title
set ttimeout
set ttimeoutlen=0
set undofile
set wildignore=*.pyc
set wildmenu
set window=40
let s:so_save = &so | let s:siso_save = &siso | set so=0 siso=0
let v:this_session=expand("<sfile>:p")
silent only
silent tabonly
cd /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff-try
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
tabrewind
edit /cygdrive/c/chromation-dropbox/Dropbox/c/LIS-770i/mBrd/src/mBrd.c
set splitbelow splitright
wincmd _ | wincmd |
split
1wincmd k
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
wincmd _ | wincmd |
split
1wincmd k
wincmd w
wincmd w
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
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
exe '1resize ' . ((&lines * 16 + 20) / 41)
exe 'vert 1resize ' . ((&columns * 79 + 88) / 177)
exe '2resize ' . ((&lines * 8 + 20) / 41)
exe 'vert 2resize ' . ((&columns * 97 + 88) / 177)
exe '3resize ' . ((&lines * 7 + 20) / 41)
exe 'vert 3resize ' . ((&columns * 97 + 88) / 177)
exe '4resize ' . ((&lines * 20 + 20) / 41)
exe 'vert 4resize ' . ((&columns * 79 + 88) / 177)
exe '5resize ' . ((&lines * 8 + 20) / 41)
exe 'vert 5resize ' . ((&columns * 97 + 88) / 177)
exe '6resize ' . ((&lines * 11 + 20) / 41)
exe 'vert 6resize ' . ((&columns * 97 + 88) / 177)
argglobal
lnoremap <buffer> ALPHA Α
lnoremap <buffer> BETA Β
lnoremap <buffer> CHI Χ
lnoremap <buffer> Dhh ┅
lnoremap <buffer> Dhv ┇
lnoremap <buffer> Dlh ┄
lnoremap <buffer> Dlv ┆
lnoremap <buffer> DELTA Δ
lnoremap <buffer> ETA Η
lnoremap <buffer> EPSILON Ε
lnoremap <buffer> GAMMA Γ
lnoremap <buffer> PHI Φ
lnoremap <buffer> RHO Ρ
lnoremap <buffer> THETA Θ
lnoremap <buffer> ZETA Ζ
lnoremap <buffer> aumlaut ä
lnoremap <buffer> alpha α
lnoremap <buffer> a4 à
lnoremap <buffer> a3 ǎ
lnoremap <buffer> a2 á
lnoremap <buffer> a1 ā
lnoremap <buffer> bvv │
let s:cpo_save=&cpo
set cpo&vim
lnoremap <buffer> bhh ─
lnoremap <buffer> bul ┘
lnoremap <buffer> bur └
lnoremap <buffer> bdl ┐
lnoremap <buffer> bdr ┌
lnoremap <buffer> beta β
lnoremap <buffer> chi χ
lnoremap <buffer> dul ┛
lnoremap <buffer> dur ┗
lnoremap <buffer> ddl ┓
lnoremap <buffer> ddr ┏
lnoremap <buffer> dhh ┉
lnoremap <buffer> dhv ┋
lnoremap <buffer> dlh ┈
lnoremap <buffer> dlv ┊
lnoremap <buffer> dac ▾
lnoremap <buffer> dae ▸
lnoremap <buffer> delta δ
lnoremap <buffer> deg °
lnoremap <buffer> eumlaut ë
lnoremap <buffer> eta η
lnoremap <buffer> epsilon ε
lnoremap <buffer> e4 è
lnoremap <buffer> e3 ě
lnoremap <buffer> e2 é
lnoremap <buffer> e1 ē
lnoremap <buffer> geq ≥
lnoremap <buffer> gamma γ
lnoremap <buffer> iumlaut ï
lnoremap <buffer> i4 ì
lnoremap <buffer> i3 ǐ
lnoremap <buffer> i2 í
lnoremap <buffer> i1 ī
lnoremap <buffer> lar ←
lnoremap <buffer> leq ≤
lnoremap <buffer> lambda λ
lnoremap <buffer> mu µ
lnoremap <buffer> oumlaut ö
lnoremap <buffer> ohm Ω
lnoremap <buffer> o4 ò
lnoremap <buffer> o3 ǒ
lnoremap <buffer> o2 ó
lnoremap <buffer> o1 ō
lnoremap <buffer> pm ±
lnoremap <buffer> phi ϕ
lnoremap <buffer> rho ρ
lnoremap <buffer> theta θ
lnoremap <buffer> uumlaut ü
lnoremap <buffer> u4 ù
lnoremap <buffer> u3 ǔ
lnoremap <buffer> u2 ú
lnoremap <buffer> u1 ū
lnoremap <buffer> zeta ζ
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=symbols
setlocal noarabic
setlocal autoindent
setlocal backupcopy=
setlocal balloonexpr=
setlocal nobinary
setlocal nobreakindent
setlocal breakindentopt=
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal cindent
setlocal cinkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
set colorcolumn=+1
setlocal colorcolumn=+1
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'c'
setlocal filetype=c
endif
setlocal fixendofline
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
set foldmethod=indent
setlocal foldmethod=indent
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal formatprg=
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=-1
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal lispwords=
set list
setlocal list
setlocal makeencoding=
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=bin,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=ccomplete#Complete
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
set relativenumber
setlocal relativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal scrolloff=-1
setlocal shiftwidth=4
setlocal noshortname
setlocal sidescrolloff=-1
setlocal signcolumn=auto
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=160
if &syntax != 'c'
setlocal syntax=c
endif
setlocal tabstop=4
setlocal tagcase=
setlocal tagfunc=
setlocal tags=
setlocal termwinkey=
setlocal termwinscroll=10000
setlocal termwinsize=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal undolevels=-123456
setlocal varsofttabstop=
setlocal vartabstop=
setlocal wincolor=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
567
normal! zo
797
normal! zo
let s:l = 571 - ((18 * winheight(0) + 8) / 16)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
571
normal! 09|
wincmd w
argglobal
if bufexists("~/chromation/dev-kit-mike/firmware/vis-spi-out/src/manual-lis-config-example.c") | buffer ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/manual-lis-config-example.c | else | edit ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/manual-lis-config-example.c | endif
lnoremap <buffer> ALPHA Α
lnoremap <buffer> BETA Β
lnoremap <buffer> CHI Χ
lnoremap <buffer> Dhh ┅
lnoremap <buffer> Dhv ┇
lnoremap <buffer> Dlh ┄
lnoremap <buffer> Dlv ┆
lnoremap <buffer> DELTA Δ
lnoremap <buffer> ETA Η
lnoremap <buffer> EPSILON Ε
lnoremap <buffer> GAMMA Γ
lnoremap <buffer> PHI Φ
lnoremap <buffer> RHO Ρ
lnoremap <buffer> THETA Θ
lnoremap <buffer> ZETA Ζ
lnoremap <buffer> aumlaut ä
lnoremap <buffer> alpha α
lnoremap <buffer> a4 à
lnoremap <buffer> a3 ǎ
lnoremap <buffer> a2 á
lnoremap <buffer> a1 ā
lnoremap <buffer> bvv │
let s:cpo_save=&cpo
set cpo&vim
lnoremap <buffer> bhh ─
lnoremap <buffer> bul ┘
lnoremap <buffer> bur └
lnoremap <buffer> bdl ┐
lnoremap <buffer> bdr ┌
lnoremap <buffer> beta β
lnoremap <buffer> chi χ
lnoremap <buffer> dul ┛
lnoremap <buffer> dur ┗
lnoremap <buffer> ddl ┓
lnoremap <buffer> ddr ┏
lnoremap <buffer> dhh ┉
lnoremap <buffer> dhv ┋
lnoremap <buffer> dlh ┈
lnoremap <buffer> dlv ┊
lnoremap <buffer> dac ▾
lnoremap <buffer> dae ▸
lnoremap <buffer> delta δ
lnoremap <buffer> deg °
lnoremap <buffer> eumlaut ë
lnoremap <buffer> eta η
lnoremap <buffer> epsilon ε
lnoremap <buffer> e4 è
lnoremap <buffer> e3 ě
lnoremap <buffer> e2 é
lnoremap <buffer> e1 ē
lnoremap <buffer> geq ≥
lnoremap <buffer> gamma γ
lnoremap <buffer> iumlaut ï
lnoremap <buffer> i4 ì
lnoremap <buffer> i3 ǐ
lnoremap <buffer> i2 í
lnoremap <buffer> i1 ī
lnoremap <buffer> lar ←
lnoremap <buffer> leq ≤
lnoremap <buffer> lambda λ
lnoremap <buffer> mu µ
lnoremap <buffer> oumlaut ö
lnoremap <buffer> ohm Ω
lnoremap <buffer> o4 ò
lnoremap <buffer> o3 ǒ
lnoremap <buffer> o2 ó
lnoremap <buffer> o1 ō
lnoremap <buffer> pm ±
lnoremap <buffer> phi ϕ
lnoremap <buffer> rho ρ
lnoremap <buffer> theta θ
lnoremap <buffer> uumlaut ü
lnoremap <buffer> u4 ù
lnoremap <buffer> u3 ǔ
lnoremap <buffer> u2 ú
lnoremap <buffer> u1 ū
lnoremap <buffer> zeta ζ
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=symbols
setlocal noarabic
setlocal autoindent
setlocal backupcopy=
setlocal balloonexpr=
setlocal nobinary
setlocal nobreakindent
setlocal breakindentopt=
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal cindent
setlocal cinkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
set colorcolumn=+1
setlocal colorcolumn=+1
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'c'
setlocal filetype=c
endif
setlocal fixendofline
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
set foldmethod=indent
setlocal foldmethod=indent
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal formatprg=
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=-1
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal lispwords=
set list
setlocal list
setlocal makeencoding=
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=bin,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=ccomplete#Complete
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
set relativenumber
setlocal relativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal scrolloff=-1
setlocal shiftwidth=4
setlocal noshortname
setlocal sidescrolloff=-1
setlocal signcolumn=auto
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=160
if &syntax != 'c'
setlocal syntax=c
endif
setlocal tabstop=4
setlocal tagcase=
setlocal tagfunc=
setlocal tags=
setlocal termwinkey=
setlocal termwinscroll=10000
setlocal termwinsize=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal undolevels=-123456
setlocal varsofttabstop=
setlocal vartabstop=
setlocal wincolor=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
5
normal! zo
7
normal! zo
let s:l = 2 - ((1 * winheight(0) + 4) / 8)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
2
normal! 0
wincmd w
argglobal
if bufexists("~/chromation/dev-kit-mike/firmware/vis-spi-out/src/SensorVis.c") | buffer ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/SensorVis.c | else | edit ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/SensorVis.c | endif
lnoremap <buffer> ALPHA Α
lnoremap <buffer> BETA Β
lnoremap <buffer> CHI Χ
lnoremap <buffer> Dhh ┅
lnoremap <buffer> Dhv ┇
lnoremap <buffer> Dlh ┄
lnoremap <buffer> Dlv ┆
lnoremap <buffer> DELTA Δ
lnoremap <buffer> ETA Η
lnoremap <buffer> EPSILON Ε
lnoremap <buffer> GAMMA Γ
lnoremap <buffer> PHI Φ
lnoremap <buffer> RHO Ρ
lnoremap <buffer> THETA Θ
lnoremap <buffer> ZETA Ζ
lnoremap <buffer> aumlaut ä
lnoremap <buffer> alpha α
lnoremap <buffer> a4 à
lnoremap <buffer> a3 ǎ
lnoremap <buffer> a2 á
lnoremap <buffer> a1 ā
lnoremap <buffer> bvv │
let s:cpo_save=&cpo
set cpo&vim
lnoremap <buffer> bhh ─
lnoremap <buffer> bul ┘
lnoremap <buffer> bur └
lnoremap <buffer> bdl ┐
lnoremap <buffer> bdr ┌
lnoremap <buffer> beta β
lnoremap <buffer> chi χ
lnoremap <buffer> dul ┛
lnoremap <buffer> dur ┗
lnoremap <buffer> ddl ┓
lnoremap <buffer> ddr ┏
lnoremap <buffer> dhh ┉
lnoremap <buffer> dhv ┋
lnoremap <buffer> dlh ┈
lnoremap <buffer> dlv ┊
lnoremap <buffer> dac ▾
lnoremap <buffer> dae ▸
lnoremap <buffer> delta δ
lnoremap <buffer> deg °
lnoremap <buffer> eumlaut ë
lnoremap <buffer> eta η
lnoremap <buffer> epsilon ε
lnoremap <buffer> e4 è
lnoremap <buffer> e3 ě
lnoremap <buffer> e2 é
lnoremap <buffer> e1 ē
lnoremap <buffer> geq ≥
lnoremap <buffer> gamma γ
lnoremap <buffer> iumlaut ï
lnoremap <buffer> i4 ì
lnoremap <buffer> i3 ǐ
lnoremap <buffer> i2 í
lnoremap <buffer> i1 ī
lnoremap <buffer> lar ←
lnoremap <buffer> leq ≤
lnoremap <buffer> lambda λ
lnoremap <buffer> mu µ
lnoremap <buffer> oumlaut ö
lnoremap <buffer> ohm Ω
lnoremap <buffer> o4 ò
lnoremap <buffer> o3 ǒ
lnoremap <buffer> o2 ó
lnoremap <buffer> o1 ō
lnoremap <buffer> pm ±
lnoremap <buffer> phi ϕ
lnoremap <buffer> rho ρ
lnoremap <buffer> theta θ
lnoremap <buffer> uumlaut ü
lnoremap <buffer> u4 ù
lnoremap <buffer> u3 ǔ
lnoremap <buffer> u2 ú
lnoremap <buffer> u1 ū
lnoremap <buffer> zeta ζ
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=symbols
setlocal noarabic
setlocal autoindent
setlocal backupcopy=
setlocal balloonexpr=
setlocal nobinary
setlocal nobreakindent
setlocal breakindentopt=
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal cindent
setlocal cinkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
set colorcolumn=+1
setlocal colorcolumn=+1
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'c'
setlocal filetype=c
endif
setlocal fixendofline
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
set foldmethod=indent
setlocal foldmethod=indent
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal formatprg=
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=-1
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal lispwords=
set list
setlocal list
setlocal makeencoding=
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=bin,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=ccomplete#Complete
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
set relativenumber
setlocal relativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal scrolloff=-1
setlocal shiftwidth=4
setlocal noshortname
setlocal sidescrolloff=-1
setlocal signcolumn=auto
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal noswapfile
setlocal synmaxcol=160
if &syntax != 'c'
setlocal syntax=c
endif
setlocal tabstop=4
setlocal tagcase=
setlocal tagfunc=
setlocal tags=
setlocal termwinkey=
setlocal termwinscroll=10000
setlocal termwinsize=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal undolevels=-123456
setlocal varsofttabstop=
setlocal vartabstop=
setlocal wincolor=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
231
normal! zo
238
normal! zo
297
normal! zo
304
normal! zo
311
normal! zo
318
normal! zo
325
normal! zo
let s:l = 231 - ((3 * winheight(0) + 3) / 7)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
231
normal! 05|
wincmd w
argglobal
if bufexists("~/chromation/dev-kit-mike/firmware/vis-spi-out/src/lis-config-old-working.lst") | buffer ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/lis-config-old-working.lst | else | edit ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/lis-config-old-working.lst | endif
lnoremap <buffer> ALPHA Α
lnoremap <buffer> BETA Β
lnoremap <buffer> CHI Χ
lnoremap <buffer> Dhh ┅
lnoremap <buffer> Dhv ┇
lnoremap <buffer> Dlh ┄
lnoremap <buffer> Dlv ┆
lnoremap <buffer> DELTA Δ
lnoremap <buffer> ETA Η
lnoremap <buffer> EPSILON Ε
lnoremap <buffer> GAMMA Γ
lnoremap <buffer> PHI Φ
lnoremap <buffer> RHO Ρ
lnoremap <buffer> THETA Θ
lnoremap <buffer> ZETA Ζ
lnoremap <buffer> aumlaut ä
lnoremap <buffer> alpha α
lnoremap <buffer> a4 à
lnoremap <buffer> a3 ǎ
lnoremap <buffer> a2 á
lnoremap <buffer> a1 ā
lnoremap <buffer> bvv │
let s:cpo_save=&cpo
set cpo&vim
lnoremap <buffer> bhh ─
lnoremap <buffer> bul ┘
lnoremap <buffer> bur └
lnoremap <buffer> bdl ┐
lnoremap <buffer> bdr ┌
lnoremap <buffer> beta β
lnoremap <buffer> chi χ
lnoremap <buffer> dul ┛
lnoremap <buffer> dur ┗
lnoremap <buffer> ddl ┓
lnoremap <buffer> ddr ┏
lnoremap <buffer> dhh ┉
lnoremap <buffer> dhv ┋
lnoremap <buffer> dlh ┈
lnoremap <buffer> dlv ┊
lnoremap <buffer> dac ▾
lnoremap <buffer> dae ▸
lnoremap <buffer> delta δ
lnoremap <buffer> deg °
lnoremap <buffer> eumlaut ë
lnoremap <buffer> eta η
lnoremap <buffer> epsilon ε
lnoremap <buffer> e4 è
lnoremap <buffer> e3 ě
lnoremap <buffer> e2 é
lnoremap <buffer> e1 ē
lnoremap <buffer> geq ≥
lnoremap <buffer> gamma γ
lnoremap <buffer> iumlaut ï
lnoremap <buffer> i4 ì
lnoremap <buffer> i3 ǐ
lnoremap <buffer> i2 í
lnoremap <buffer> i1 ī
lnoremap <buffer> lar ←
lnoremap <buffer> leq ≤
lnoremap <buffer> lambda λ
lnoremap <buffer> mu µ
lnoremap <buffer> oumlaut ö
lnoremap <buffer> ohm Ω
lnoremap <buffer> o4 ò
lnoremap <buffer> o3 ǒ
lnoremap <buffer> o2 ó
lnoremap <buffer> o1 ō
lnoremap <buffer> pm ±
lnoremap <buffer> phi ϕ
lnoremap <buffer> rho ρ
lnoremap <buffer> theta θ
lnoremap <buffer> uumlaut ü
lnoremap <buffer> u4 ù
lnoremap <buffer> u3 ǔ
lnoremap <buffer> u2 ú
lnoremap <buffer> u1 ū
lnoremap <buffer> zeta ζ
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=symbols
setlocal noarabic
setlocal autoindent
setlocal backupcopy=
setlocal balloonexpr=
setlocal nobinary
setlocal nobreakindent
setlocal breakindentopt=
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
set colorcolumn=+1
setlocal colorcolumn=+1
setlocal comments=s1:/*,mb:*,ex:*/,://,b:#,:%,:XCOMM,n:>,fb:-
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'asm'
setlocal filetype=asm
endif
setlocal fixendofline
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
set foldmethod=indent
setlocal foldmethod=indent
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=tcq
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal formatprg=
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=-1
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal lispwords=
set list
setlocal list
setlocal makeencoding=
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=bin,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
set relativenumber
setlocal relativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal scrolloff=-1
setlocal shiftwidth=4
setlocal noshortname
setlocal sidescrolloff=-1
setlocal signcolumn=auto
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=160
if &syntax != 'asm'
setlocal syntax=asm
endif
setlocal tabstop=4
setlocal tagcase=
setlocal tagfunc=
setlocal tags=
setlocal termwinkey=
setlocal termwinscroll=10000
setlocal termwinsize=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal undolevels=-123456
setlocal varsofttabstop=
setlocal vartabstop=
setlocal wincolor=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
let s:l = 25 - ((9 * winheight(0) + 10) / 20)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
25
normal! 03|
wincmd w
argglobal
if bufexists("~/chromation/dev-kit-mike/firmware/vis-spi-out/src/lis-config-new-fixed.lst") | buffer ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/lis-config-new-fixed.lst | else | edit ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/lis-config-new-fixed.lst | endif
lnoremap <buffer> ALPHA Α
lnoremap <buffer> BETA Β
lnoremap <buffer> CHI Χ
lnoremap <buffer> Dhh ┅
lnoremap <buffer> Dhv ┇
lnoremap <buffer> Dlh ┄
lnoremap <buffer> Dlv ┆
lnoremap <buffer> DELTA Δ
lnoremap <buffer> ETA Η
lnoremap <buffer> EPSILON Ε
lnoremap <buffer> GAMMA Γ
lnoremap <buffer> PHI Φ
lnoremap <buffer> RHO Ρ
lnoremap <buffer> THETA Θ
lnoremap <buffer> ZETA Ζ
lnoremap <buffer> aumlaut ä
lnoremap <buffer> alpha α
lnoremap <buffer> a4 à
lnoremap <buffer> a3 ǎ
lnoremap <buffer> a2 á
lnoremap <buffer> a1 ā
lnoremap <buffer> bvv │
let s:cpo_save=&cpo
set cpo&vim
lnoremap <buffer> bhh ─
lnoremap <buffer> bul ┘
lnoremap <buffer> bur └
lnoremap <buffer> bdl ┐
lnoremap <buffer> bdr ┌
lnoremap <buffer> beta β
lnoremap <buffer> chi χ
lnoremap <buffer> dul ┛
lnoremap <buffer> dur ┗
lnoremap <buffer> ddl ┓
lnoremap <buffer> ddr ┏
lnoremap <buffer> dhh ┉
lnoremap <buffer> dhv ┋
lnoremap <buffer> dlh ┈
lnoremap <buffer> dlv ┊
lnoremap <buffer> dac ▾
lnoremap <buffer> dae ▸
lnoremap <buffer> delta δ
lnoremap <buffer> deg °
lnoremap <buffer> eumlaut ë
lnoremap <buffer> eta η
lnoremap <buffer> epsilon ε
lnoremap <buffer> e4 è
lnoremap <buffer> e3 ě
lnoremap <buffer> e2 é
lnoremap <buffer> e1 ē
lnoremap <buffer> geq ≥
lnoremap <buffer> gamma γ
lnoremap <buffer> iumlaut ï
lnoremap <buffer> i4 ì
lnoremap <buffer> i3 ǐ
lnoremap <buffer> i2 í
lnoremap <buffer> i1 ī
lnoremap <buffer> lar ←
lnoremap <buffer> leq ≤
lnoremap <buffer> lambda λ
lnoremap <buffer> mu µ
lnoremap <buffer> oumlaut ö
lnoremap <buffer> ohm Ω
lnoremap <buffer> o4 ò
lnoremap <buffer> o3 ǒ
lnoremap <buffer> o2 ó
lnoremap <buffer> o1 ō
lnoremap <buffer> pm ±
lnoremap <buffer> phi ϕ
lnoremap <buffer> rho ρ
lnoremap <buffer> theta θ
lnoremap <buffer> uumlaut ü
lnoremap <buffer> u4 ù
lnoremap <buffer> u3 ǔ
lnoremap <buffer> u2 ú
lnoremap <buffer> u1 ū
lnoremap <buffer> zeta ζ
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=symbols
setlocal noarabic
setlocal autoindent
setlocal backupcopy=
setlocal balloonexpr=
setlocal nobinary
setlocal nobreakindent
setlocal breakindentopt=
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
set colorcolumn=+1
setlocal colorcolumn=+1
setlocal comments=s1:/*,mb:*,ex:*/,://,b:#,:%,:XCOMM,n:>,fb:-
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'asm'
setlocal filetype=asm
endif
setlocal fixendofline
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
set foldmethod=indent
setlocal foldmethod=indent
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=tcq
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal formatprg=
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=-1
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal lispwords=
set list
setlocal list
setlocal makeencoding=
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=bin,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
set relativenumber
setlocal relativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal scrolloff=-1
setlocal shiftwidth=4
setlocal noshortname
setlocal sidescrolloff=-1
setlocal signcolumn=auto
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=160
if &syntax != 'asm'
setlocal syntax=asm
endif
setlocal tabstop=4
setlocal tagcase=
setlocal tagfunc=
setlocal tags=
setlocal termwinkey=
setlocal termwinscroll=10000
setlocal termwinsize=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal undolevels=-123456
setlocal varsofttabstop=
setlocal vartabstop=
setlocal wincolor=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
let s:l = 7 - ((0 * winheight(0) + 4) / 8)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
7
normal! 0
wincmd w
argglobal
if bufexists("~/chromation/dev-kit-mike/firmware/vis-spi-out/src/lis-config-new-broken.lst") | buffer ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/lis-config-new-broken.lst | else | edit ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/lis-config-new-broken.lst | endif
lnoremap <buffer> ALPHA Α
lnoremap <buffer> BETA Β
lnoremap <buffer> CHI Χ
lnoremap <buffer> Dhh ┅
lnoremap <buffer> Dhv ┇
lnoremap <buffer> Dlh ┄
lnoremap <buffer> Dlv ┆
lnoremap <buffer> DELTA Δ
lnoremap <buffer> ETA Η
lnoremap <buffer> EPSILON Ε
lnoremap <buffer> GAMMA Γ
lnoremap <buffer> PHI Φ
lnoremap <buffer> RHO Ρ
lnoremap <buffer> THETA Θ
lnoremap <buffer> ZETA Ζ
lnoremap <buffer> aumlaut ä
lnoremap <buffer> alpha α
lnoremap <buffer> a4 à
lnoremap <buffer> a3 ǎ
lnoremap <buffer> a2 á
lnoremap <buffer> a1 ā
lnoremap <buffer> bvv │
let s:cpo_save=&cpo
set cpo&vim
lnoremap <buffer> bhh ─
lnoremap <buffer> bul ┘
lnoremap <buffer> bur └
lnoremap <buffer> bdl ┐
lnoremap <buffer> bdr ┌
lnoremap <buffer> beta β
lnoremap <buffer> chi χ
lnoremap <buffer> dul ┛
lnoremap <buffer> dur ┗
lnoremap <buffer> ddl ┓
lnoremap <buffer> ddr ┏
lnoremap <buffer> dhh ┉
lnoremap <buffer> dhv ┋
lnoremap <buffer> dlh ┈
lnoremap <buffer> dlv ┊
lnoremap <buffer> dac ▾
lnoremap <buffer> dae ▸
lnoremap <buffer> delta δ
lnoremap <buffer> deg °
lnoremap <buffer> eumlaut ë
lnoremap <buffer> eta η
lnoremap <buffer> epsilon ε
lnoremap <buffer> e4 è
lnoremap <buffer> e3 ě
lnoremap <buffer> e2 é
lnoremap <buffer> e1 ē
lnoremap <buffer> geq ≥
lnoremap <buffer> gamma γ
lnoremap <buffer> iumlaut ï
lnoremap <buffer> i4 ì
lnoremap <buffer> i3 ǐ
lnoremap <buffer> i2 í
lnoremap <buffer> i1 ī
lnoremap <buffer> lar ←
lnoremap <buffer> leq ≤
lnoremap <buffer> lambda λ
lnoremap <buffer> mu µ
lnoremap <buffer> oumlaut ö
lnoremap <buffer> ohm Ω
lnoremap <buffer> o4 ò
lnoremap <buffer> o3 ǒ
lnoremap <buffer> o2 ó
lnoremap <buffer> o1 ō
lnoremap <buffer> pm ±
lnoremap <buffer> phi ϕ
lnoremap <buffer> rho ρ
lnoremap <buffer> theta θ
lnoremap <buffer> uumlaut ü
lnoremap <buffer> u4 ù
lnoremap <buffer> u3 ǔ
lnoremap <buffer> u2 ú
lnoremap <buffer> u1 ū
lnoremap <buffer> zeta ζ
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=symbols
setlocal noarabic
setlocal autoindent
setlocal backupcopy=
setlocal balloonexpr=
setlocal nobinary
setlocal nobreakindent
setlocal breakindentopt=
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
set colorcolumn=+1
setlocal colorcolumn=+1
setlocal comments=s1:/*,mb:*,ex:*/,://,b:#,:%,:XCOMM,n:>,fb:-
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'asm'
setlocal filetype=asm
endif
setlocal fixendofline
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
set foldmethod=indent
setlocal foldmethod=indent
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=tcq
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal formatprg=
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=-1
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal lispwords=
set list
setlocal list
setlocal makeencoding=
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=bin,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
set relativenumber
setlocal relativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal scrolloff=-1
setlocal shiftwidth=4
setlocal noshortname
setlocal sidescrolloff=-1
setlocal signcolumn=auto
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=160
if &syntax != 'asm'
setlocal syntax=asm
endif
setlocal tabstop=4
setlocal tagcase=
setlocal tagfunc=
setlocal tags=
setlocal termwinkey=
setlocal termwinscroll=10000
setlocal termwinsize=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal undolevels=-123456
setlocal varsofttabstop=
setlocal vartabstop=
setlocal wincolor=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
let s:l = 34 - ((5 * winheight(0) + 5) / 11)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
34
normal! 050|
wincmd w
exe '1resize ' . ((&lines * 16 + 20) / 41)
exe 'vert 1resize ' . ((&columns * 79 + 88) / 177)
exe '2resize ' . ((&lines * 8 + 20) / 41)
exe 'vert 2resize ' . ((&columns * 97 + 88) / 177)
exe '3resize ' . ((&lines * 7 + 20) / 41)
exe 'vert 3resize ' . ((&columns * 97 + 88) / 177)
exe '4resize ' . ((&lines * 20 + 20) / 41)
exe 'vert 4resize ' . ((&columns * 79 + 88) / 177)
exe '5resize ' . ((&lines * 8 + 20) / 41)
exe 'vert 5resize ' . ((&columns * 97 + 88) / 177)
exe '6resize ' . ((&lines * 11 + 20) / 41)
exe 'vert 6resize ' . ((&columns * 97 + 88) / 177)
tabnext
edit ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/SensorVis.c
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd _ | wincmd |
split
wincmd _ | wincmd |
split
2wincmd k
wincmd w
wincmd w
wincmd w
wincmd _ | wincmd |
split
wincmd _ | wincmd |
split
2wincmd k
wincmd w
wincmd w
set nosplitbelow
wincmd t
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe '1resize ' . ((&lines * 10 + 20) / 41)
exe 'vert 1resize ' . ((&columns * 79 + 88) / 177)
exe '2resize ' . ((&lines * 11 + 20) / 41)
exe 'vert 2resize ' . ((&columns * 79 + 88) / 177)
exe '3resize ' . ((&lines * 14 + 20) / 41)
exe 'vert 3resize ' . ((&columns * 79 + 88) / 177)
exe '4resize ' . ((&lines * 12 + 20) / 41)
exe 'vert 4resize ' . ((&columns * 97 + 88) / 177)
exe '5resize ' . ((&lines * 11 + 20) / 41)
exe 'vert 5resize ' . ((&columns * 97 + 88) / 177)
exe '6resize ' . ((&lines * 12 + 20) / 41)
exe 'vert 6resize ' . ((&columns * 97 + 88) / 177)
argglobal
lnoremap <buffer> ALPHA Α
lnoremap <buffer> BETA Β
lnoremap <buffer> CHI Χ
lnoremap <buffer> Dhh ┅
lnoremap <buffer> Dhv ┇
lnoremap <buffer> Dlh ┄
lnoremap <buffer> Dlv ┆
lnoremap <buffer> DELTA Δ
lnoremap <buffer> ETA Η
lnoremap <buffer> EPSILON Ε
lnoremap <buffer> GAMMA Γ
lnoremap <buffer> PHI Φ
lnoremap <buffer> RHO Ρ
lnoremap <buffer> THETA Θ
lnoremap <buffer> ZETA Ζ
lnoremap <buffer> aumlaut ä
lnoremap <buffer> alpha α
lnoremap <buffer> a4 à
lnoremap <buffer> a3 ǎ
lnoremap <buffer> a2 á
lnoremap <buffer> a1 ā
lnoremap <buffer> bvv │
let s:cpo_save=&cpo
set cpo&vim
lnoremap <buffer> bhh ─
lnoremap <buffer> bul ┘
lnoremap <buffer> bur └
lnoremap <buffer> bdl ┐
lnoremap <buffer> bdr ┌
lnoremap <buffer> beta β
lnoremap <buffer> chi χ
lnoremap <buffer> dul ┛
lnoremap <buffer> dur ┗
lnoremap <buffer> ddl ┓
lnoremap <buffer> ddr ┏
lnoremap <buffer> dhh ┉
lnoremap <buffer> dhv ┋
lnoremap <buffer> dlh ┈
lnoremap <buffer> dlv ┊
lnoremap <buffer> dac ▾
lnoremap <buffer> dae ▸
lnoremap <buffer> delta δ
lnoremap <buffer> deg °
lnoremap <buffer> eumlaut ë
lnoremap <buffer> eta η
lnoremap <buffer> epsilon ε
lnoremap <buffer> e4 è
lnoremap <buffer> e3 ě
lnoremap <buffer> e2 é
lnoremap <buffer> e1 ē
lnoremap <buffer> geq ≥
lnoremap <buffer> gamma γ
lnoremap <buffer> iumlaut ï
lnoremap <buffer> i4 ì
lnoremap <buffer> i3 ǐ
lnoremap <buffer> i2 í
lnoremap <buffer> i1 ī
lnoremap <buffer> lar ←
lnoremap <buffer> leq ≤
lnoremap <buffer> lambda λ
lnoremap <buffer> mu µ
lnoremap <buffer> oumlaut ö
lnoremap <buffer> ohm Ω
lnoremap <buffer> o4 ò
lnoremap <buffer> o3 ǒ
lnoremap <buffer> o2 ó
lnoremap <buffer> o1 ō
lnoremap <buffer> pm ±
lnoremap <buffer> phi ϕ
lnoremap <buffer> rho ρ
lnoremap <buffer> theta θ
lnoremap <buffer> uumlaut ü
lnoremap <buffer> u4 ù
lnoremap <buffer> u3 ǔ
lnoremap <buffer> u2 ú
lnoremap <buffer> u1 ū
lnoremap <buffer> zeta ζ
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=symbols
setlocal noarabic
setlocal autoindent
setlocal backupcopy=
setlocal balloonexpr=
setlocal nobinary
setlocal nobreakindent
setlocal breakindentopt=
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal cindent
setlocal cinkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
set colorcolumn=+1
setlocal colorcolumn=+1
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'c'
setlocal filetype=c
endif
setlocal fixendofline
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
set foldmethod=indent
setlocal foldmethod=indent
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal formatprg=
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=-1
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal lispwords=
set list
setlocal list
setlocal makeencoding=
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=bin,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=ccomplete#Complete
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
set relativenumber
setlocal relativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal scrolloff=-1
setlocal shiftwidth=4
setlocal noshortname
setlocal sidescrolloff=-1
setlocal signcolumn=auto
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal noswapfile
setlocal synmaxcol=160
if &syntax != 'c'
setlocal syntax=c
endif
setlocal tabstop=4
setlocal tagcase=
setlocal tagfunc=
setlocal tags=
setlocal termwinkey=
setlocal termwinscroll=10000
setlocal termwinsize=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal undolevels=-123456
setlocal varsofttabstop=
setlocal vartabstop=
setlocal wincolor=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
55
normal! zo
231
normal! zo
238
normal! zo
238
normal! zc
334
normal! zo
334
normal! zc
376
normal! zo
396
normal! zo
412
normal! zo
let s:l = 55 - ((0 * winheight(0) + 5) / 10)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
55
normal! 05|
wincmd w
argglobal
if bufexists("/cygdrive/c/Program\ Files\ (x86)/Atmel/Studio/7.0/toolchain/avr8/avr8-gnu-toolchain/avr/include/avr/iom328p.h") | buffer /cygdrive/c/Program\ Files\ (x86)/Atmel/Studio/7.0/toolchain/avr8/avr8-gnu-toolchain/avr/include/avr/iom328p.h | else | edit /cygdrive/c/Program\ Files\ (x86)/Atmel/Studio/7.0/toolchain/avr8/avr8-gnu-toolchain/avr/include/avr/iom328p.h | endif
lnoremap <buffer> ALPHA Α
lnoremap <buffer> BETA Β
lnoremap <buffer> CHI Χ
lnoremap <buffer> Dhh ┅
lnoremap <buffer> Dhv ┇
lnoremap <buffer> Dlh ┄
lnoremap <buffer> Dlv ┆
lnoremap <buffer> DELTA Δ
lnoremap <buffer> ETA Η
lnoremap <buffer> EPSILON Ε
lnoremap <buffer> GAMMA Γ
lnoremap <buffer> PHI Φ
lnoremap <buffer> RHO Ρ
lnoremap <buffer> THETA Θ
lnoremap <buffer> ZETA Ζ
lnoremap <buffer> aumlaut ä
lnoremap <buffer> alpha α
lnoremap <buffer> a4 à
lnoremap <buffer> a3 ǎ
lnoremap <buffer> a2 á
lnoremap <buffer> a1 ā
lnoremap <buffer> bvv │
let s:cpo_save=&cpo
set cpo&vim
lnoremap <buffer> bhh ─
lnoremap <buffer> bul ┘
lnoremap <buffer> bur └
lnoremap <buffer> bdl ┐
lnoremap <buffer> bdr ┌
lnoremap <buffer> beta β
lnoremap <buffer> chi χ
lnoremap <buffer> dul ┛
lnoremap <buffer> dur ┗
lnoremap <buffer> ddl ┓
lnoremap <buffer> ddr ┏
lnoremap <buffer> dhh ┉
lnoremap <buffer> dhv ┋
lnoremap <buffer> dlh ┈
lnoremap <buffer> dlv ┊
lnoremap <buffer> dac ▾
lnoremap <buffer> dae ▸
lnoremap <buffer> delta δ
lnoremap <buffer> deg °
lnoremap <buffer> eumlaut ë
lnoremap <buffer> eta η
lnoremap <buffer> epsilon ε
lnoremap <buffer> e4 è
lnoremap <buffer> e3 ě
lnoremap <buffer> e2 é
lnoremap <buffer> e1 ē
lnoremap <buffer> geq ≥
lnoremap <buffer> gamma γ
lnoremap <buffer> iumlaut ï
lnoremap <buffer> i4 ì
lnoremap <buffer> i3 ǐ
lnoremap <buffer> i2 í
lnoremap <buffer> i1 ī
lnoremap <buffer> lar ←
lnoremap <buffer> leq ≤
lnoremap <buffer> lambda λ
lnoremap <buffer> mu µ
lnoremap <buffer> oumlaut ö
lnoremap <buffer> ohm Ω
lnoremap <buffer> o4 ò
lnoremap <buffer> o3 ǒ
lnoremap <buffer> o2 ó
lnoremap <buffer> o1 ō
lnoremap <buffer> pm ±
lnoremap <buffer> phi ϕ
lnoremap <buffer> rho ρ
lnoremap <buffer> theta θ
lnoremap <buffer> uumlaut ü
lnoremap <buffer> u4 ù
lnoremap <buffer> u3 ǔ
lnoremap <buffer> u2 ú
lnoremap <buffer> u1 ū
lnoremap <buffer> zeta ζ
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=symbols
setlocal noarabic
setlocal autoindent
setlocal backupcopy=
setlocal balloonexpr=
setlocal nobinary
setlocal nobreakindent
setlocal breakindentopt=
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal cindent
setlocal cinkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
set colorcolumn=+1
setlocal colorcolumn=+1
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'cpp'
setlocal filetype=cpp
endif
setlocal fixendofline
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
set foldmethod=indent
setlocal foldmethod=indent
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal formatprg=
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=-1
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal lispwords=
set list
setlocal list
setlocal makeencoding=
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=bin,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=ccomplete#Complete
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal readonly
set relativenumber
setlocal relativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal scrolloff=-1
setlocal shiftwidth=4
setlocal noshortname
setlocal sidescrolloff=-1
setlocal signcolumn=auto
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=160
if &syntax != 'cpp'
setlocal syntax=cpp
endif
setlocal tabstop=4
setlocal tagcase=
setlocal tagfunc=
setlocal tags=
setlocal termwinkey=
setlocal termwinscroll=10000
setlocal termwinsize=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal undolevels=-123456
setlocal varsofttabstop=
setlocal vartabstop=
setlocal wincolor=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
let s:l = 302 - ((5 * winheight(0) + 5) / 11)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
302
normal! 09|
wincmd w
argglobal
if bufexists("~/chromation/dev-kit-mike/firmware/vis-spi-out/src/Spi-Hardware.h") | buffer ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/Spi-Hardware.h | else | edit ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/Spi-Hardware.h | endif
lnoremap <buffer> ALPHA Α
lnoremap <buffer> BETA Β
lnoremap <buffer> CHI Χ
lnoremap <buffer> Dhh ┅
lnoremap <buffer> Dhv ┇
lnoremap <buffer> Dlh ┄
lnoremap <buffer> Dlv ┆
lnoremap <buffer> DELTA Δ
lnoremap <buffer> ETA Η
lnoremap <buffer> EPSILON Ε
lnoremap <buffer> GAMMA Γ
lnoremap <buffer> PHI Φ
lnoremap <buffer> RHO Ρ
lnoremap <buffer> THETA Θ
lnoremap <buffer> ZETA Ζ
lnoremap <buffer> aumlaut ä
lnoremap <buffer> alpha α
lnoremap <buffer> a4 à
lnoremap <buffer> a3 ǎ
lnoremap <buffer> a2 á
lnoremap <buffer> a1 ā
lnoremap <buffer> bvv │
let s:cpo_save=&cpo
set cpo&vim
lnoremap <buffer> bhh ─
lnoremap <buffer> bul ┘
lnoremap <buffer> bur └
lnoremap <buffer> bdl ┐
lnoremap <buffer> bdr ┌
lnoremap <buffer> beta β
lnoremap <buffer> chi χ
lnoremap <buffer> dul ┛
lnoremap <buffer> dur ┗
lnoremap <buffer> ddl ┓
lnoremap <buffer> ddr ┏
lnoremap <buffer> dhh ┉
lnoremap <buffer> dhv ┋
lnoremap <buffer> dlh ┈
lnoremap <buffer> dlv ┊
lnoremap <buffer> dac ▾
lnoremap <buffer> dae ▸
lnoremap <buffer> delta δ
lnoremap <buffer> deg °
lnoremap <buffer> eumlaut ë
lnoremap <buffer> eta η
lnoremap <buffer> epsilon ε
lnoremap <buffer> e4 è
lnoremap <buffer> e3 ě
lnoremap <buffer> e2 é
lnoremap <buffer> e1 ē
lnoremap <buffer> geq ≥
lnoremap <buffer> gamma γ
lnoremap <buffer> iumlaut ï
lnoremap <buffer> i4 ì
lnoremap <buffer> i3 ǐ
lnoremap <buffer> i2 í
lnoremap <buffer> i1 ī
lnoremap <buffer> lar ←
lnoremap <buffer> leq ≤
lnoremap <buffer> lambda λ
lnoremap <buffer> mu µ
lnoremap <buffer> oumlaut ö
lnoremap <buffer> ohm Ω
lnoremap <buffer> o4 ò
lnoremap <buffer> o3 ǒ
lnoremap <buffer> o2 ó
lnoremap <buffer> o1 ō
lnoremap <buffer> pm ±
lnoremap <buffer> phi ϕ
lnoremap <buffer> rho ρ
lnoremap <buffer> theta θ
lnoremap <buffer> uumlaut ü
lnoremap <buffer> u4 ù
lnoremap <buffer> u3 ǔ
lnoremap <buffer> u2 ú
lnoremap <buffer> u1 ū
lnoremap <buffer> zeta ζ
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=symbols
setlocal noarabic
setlocal autoindent
setlocal backupcopy=
setlocal balloonexpr=
setlocal nobinary
setlocal nobreakindent
setlocal breakindentopt=
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal cindent
setlocal cinkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
set colorcolumn=+1
setlocal colorcolumn=+1
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'cpp'
setlocal filetype=cpp
endif
setlocal fixendofline
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
set foldmethod=indent
setlocal foldmethod=indent
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal formatprg=
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=-1
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal lispwords=
set list
setlocal list
setlocal makeencoding=
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=bin,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=ccomplete#Complete
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
set relativenumber
setlocal relativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal scrolloff=-1
setlocal shiftwidth=4
setlocal noshortname
setlocal sidescrolloff=-1
setlocal signcolumn=auto
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=160
if &syntax != 'cpp'
setlocal syntax=cpp
endif
setlocal tabstop=4
setlocal tagcase=
setlocal tagfunc=
setlocal tags=
setlocal termwinkey=
setlocal termwinscroll=10000
setlocal termwinsize=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal undolevels=-123456
setlocal varsofttabstop=
setlocal vartabstop=
setlocal wincolor=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
let s:l = 30 - ((22 * winheight(0) + 7) / 14)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
30
normal! 041|
wincmd w
argglobal
if bufexists("~/chromation/dev-kit-mike/firmware/vis-spi-out/src/WriteSpiMaster-is-a-func-ptr.lst") | buffer ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/WriteSpiMaster-is-a-func-ptr.lst | else | edit ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/WriteSpiMaster-is-a-func-ptr.lst | endif
lnoremap <buffer> ALPHA Α
lnoremap <buffer> BETA Β
lnoremap <buffer> CHI Χ
lnoremap <buffer> Dhh ┅
lnoremap <buffer> Dhv ┇
lnoremap <buffer> Dlh ┄
lnoremap <buffer> Dlv ┆
lnoremap <buffer> DELTA Δ
lnoremap <buffer> ETA Η
lnoremap <buffer> EPSILON Ε
lnoremap <buffer> GAMMA Γ
lnoremap <buffer> PHI Φ
lnoremap <buffer> RHO Ρ
lnoremap <buffer> THETA Θ
lnoremap <buffer> ZETA Ζ
lnoremap <buffer> aumlaut ä
lnoremap <buffer> alpha α
lnoremap <buffer> a4 à
lnoremap <buffer> a3 ǎ
lnoremap <buffer> a2 á
lnoremap <buffer> a1 ā
lnoremap <buffer> bvv │
let s:cpo_save=&cpo
set cpo&vim
lnoremap <buffer> bhh ─
lnoremap <buffer> bul ┘
lnoremap <buffer> bur └
lnoremap <buffer> bdl ┐
lnoremap <buffer> bdr ┌
lnoremap <buffer> beta β
lnoremap <buffer> chi χ
lnoremap <buffer> dul ┛
lnoremap <buffer> dur ┗
lnoremap <buffer> ddl ┓
lnoremap <buffer> ddr ┏
lnoremap <buffer> dhh ┉
lnoremap <buffer> dhv ┋
lnoremap <buffer> dlh ┈
lnoremap <buffer> dlv ┊
lnoremap <buffer> dac ▾
lnoremap <buffer> dae ▸
lnoremap <buffer> delta δ
lnoremap <buffer> deg °
lnoremap <buffer> eumlaut ë
lnoremap <buffer> eta η
lnoremap <buffer> epsilon ε
lnoremap <buffer> e4 è
lnoremap <buffer> e3 ě
lnoremap <buffer> e2 é
lnoremap <buffer> e1 ē
lnoremap <buffer> geq ≥
lnoremap <buffer> gamma γ
lnoremap <buffer> iumlaut ï
lnoremap <buffer> i4 ì
lnoremap <buffer> i3 ǐ
lnoremap <buffer> i2 í
lnoremap <buffer> i1 ī
lnoremap <buffer> lar ←
lnoremap <buffer> leq ≤
lnoremap <buffer> lambda λ
lnoremap <buffer> mu µ
lnoremap <buffer> oumlaut ö
lnoremap <buffer> ohm Ω
lnoremap <buffer> o4 ò
lnoremap <buffer> o3 ǒ
lnoremap <buffer> o2 ó
lnoremap <buffer> o1 ō
lnoremap <buffer> pm ±
lnoremap <buffer> phi ϕ
lnoremap <buffer> rho ρ
lnoremap <buffer> theta θ
lnoremap <buffer> uumlaut ü
lnoremap <buffer> u4 ù
lnoremap <buffer> u3 ǔ
lnoremap <buffer> u2 ú
lnoremap <buffer> u1 ū
lnoremap <buffer> zeta ζ
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=symbols
setlocal noarabic
setlocal autoindent
setlocal backupcopy=
setlocal balloonexpr=
setlocal nobinary
setlocal nobreakindent
setlocal breakindentopt=
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
set colorcolumn=+1
setlocal colorcolumn=+1
setlocal comments=s1:/*,mb:*,ex:*/,://,b:#,:%,:XCOMM,n:>,fb:-
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'asm'
setlocal filetype=asm
endif
setlocal fixendofline
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
set foldmethod=indent
setlocal foldmethod=indent
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=tcq
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal formatprg=
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=-1
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal lispwords=
set list
setlocal list
setlocal makeencoding=
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=bin,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
set relativenumber
setlocal relativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal scrolloff=-1
setlocal shiftwidth=4
setlocal noshortname
setlocal sidescrolloff=-1
setlocal signcolumn=auto
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=160
if &syntax != 'asm'
setlocal syntax=asm
endif
setlocal tabstop=4
setlocal tagcase=
setlocal tagfunc=
setlocal tags=
setlocal termwinkey=
setlocal termwinscroll=10000
setlocal termwinsize=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal undolevels=-123456
setlocal varsofttabstop=
setlocal vartabstop=
setlocal wincolor=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
3
normal! zo
8
normal! zo
13
normal! zo
let s:l = 33 - ((0 * winheight(0) + 6) / 12)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
33
normal! 029|
wincmd w
argglobal
if bufexists("~/chromation/dev-kit-mike/firmware/vis-spi-out/build/vis-spi-out.lst") | buffer ~/chromation/dev-kit-mike/firmware/vis-spi-out/build/vis-spi-out.lst | else | edit ~/chromation/dev-kit-mike/firmware/vis-spi-out/build/vis-spi-out.lst | endif
lnoremap <buffer> ALPHA Α
lnoremap <buffer> BETA Β
lnoremap <buffer> CHI Χ
lnoremap <buffer> Dhh ┅
lnoremap <buffer> Dhv ┇
lnoremap <buffer> Dlh ┄
lnoremap <buffer> Dlv ┆
lnoremap <buffer> DELTA Δ
lnoremap <buffer> ETA Η
lnoremap <buffer> EPSILON Ε
lnoremap <buffer> GAMMA Γ
lnoremap <buffer> PHI Φ
lnoremap <buffer> RHO Ρ
lnoremap <buffer> THETA Θ
lnoremap <buffer> ZETA Ζ
lnoremap <buffer> aumlaut ä
lnoremap <buffer> alpha α
lnoremap <buffer> a4 à
lnoremap <buffer> a3 ǎ
lnoremap <buffer> a2 á
lnoremap <buffer> a1 ā
lnoremap <buffer> bvv │
let s:cpo_save=&cpo
set cpo&vim
lnoremap <buffer> bhh ─
lnoremap <buffer> bul ┘
lnoremap <buffer> bur └
lnoremap <buffer> bdl ┐
lnoremap <buffer> bdr ┌
lnoremap <buffer> beta β
lnoremap <buffer> chi χ
lnoremap <buffer> dul ┛
lnoremap <buffer> dur ┗
lnoremap <buffer> ddl ┓
lnoremap <buffer> ddr ┏
lnoremap <buffer> dhh ┉
lnoremap <buffer> dhv ┋
lnoremap <buffer> dlh ┈
lnoremap <buffer> dlv ┊
lnoremap <buffer> dac ▾
lnoremap <buffer> dae ▸
lnoremap <buffer> delta δ
lnoremap <buffer> deg °
lnoremap <buffer> eumlaut ë
lnoremap <buffer> eta η
lnoremap <buffer> epsilon ε
lnoremap <buffer> e4 è
lnoremap <buffer> e3 ě
lnoremap <buffer> e2 é
lnoremap <buffer> e1 ē
lnoremap <buffer> geq ≥
lnoremap <buffer> gamma γ
lnoremap <buffer> iumlaut ï
lnoremap <buffer> i4 ì
lnoremap <buffer> i3 ǐ
lnoremap <buffer> i2 í
lnoremap <buffer> i1 ī
lnoremap <buffer> lar ←
lnoremap <buffer> leq ≤
lnoremap <buffer> lambda λ
lnoremap <buffer> mu µ
lnoremap <buffer> oumlaut ö
lnoremap <buffer> ohm Ω
lnoremap <buffer> o4 ò
lnoremap <buffer> o3 ǒ
lnoremap <buffer> o2 ó
lnoremap <buffer> o1 ō
lnoremap <buffer> pm ±
lnoremap <buffer> phi ϕ
lnoremap <buffer> rho ρ
lnoremap <buffer> theta θ
lnoremap <buffer> uumlaut ü
lnoremap <buffer> u4 ù
lnoremap <buffer> u3 ǔ
lnoremap <buffer> u2 ú
lnoremap <buffer> u1 ū
lnoremap <buffer> zeta ζ
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=symbols
setlocal noarabic
setlocal autoindent
setlocal backupcopy=
setlocal balloonexpr=
setlocal nobinary
setlocal nobreakindent
setlocal breakindentopt=
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
set colorcolumn=+1
setlocal colorcolumn=+1
setlocal comments=s1:/*,mb:*,ex:*/,://,b:#,:%,:XCOMM,n:>,fb:-
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'asm'
setlocal filetype=asm
endif
setlocal fixendofline
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
set foldmethod=indent
setlocal foldmethod=indent
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=tcq
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal formatprg=
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=-1
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal lispwords=
set list
setlocal list
setlocal makeencoding=
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=bin,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
set relativenumber
setlocal relativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal scrolloff=-1
setlocal shiftwidth=4
setlocal noshortname
setlocal sidescrolloff=-1
setlocal signcolumn=auto
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=160
if &syntax != 'asm'
setlocal syntax=asm
endif
setlocal tabstop=4
setlocal tagcase=
setlocal tagfunc=
setlocal tags=
setlocal termwinkey=
setlocal termwinscroll=10000
setlocal termwinsize=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal undolevels=-123456
setlocal varsofttabstop=
setlocal vartabstop=
setlocal wincolor=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
438
normal! zo
474
normal! zo
507
normal! zo
630
normal! zo
669
normal! zo
745
normal! zo
834
normal! zo
2228
normal! zo
2481
normal! zo
let s:l = 629 - ((10 * winheight(0) + 5) / 11)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
629
normal! 0
wincmd w
argglobal
if bufexists("~/chromation/dev-kit-mike/firmware/vis-spi-out/src/SensorVis.c") | buffer ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/SensorVis.c | else | edit ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/SensorVis.c | endif
lnoremap <buffer> ALPHA Α
lnoremap <buffer> BETA Β
lnoremap <buffer> CHI Χ
lnoremap <buffer> Dhh ┅
lnoremap <buffer> Dhv ┇
lnoremap <buffer> Dlh ┄
lnoremap <buffer> Dlv ┆
lnoremap <buffer> DELTA Δ
lnoremap <buffer> ETA Η
lnoremap <buffer> EPSILON Ε
lnoremap <buffer> GAMMA Γ
lnoremap <buffer> PHI Φ
lnoremap <buffer> RHO Ρ
lnoremap <buffer> THETA Θ
lnoremap <buffer> ZETA Ζ
lnoremap <buffer> aumlaut ä
lnoremap <buffer> alpha α
lnoremap <buffer> a4 à
lnoremap <buffer> a3 ǎ
lnoremap <buffer> a2 á
lnoremap <buffer> a1 ā
lnoremap <buffer> bvv │
let s:cpo_save=&cpo
set cpo&vim
lnoremap <buffer> bhh ─
lnoremap <buffer> bul ┘
lnoremap <buffer> bur └
lnoremap <buffer> bdl ┐
lnoremap <buffer> bdr ┌
lnoremap <buffer> beta β
lnoremap <buffer> chi χ
lnoremap <buffer> dul ┛
lnoremap <buffer> dur ┗
lnoremap <buffer> ddl ┓
lnoremap <buffer> ddr ┏
lnoremap <buffer> dhh ┉
lnoremap <buffer> dhv ┋
lnoremap <buffer> dlh ┈
lnoremap <buffer> dlv ┊
lnoremap <buffer> dac ▾
lnoremap <buffer> dae ▸
lnoremap <buffer> delta δ
lnoremap <buffer> deg °
lnoremap <buffer> eumlaut ë
lnoremap <buffer> eta η
lnoremap <buffer> epsilon ε
lnoremap <buffer> e4 è
lnoremap <buffer> e3 ě
lnoremap <buffer> e2 é
lnoremap <buffer> e1 ē
lnoremap <buffer> geq ≥
lnoremap <buffer> gamma γ
lnoremap <buffer> iumlaut ï
lnoremap <buffer> i4 ì
lnoremap <buffer> i3 ǐ
lnoremap <buffer> i2 í
lnoremap <buffer> i1 ī
lnoremap <buffer> lar ←
lnoremap <buffer> leq ≤
lnoremap <buffer> lambda λ
lnoremap <buffer> mu µ
lnoremap <buffer> oumlaut ö
lnoremap <buffer> ohm Ω
lnoremap <buffer> o4 ò
lnoremap <buffer> o3 ǒ
lnoremap <buffer> o2 ó
lnoremap <buffer> o1 ō
lnoremap <buffer> pm ±
lnoremap <buffer> phi ϕ
lnoremap <buffer> rho ρ
lnoremap <buffer> theta θ
lnoremap <buffer> uumlaut ü
lnoremap <buffer> u4 ù
lnoremap <buffer> u3 ǔ
lnoremap <buffer> u2 ú
lnoremap <buffer> u1 ū
lnoremap <buffer> zeta ζ
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=symbols
setlocal noarabic
setlocal autoindent
setlocal backupcopy=
setlocal balloonexpr=
setlocal nobinary
setlocal nobreakindent
setlocal breakindentopt=
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal cindent
setlocal cinkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
set colorcolumn=+1
setlocal colorcolumn=+1
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'c'
setlocal filetype=c
endif
setlocal fixendofline
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
set foldmethod=indent
setlocal foldmethod=indent
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal formatprg=
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=-1
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal lispwords=
set list
setlocal list
setlocal makeencoding=
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=bin,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=ccomplete#Complete
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
set relativenumber
setlocal relativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal scrolloff=-1
setlocal shiftwidth=4
setlocal noshortname
setlocal sidescrolloff=-1
setlocal signcolumn=auto
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal noswapfile
setlocal synmaxcol=160
if &syntax != 'c'
setlocal syntax=c
endif
setlocal tabstop=4
setlocal tagcase=
setlocal tagfunc=
setlocal tags=
setlocal termwinkey=
setlocal termwinscroll=10000
setlocal termwinsize=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal undolevels=-123456
setlocal varsofttabstop=
setlocal vartabstop=
setlocal wincolor=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
55
normal! zo
64
normal! zo
67
normal! zo
128
normal! zo
144
normal! zo
231
normal! zo
238
normal! zo
238
normal! zc
334
normal! zo
334
normal! zc
376
normal! zo
396
normal! zo
412
normal! zo
let s:l = 73 - ((4 * winheight(0) + 6) / 12)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
73
normal! 023|
wincmd w
exe '1resize ' . ((&lines * 10 + 20) / 41)
exe 'vert 1resize ' . ((&columns * 79 + 88) / 177)
exe '2resize ' . ((&lines * 11 + 20) / 41)
exe 'vert 2resize ' . ((&columns * 79 + 88) / 177)
exe '3resize ' . ((&lines * 14 + 20) / 41)
exe 'vert 3resize ' . ((&columns * 79 + 88) / 177)
exe '4resize ' . ((&lines * 12 + 20) / 41)
exe 'vert 4resize ' . ((&columns * 97 + 88) / 177)
exe '5resize ' . ((&lines * 11 + 20) / 41)
exe 'vert 5resize ' . ((&columns * 97 + 88) / 177)
exe '6resize ' . ((&lines * 12 + 20) / 41)
exe 'vert 6resize ' . ((&columns * 97 + 88) / 177)
tabnext
edit ~/chromation/dev-kit-mike/README.md
set splitbelow splitright
wincmd _ | wincmd |
split
wincmd _ | wincmd |
split
wincmd _ | wincmd |
split
3wincmd k
wincmd w
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
wincmd w
wincmd w
set nosplitbelow
wincmd t
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe '1resize ' . ((&lines * 7 + 20) / 41)
exe '2resize ' . ((&lines * 8 + 20) / 41)
exe 'vert 2resize ' . ((&columns * 88 + 88) / 177)
exe '3resize ' . ((&lines * 8 + 20) / 41)
exe 'vert 3resize ' . ((&columns * 88 + 88) / 177)
exe '4resize ' . ((&lines * 8 + 20) / 41)
exe '5resize ' . ((&lines * 11 + 20) / 41)
argglobal
lnoremap <buffer> ALPHA Α
lnoremap <buffer> BETA Β
lnoremap <buffer> CHI Χ
lnoremap <buffer> Dhh ┅
lnoremap <buffer> Dhv ┇
lnoremap <buffer> Dlh ┄
lnoremap <buffer> Dlv ┆
lnoremap <buffer> DELTA Δ
lnoremap <buffer> ETA Η
lnoremap <buffer> EPSILON Ε
lnoremap <buffer> GAMMA Γ
lnoremap <buffer> PHI Φ
lnoremap <buffer> RHO Ρ
lnoremap <buffer> THETA Θ
lnoremap <buffer> ZETA Ζ
lnoremap <buffer> aumlaut ä
lnoremap <buffer> alpha α
lnoremap <buffer> a4 à
lnoremap <buffer> a3 ǎ
lnoremap <buffer> a2 á
lnoremap <buffer> a1 ā
lnoremap <buffer> bvv │
let s:cpo_save=&cpo
set cpo&vim
lnoremap <buffer> bhh ─
lnoremap <buffer> bul ┘
lnoremap <buffer> bur └
lnoremap <buffer> bdl ┐
lnoremap <buffer> bdr ┌
lnoremap <buffer> beta β
lnoremap <buffer> chi χ
lnoremap <buffer> dul ┛
lnoremap <buffer> dur ┗
lnoremap <buffer> ddl ┓
lnoremap <buffer> ddr ┏
lnoremap <buffer> dhh ┉
lnoremap <buffer> dhv ┋
lnoremap <buffer> dlh ┈
lnoremap <buffer> dlv ┊
lnoremap <buffer> dac ▾
lnoremap <buffer> dae ▸
lnoremap <buffer> delta δ
lnoremap <buffer> deg °
lnoremap <buffer> eumlaut ë
lnoremap <buffer> eta η
lnoremap <buffer> epsilon ε
lnoremap <buffer> e4 è
lnoremap <buffer> e3 ě
lnoremap <buffer> e2 é
lnoremap <buffer> e1 ē
lnoremap <buffer> geq ≥
lnoremap <buffer> gamma γ
lnoremap <buffer> iumlaut ï
lnoremap <buffer> i4 ì
lnoremap <buffer> i3 ǐ
lnoremap <buffer> i2 í
lnoremap <buffer> i1 ī
lnoremap <buffer> lar ←
lnoremap <buffer> leq ≤
lnoremap <buffer> lambda λ
lnoremap <buffer> mu µ
lnoremap <buffer> oumlaut ö
lnoremap <buffer> ohm Ω
lnoremap <buffer> o4 ò
lnoremap <buffer> o3 ǒ
lnoremap <buffer> o2 ó
lnoremap <buffer> o1 ō
lnoremap <buffer> pm ±
lnoremap <buffer> phi ϕ
lnoremap <buffer> rho ρ
lnoremap <buffer> theta θ
lnoremap <buffer> uumlaut ü
lnoremap <buffer> u4 ù
lnoremap <buffer> u3 ǔ
lnoremap <buffer> u2 ú
lnoremap <buffer> u1 ū
lnoremap <buffer> zeta ζ
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=symbols
setlocal noarabic
setlocal autoindent
setlocal backupcopy=
setlocal balloonexpr=
setlocal nobinary
setlocal nobreakindent
setlocal breakindentopt=
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
set colorcolumn=+1
setlocal colorcolumn=+1
setlocal comments=fb:*,fb:-,fb:+,n:>
setlocal commentstring=<!--%s-->
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'markdown'
setlocal filetype=markdown
endif
setlocal fixendofline
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=MarkdownFold()
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
set foldmethod=indent
setlocal foldmethod=expr
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=MarkdownFoldText()
setlocal formatexpr=
setlocal formatoptions=tcqln
setlocal formatlistpat=^\\s*\\d\\+\\.\\s\\+\\|^[-*+]\\s\\+\\|^\\[^\\ze[^\\]]\\+\\]:
setlocal formatprg=
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=-1
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal lispwords=
set list
setlocal list
setlocal makeencoding=
setlocal makeprg=
setlocal matchpairs=(:),{:},[:],<:>
setlocal modeline
setlocal modifiable
setlocal nrformats=bin,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=htmlcomplete#CompleteTags
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
set relativenumber
setlocal relativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal scrolloff=-1
setlocal shiftwidth=4
setlocal noshortname
setlocal sidescrolloff=-1
setlocal signcolumn=auto
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=160
if &syntax != 'markdown'
setlocal syntax=markdown
endif
setlocal tabstop=4
setlocal tagcase=
setlocal tagfunc=
setlocal tags=
setlocal termwinkey=
setlocal termwinscroll=10000
setlocal termwinsize=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal undolevels=-123456
setlocal varsofttabstop=
setlocal vartabstop=
setlocal wincolor=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
3895
normal! zo
3956
normal! zo
let s:l = 3957 - ((46 * winheight(0) + 3) / 7)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
3957
normal! 0
wincmd w
argglobal
if bufexists("/cygdrive/c/Users/Mike/Documents/chromation/txt/updates/notes-for-update/notes_for_update.md") | buffer /cygdrive/c/Users/Mike/Documents/chromation/txt/updates/notes-for-update/notes_for_update.md | else | edit /cygdrive/c/Users/Mike/Documents/chromation/txt/updates/notes-for-update/notes_for_update.md | endif
lnoremap <buffer> ALPHA Α
lnoremap <buffer> BETA Β
lnoremap <buffer> CHI Χ
lnoremap <buffer> Dhh ┅
lnoremap <buffer> Dhv ┇
lnoremap <buffer> Dlh ┄
lnoremap <buffer> Dlv ┆
lnoremap <buffer> DELTA Δ
lnoremap <buffer> ETA Η
lnoremap <buffer> EPSILON Ε
lnoremap <buffer> GAMMA Γ
lnoremap <buffer> PHI Φ
lnoremap <buffer> RHO Ρ
lnoremap <buffer> THETA Θ
lnoremap <buffer> ZETA Ζ
lnoremap <buffer> aumlaut ä
lnoremap <buffer> alpha α
lnoremap <buffer> a4 à
lnoremap <buffer> a3 ǎ
lnoremap <buffer> a2 á
lnoremap <buffer> a1 ā
lnoremap <buffer> bvv │
let s:cpo_save=&cpo
set cpo&vim
lnoremap <buffer> bhh ─
lnoremap <buffer> bul ┘
lnoremap <buffer> bur └
lnoremap <buffer> bdl ┐
lnoremap <buffer> bdr ┌
lnoremap <buffer> beta β
lnoremap <buffer> chi χ
lnoremap <buffer> dul ┛
lnoremap <buffer> dur ┗
lnoremap <buffer> ddl ┓
lnoremap <buffer> ddr ┏
lnoremap <buffer> dhh ┉
lnoremap <buffer> dhv ┋
lnoremap <buffer> dlh ┈
lnoremap <buffer> dlv ┊
lnoremap <buffer> dac ▾
lnoremap <buffer> dae ▸
lnoremap <buffer> delta δ
lnoremap <buffer> deg °
lnoremap <buffer> eumlaut ë
lnoremap <buffer> eta η
lnoremap <buffer> epsilon ε
lnoremap <buffer> e4 è
lnoremap <buffer> e3 ě
lnoremap <buffer> e2 é
lnoremap <buffer> e1 ē
lnoremap <buffer> geq ≥
lnoremap <buffer> gamma γ
lnoremap <buffer> iumlaut ï
lnoremap <buffer> i4 ì
lnoremap <buffer> i3 ǐ
lnoremap <buffer> i2 í
lnoremap <buffer> i1 ī
lnoremap <buffer> lar ←
lnoremap <buffer> leq ≤
lnoremap <buffer> lambda λ
lnoremap <buffer> mu µ
lnoremap <buffer> oumlaut ö
lnoremap <buffer> ohm Ω
lnoremap <buffer> o4 ò
lnoremap <buffer> o3 ǒ
lnoremap <buffer> o2 ó
lnoremap <buffer> o1 ō
lnoremap <buffer> pm ±
lnoremap <buffer> phi ϕ
lnoremap <buffer> rho ρ
lnoremap <buffer> theta θ
lnoremap <buffer> uumlaut ü
lnoremap <buffer> u4 ù
lnoremap <buffer> u3 ǔ
lnoremap <buffer> u2 ú
lnoremap <buffer> u1 ū
lnoremap <buffer> zeta ζ
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=symbols
setlocal noarabic
setlocal autoindent
setlocal backupcopy=
setlocal balloonexpr=
setlocal nobinary
setlocal nobreakindent
setlocal breakindentopt=
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
set colorcolumn=+1
setlocal colorcolumn=+1
setlocal comments=fb:*,fb:-,fb:+,n:>
setlocal commentstring=<!--%s-->
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'markdown'
setlocal filetype=markdown
endif
setlocal fixendofline
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=MarkdownFold()
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
set foldmethod=indent
setlocal foldmethod=expr
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=MarkdownFoldText()
setlocal formatexpr=
setlocal formatoptions=tcqln
setlocal formatlistpat=^\\s*\\d\\+\\.\\s\\+\\|^[-*+]\\s\\+\\|^\\[^\\ze[^\\]]\\+\\]:
setlocal formatprg=
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=-1
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal lispwords=
set list
setlocal list
setlocal makeencoding=
setlocal makeprg=
setlocal matchpairs=(:),{:},[:],<:>
setlocal modeline
setlocal modifiable
setlocal nrformats=bin,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=htmlcomplete#CompleteTags
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
set relativenumber
setlocal relativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal scrolloff=-1
setlocal shiftwidth=4
setlocal noshortname
setlocal sidescrolloff=-1
setlocal signcolumn=auto
setlocal nosmartindent
setlocal softtabstop=4
setlocal spell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en_us
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=160
if &syntax != 'markdown'
setlocal syntax=markdown
endif
setlocal tabstop=4
setlocal tagcase=
setlocal tagfunc=
setlocal tags=
setlocal termwinkey=
setlocal termwinscroll=10000
setlocal termwinsize=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal undolevels=-123456
setlocal varsofttabstop=
setlocal vartabstop=
setlocal wincolor=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
37
normal! zo
38
normal! zo
39
normal! zo
41
normal! zo
108
normal! zo
114
normal! zo
5429
normal! zo
6298
normal! zo
6298
normal! zc
6306
normal! zo
6328
normal! zo
6415
normal! zo
6417
normal! zo
6420
normal! zo
6441
normal! zo
6415
normal! zc
6550
normal! zo
6555
normal! zo
6564
normal! zo
6574
normal! zo
7224
normal! zo
7273
normal! zo
let s:l = 7215 - ((4 * winheight(0) + 4) / 8)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
7215
normal! 0
wincmd w
argglobal
if bufexists("~/read-the-docs/man-gcc.man") | buffer ~/read-the-docs/man-gcc.man | else | edit ~/read-the-docs/man-gcc.man | endif
let s:cpo_save=&cpo
set cpo&vim
nmap <buffer> \h <Plug>ManBS
nnoremap <buffer> <silent> q :q
nnoremap <buffer> <Plug>ManBS :%s/.\b//g:setl nomod''
lnoremap <buffer> ALPHA Α
lnoremap <buffer> BETA Β
lnoremap <buffer> CHI Χ
lnoremap <buffer> Dhh ┅
lnoremap <buffer> Dhv ┇
lnoremap <buffer> Dlh ┄
lnoremap <buffer> Dlv ┆
lnoremap <buffer> DELTA Δ
lnoremap <buffer> ETA Η
lnoremap <buffer> EPSILON Ε
lnoremap <buffer> GAMMA Γ
lnoremap <buffer> PHI Φ
lnoremap <buffer> RHO Ρ
lnoremap <buffer> THETA Θ
lnoremap <buffer> ZETA Ζ
lnoremap <buffer> aumlaut ä
lnoremap <buffer> alpha α
lnoremap <buffer> a4 à
lnoremap <buffer> a3 ǎ
lnoremap <buffer> a2 á
lnoremap <buffer> a1 ā
lnoremap <buffer> bvv │
lnoremap <buffer> bhh ─
lnoremap <buffer> bul ┘
lnoremap <buffer> bur └
lnoremap <buffer> bdl ┐
lnoremap <buffer> bdr ┌
lnoremap <buffer> beta β
lnoremap <buffer> chi χ
lnoremap <buffer> dul ┛
lnoremap <buffer> dur ┗
lnoremap <buffer> ddl ┓
lnoremap <buffer> ddr ┏
lnoremap <buffer> dhh ┉
lnoremap <buffer> dhv ┋
lnoremap <buffer> dlh ┈
lnoremap <buffer> dlv ┊
lnoremap <buffer> dac ▾
lnoremap <buffer> dae ▸
lnoremap <buffer> delta δ
lnoremap <buffer> deg °
lnoremap <buffer> eumlaut ë
lnoremap <buffer> eta η
lnoremap <buffer> epsilon ε
lnoremap <buffer> e4 è
lnoremap <buffer> e3 ě
lnoremap <buffer> e2 é
lnoremap <buffer> e1 ē
lnoremap <buffer> geq ≥
lnoremap <buffer> gamma γ
lnoremap <buffer> iumlaut ï
lnoremap <buffer> i4 ì
lnoremap <buffer> i3 ǐ
lnoremap <buffer> i2 í
lnoremap <buffer> i1 ī
lnoremap <buffer> lar ←
lnoremap <buffer> leq ≤
lnoremap <buffer> lambda λ
lnoremap <buffer> mu µ
lnoremap <buffer> oumlaut ö
lnoremap <buffer> ohm Ω
lnoremap <buffer> o4 ò
lnoremap <buffer> o3 ǒ
lnoremap <buffer> o2 ó
lnoremap <buffer> o1 ō
lnoremap <buffer> pm ±
lnoremap <buffer> phi ϕ
lnoremap <buffer> rho ρ
lnoremap <buffer> theta θ
lnoremap <buffer> uumlaut ü
lnoremap <buffer> u4 ù
lnoremap <buffer> u3 ǔ
lnoremap <buffer> u2 ú
lnoremap <buffer> u1 ū
lnoremap <buffer> zeta ζ
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=symbols
setlocal noarabic
setlocal autoindent
setlocal backupcopy=
setlocal balloonexpr=
setlocal nobinary
setlocal nobreakindent
setlocal breakindentopt=
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
set colorcolumn=+1
setlocal colorcolumn=+1
setlocal comments=s1:/*,mb:*,ex:*/,://,b:#,:%,:XCOMM,n:>,fb:-
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'man'
setlocal filetype=man
endif
setlocal fixendofline
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
set foldmethod=indent
setlocal foldmethod=indent
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=tcq
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal formatprg=
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=-1
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255,.,-
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal lispwords=
set list
setlocal list
setlocal makeencoding=
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=bin,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
set relativenumber
setlocal relativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal scrolloff=-1
setlocal shiftwidth=4
setlocal noshortname
setlocal sidescrolloff=-1
setlocal signcolumn=auto
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=160
if &syntax != 'man'
setlocal syntax=man
endif
setlocal tabstop=4
setlocal tagcase=
setlocal tagfunc=
setlocal tags=
setlocal termwinkey=
setlocal termwinscroll=10000
setlocal termwinsize=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal undolevels=-123456
setlocal varsofttabstop=
setlocal vartabstop=
setlocal wincolor=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
7
normal! zo
8
normal! zo
18
normal! zo
44
normal! zo
164
normal! zo
451
normal! zo
522
normal! zo
534
normal! zo
609
normal! zo
764
normal! zo
868
normal! zo
1040
normal! zo
1557
normal! zo
1562
normal! zo
1567
normal! zo
1572
normal! zo
1842
normal! zo
1891
normal! zo
2263
normal! zo
2347
normal! zo
2472
normal! zo
2482
normal! zo
2559
normal! zo
2845
normal! zo
2875
normal! zo
2891
normal! zo
3080
normal! zo
3478
normal! zo
3574
normal! zo
3585
normal! zo
3744
normal! zo
3789
normal! zo
5128
normal! zo
5218
normal! zo
6067
normal! zo
6522
normal! zo
6531
normal! zo
6534
normal! zo
6547
normal! zo
6550
normal! zo
6557
normal! zo
6563
normal! zo
6566
normal! zo
6570
normal! zo
6570
normal! zo
6593
normal! zo
6593
normal! zo
6602
normal! zo
6610
normal! zo
6610
normal! zo
6620
normal! zo
6624
normal! zo
6624
normal! zo
6629
normal! zo
6629
normal! zc
6640
normal! zo
6640
normal! zc
6647
normal! zo
6656
normal! zo
6659
normal! zo
6688
normal! zo
6751
normal! zo
6787
normal! zo
7037
normal! zo
7043
normal! zo
7160
normal! zo
7516
normal! zo
8292
normal! zo
14873
normal! zo
14876
normal! zo
15252
normal! zo
15643
normal! zo
15741
normal! zo
15751
normal! zo
let s:l = 6532 - ((2 * winheight(0) + 4) / 8)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
6532
normal! 0
wincmd w
argglobal
if bufexists("/cygdrive/c/Users/Mike/Documents/chromation/txt/updates/notes-for-update/notes_for_update.md") | buffer /cygdrive/c/Users/Mike/Documents/chromation/txt/updates/notes-for-update/notes_for_update.md | else | edit /cygdrive/c/Users/Mike/Documents/chromation/txt/updates/notes-for-update/notes_for_update.md | endif
lnoremap <buffer> ALPHA Α
lnoremap <buffer> BETA Β
lnoremap <buffer> CHI Χ
lnoremap <buffer> Dhh ┅
lnoremap <buffer> Dhv ┇
lnoremap <buffer> Dlh ┄
lnoremap <buffer> Dlv ┆
lnoremap <buffer> DELTA Δ
lnoremap <buffer> ETA Η
lnoremap <buffer> EPSILON Ε
lnoremap <buffer> GAMMA Γ
lnoremap <buffer> PHI Φ
lnoremap <buffer> RHO Ρ
lnoremap <buffer> THETA Θ
lnoremap <buffer> ZETA Ζ
lnoremap <buffer> aumlaut ä
lnoremap <buffer> alpha α
lnoremap <buffer> a4 à
lnoremap <buffer> a3 ǎ
lnoremap <buffer> a2 á
lnoremap <buffer> a1 ā
lnoremap <buffer> bvv │
let s:cpo_save=&cpo
set cpo&vim
lnoremap <buffer> bhh ─
lnoremap <buffer> bul ┘
lnoremap <buffer> bur └
lnoremap <buffer> bdl ┐
lnoremap <buffer> bdr ┌
lnoremap <buffer> beta β
lnoremap <buffer> chi χ
lnoremap <buffer> dul ┛
lnoremap <buffer> dur ┗
lnoremap <buffer> ddl ┓
lnoremap <buffer> ddr ┏
lnoremap <buffer> dhh ┉
lnoremap <buffer> dhv ┋
lnoremap <buffer> dlh ┈
lnoremap <buffer> dlv ┊
lnoremap <buffer> dac ▾
lnoremap <buffer> dae ▸
lnoremap <buffer> delta δ
lnoremap <buffer> deg °
lnoremap <buffer> eumlaut ë
lnoremap <buffer> eta η
lnoremap <buffer> epsilon ε
lnoremap <buffer> e4 è
lnoremap <buffer> e3 ě
lnoremap <buffer> e2 é
lnoremap <buffer> e1 ē
lnoremap <buffer> geq ≥
lnoremap <buffer> gamma γ
lnoremap <buffer> iumlaut ï
lnoremap <buffer> i4 ì
lnoremap <buffer> i3 ǐ
lnoremap <buffer> i2 í
lnoremap <buffer> i1 ī
lnoremap <buffer> lar ←
lnoremap <buffer> leq ≤
lnoremap <buffer> lambda λ
lnoremap <buffer> mu µ
lnoremap <buffer> oumlaut ö
lnoremap <buffer> ohm Ω
lnoremap <buffer> o4 ò
lnoremap <buffer> o3 ǒ
lnoremap <buffer> o2 ó
lnoremap <buffer> o1 ō
lnoremap <buffer> pm ±
lnoremap <buffer> phi ϕ
lnoremap <buffer> rho ρ
lnoremap <buffer> theta θ
lnoremap <buffer> uumlaut ü
lnoremap <buffer> u4 ù
lnoremap <buffer> u3 ǔ
lnoremap <buffer> u2 ú
lnoremap <buffer> u1 ū
lnoremap <buffer> zeta ζ
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=symbols
setlocal noarabic
setlocal autoindent
setlocal backupcopy=
setlocal balloonexpr=
setlocal nobinary
setlocal nobreakindent
setlocal breakindentopt=
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
set colorcolumn=+1
setlocal colorcolumn=+1
setlocal comments=fb:*,fb:-,fb:+,n:>
setlocal commentstring=<!--%s-->
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'markdown'
setlocal filetype=markdown
endif
setlocal fixendofline
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=MarkdownFold()
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
set foldmethod=indent
setlocal foldmethod=expr
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=MarkdownFoldText()
setlocal formatexpr=
setlocal formatoptions=tcqln
setlocal formatlistpat=^\\s*\\d\\+\\.\\s\\+\\|^[-*+]\\s\\+\\|^\\[^\\ze[^\\]]\\+\\]:
setlocal formatprg=
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=-1
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal lispwords=
set list
setlocal list
setlocal makeencoding=
setlocal makeprg=
setlocal matchpairs=(:),{:},[:],<:>
setlocal modeline
setlocal modifiable
setlocal nrformats=bin,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=htmlcomplete#CompleteTags
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
set relativenumber
setlocal relativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal scrolloff=-1
setlocal shiftwidth=4
setlocal noshortname
setlocal sidescrolloff=-1
setlocal signcolumn=auto
setlocal nosmartindent
setlocal softtabstop=4
setlocal spell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en_us
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=160
if &syntax != 'markdown'
setlocal syntax=markdown
endif
setlocal tabstop=4
setlocal tagcase=
setlocal tagfunc=
setlocal tags=
setlocal termwinkey=
setlocal termwinscroll=10000
setlocal termwinsize=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal undolevels=-123456
setlocal varsofttabstop=
setlocal vartabstop=
setlocal wincolor=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
37
normal! zo
38
normal! zo
39
normal! zo
41
normal! zo
108
normal! zo
114
normal! zo
5429
normal! zo
6298
normal! zo
6298
normal! zc
6306
normal! zo
6328
normal! zo
6415
normal! zo
6417
normal! zo
6420
normal! zo
6441
normal! zo
6415
normal! zc
6550
normal! zo
6555
normal! zo
6564
normal! zo
6574
normal! zo
7224
normal! zo
7273
normal! zo
let s:l = 6972 - ((5 * winheight(0) + 4) / 8)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
6972
normal! 038|
wincmd w
argglobal
if bufexists("/cygdrive/c/Users/Mike/Documents/chromation/txt/updates/notes-for-update/notes_for_update.md") | buffer /cygdrive/c/Users/Mike/Documents/chromation/txt/updates/notes-for-update/notes_for_update.md | else | edit /cygdrive/c/Users/Mike/Documents/chromation/txt/updates/notes-for-update/notes_for_update.md | endif
lnoremap <buffer> ALPHA Α
lnoremap <buffer> BETA Β
lnoremap <buffer> CHI Χ
lnoremap <buffer> Dhh ┅
lnoremap <buffer> Dhv ┇
lnoremap <buffer> Dlh ┄
lnoremap <buffer> Dlv ┆
lnoremap <buffer> DELTA Δ
lnoremap <buffer> ETA Η
lnoremap <buffer> EPSILON Ε
lnoremap <buffer> GAMMA Γ
lnoremap <buffer> PHI Φ
lnoremap <buffer> RHO Ρ
lnoremap <buffer> THETA Θ
lnoremap <buffer> ZETA Ζ
lnoremap <buffer> aumlaut ä
lnoremap <buffer> alpha α
lnoremap <buffer> a4 à
lnoremap <buffer> a3 ǎ
lnoremap <buffer> a2 á
lnoremap <buffer> a1 ā
lnoremap <buffer> bvv │
let s:cpo_save=&cpo
set cpo&vim
lnoremap <buffer> bhh ─
lnoremap <buffer> bul ┘
lnoremap <buffer> bur └
lnoremap <buffer> bdl ┐
lnoremap <buffer> bdr ┌
lnoremap <buffer> beta β
lnoremap <buffer> chi χ
lnoremap <buffer> dul ┛
lnoremap <buffer> dur ┗
lnoremap <buffer> ddl ┓
lnoremap <buffer> ddr ┏
lnoremap <buffer> dhh ┉
lnoremap <buffer> dhv ┋
lnoremap <buffer> dlh ┈
lnoremap <buffer> dlv ┊
lnoremap <buffer> dac ▾
lnoremap <buffer> dae ▸
lnoremap <buffer> delta δ
lnoremap <buffer> deg °
lnoremap <buffer> eumlaut ë
lnoremap <buffer> eta η
lnoremap <buffer> epsilon ε
lnoremap <buffer> e4 è
lnoremap <buffer> e3 ě
lnoremap <buffer> e2 é
lnoremap <buffer> e1 ē
lnoremap <buffer> geq ≥
lnoremap <buffer> gamma γ
lnoremap <buffer> iumlaut ï
lnoremap <buffer> i4 ì
lnoremap <buffer> i3 ǐ
lnoremap <buffer> i2 í
lnoremap <buffer> i1 ī
lnoremap <buffer> lar ←
lnoremap <buffer> leq ≤
lnoremap <buffer> lambda λ
lnoremap <buffer> mu µ
lnoremap <buffer> oumlaut ö
lnoremap <buffer> ohm Ω
lnoremap <buffer> o4 ò
lnoremap <buffer> o3 ǒ
lnoremap <buffer> o2 ó
lnoremap <buffer> o1 ō
lnoremap <buffer> pm ±
lnoremap <buffer> phi ϕ
lnoremap <buffer> rho ρ
lnoremap <buffer> theta θ
lnoremap <buffer> uumlaut ü
lnoremap <buffer> u4 ù
lnoremap <buffer> u3 ǔ
lnoremap <buffer> u2 ú
lnoremap <buffer> u1 ū
lnoremap <buffer> zeta ζ
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=symbols
setlocal noarabic
setlocal autoindent
setlocal backupcopy=
setlocal balloonexpr=
setlocal nobinary
setlocal nobreakindent
setlocal breakindentopt=
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
set colorcolumn=+1
setlocal colorcolumn=+1
setlocal comments=fb:*,fb:-,fb:+,n:>
setlocal commentstring=<!--%s-->
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'markdown'
setlocal filetype=markdown
endif
setlocal fixendofline
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=MarkdownFold()
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
set foldmethod=indent
setlocal foldmethod=expr
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=MarkdownFoldText()
setlocal formatexpr=
setlocal formatoptions=tcqln
setlocal formatlistpat=^\\s*\\d\\+\\.\\s\\+\\|^[-*+]\\s\\+\\|^\\[^\\ze[^\\]]\\+\\]:
setlocal formatprg=
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=-1
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal lispwords=
set list
setlocal list
setlocal makeencoding=
setlocal makeprg=
setlocal matchpairs=(:),{:},[:],<:>
setlocal modeline
setlocal modifiable
setlocal nrformats=bin,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=htmlcomplete#CompleteTags
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
set relativenumber
setlocal relativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal scrolloff=-1
setlocal shiftwidth=4
setlocal noshortname
setlocal sidescrolloff=-1
setlocal signcolumn=auto
setlocal nosmartindent
setlocal softtabstop=4
setlocal spell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en_us
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=160
if &syntax != 'markdown'
setlocal syntax=markdown
endif
setlocal tabstop=4
setlocal tagcase=
setlocal tagfunc=
setlocal tags=
setlocal termwinkey=
setlocal termwinscroll=10000
setlocal termwinsize=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal undolevels=-123456
setlocal varsofttabstop=
setlocal vartabstop=
setlocal wincolor=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
37
normal! zo
38
normal! zo
39
normal! zo
41
normal! zo
108
normal! zo
114
normal! zo
5429
normal! zo
6298
normal! zo
6298
normal! zc
6306
normal! zo
6328
normal! zo
6415
normal! zo
6417
normal! zo
6420
normal! zo
6441
normal! zo
6415
normal! zc
6550
normal! zo
6555
normal! zo
6564
normal! zo
6574
normal! zo
7224
normal! zo
7273
normal! zo
let s:l = 6959 - ((8 * winheight(0) + 5) / 11)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
6959
normal! 0
wincmd w
exe '1resize ' . ((&lines * 7 + 20) / 41)
exe '2resize ' . ((&lines * 8 + 20) / 41)
exe 'vert 2resize ' . ((&columns * 88 + 88) / 177)
exe '3resize ' . ((&lines * 8 + 20) / 41)
exe 'vert 3resize ' . ((&columns * 88 + 88) / 177)
exe '4resize ' . ((&lines * 8 + 20) / 41)
exe '5resize ' . ((&lines * 11 + 20) / 41)
tabnext
edit Makefile
set splitbelow splitright
wincmd _ | wincmd |
split
1wincmd k
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd _ | wincmd |
split
wincmd _ | wincmd |
split
wincmd _ | wincmd |
split
wincmd _ | wincmd |
split
4wincmd k
wincmd w
wincmd w
wincmd w
wincmd w
wincmd w
wincmd w
set nosplitbelow
wincmd t
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe '1resize ' . ((&lines * 7 + 20) / 41)
exe 'vert 1resize ' . ((&columns * 88 + 88) / 177)
exe '2resize ' . ((&lines * 4 + 20) / 41)
exe 'vert 2resize ' . ((&columns * 88 + 88) / 177)
exe '3resize ' . ((&lines * 1 + 20) / 41)
exe 'vert 3resize ' . ((&columns * 88 + 88) / 177)
exe '4resize ' . ((&lines * 1 + 20) / 41)
exe 'vert 4resize ' . ((&columns * 88 + 88) / 177)
exe '5resize ' . ((&lines * 15 + 20) / 41)
exe 'vert 5resize ' . ((&columns * 88 + 88) / 177)
exe '6resize ' . ((&lines * 32 + 20) / 41)
exe 'vert 6resize ' . ((&columns * 88 + 88) / 177)
exe '7resize ' . ((&lines * 4 + 20) / 41)
argglobal
lnoremap <buffer> ALPHA Α
lnoremap <buffer> BETA Β
lnoremap <buffer> CHI Χ
lnoremap <buffer> Dhh ┅
lnoremap <buffer> Dhv ┇
lnoremap <buffer> Dlh ┄
lnoremap <buffer> Dlv ┆
lnoremap <buffer> DELTA Δ
lnoremap <buffer> ETA Η
lnoremap <buffer> EPSILON Ε
lnoremap <buffer> GAMMA Γ
lnoremap <buffer> PHI Φ
lnoremap <buffer> RHO Ρ
lnoremap <buffer> THETA Θ
lnoremap <buffer> ZETA Ζ
lnoremap <buffer> aumlaut ä
lnoremap <buffer> alpha α
lnoremap <buffer> a4 à
lnoremap <buffer> a3 ǎ
lnoremap <buffer> a2 á
lnoremap <buffer> a1 ā
lnoremap <buffer> bvv │
let s:cpo_save=&cpo
set cpo&vim
lnoremap <buffer> bhh ─
lnoremap <buffer> bul ┘
lnoremap <buffer> bur └
lnoremap <buffer> bdl ┐
lnoremap <buffer> bdr ┌
lnoremap <buffer> beta β
lnoremap <buffer> chi χ
lnoremap <buffer> dul ┛
lnoremap <buffer> dur ┗
lnoremap <buffer> ddl ┓
lnoremap <buffer> ddr ┏
lnoremap <buffer> dhh ┉
lnoremap <buffer> dhv ┋
lnoremap <buffer> dlh ┈
lnoremap <buffer> dlv ┊
lnoremap <buffer> dac ▾
lnoremap <buffer> dae ▸
lnoremap <buffer> delta δ
lnoremap <buffer> deg °
lnoremap <buffer> eumlaut ë
lnoremap <buffer> eta η
lnoremap <buffer> epsilon ε
lnoremap <buffer> e4 è
lnoremap <buffer> e3 ě
lnoremap <buffer> e2 é
lnoremap <buffer> e1 ē
lnoremap <buffer> geq ≥
lnoremap <buffer> gamma γ
lnoremap <buffer> iumlaut ï
lnoremap <buffer> i4 ì
lnoremap <buffer> i3 ǐ
lnoremap <buffer> i2 í
lnoremap <buffer> i1 ī
lnoremap <buffer> lar ←
lnoremap <buffer> leq ≤
lnoremap <buffer> lambda λ
lnoremap <buffer> mu µ
lnoremap <buffer> oumlaut ö
lnoremap <buffer> ohm Ω
lnoremap <buffer> o4 ò
lnoremap <buffer> o3 ǒ
lnoremap <buffer> o2 ó
lnoremap <buffer> o1 ō
lnoremap <buffer> pm ±
lnoremap <buffer> phi ϕ
lnoremap <buffer> rho ρ
lnoremap <buffer> theta θ
lnoremap <buffer> uumlaut ü
lnoremap <buffer> u4 ù
lnoremap <buffer> u3 ǔ
lnoremap <buffer> u2 ú
lnoremap <buffer> u1 ū
lnoremap <buffer> zeta ζ
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=symbols
setlocal noarabic
setlocal autoindent
setlocal backupcopy=
setlocal balloonexpr=
setlocal nobinary
setlocal nobreakindent
setlocal breakindentopt=
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
set colorcolumn=+1
setlocal colorcolumn=+1
setlocal comments=sO:#\ -,mO:#\ \ ,b:#
setlocal commentstring=#\ %s
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal noexpandtab
if &filetype != 'make'
setlocal filetype=make
endif
setlocal fixendofline
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
set foldmethod=indent
setlocal foldmethod=indent
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal formatprg=
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=-1
setlocal include=^\\s*include
setlocal includeexpr=
setlocal indentexpr=GetMakeIndent()
setlocal indentkeys=!^F,o,O,<:>,=else,=endif
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal lispwords=
set list
setlocal list
setlocal makeencoding=
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=bin,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
set relativenumber
setlocal relativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal scrolloff=-1
setlocal shiftwidth=4
setlocal noshortname
setlocal sidescrolloff=-1
setlocal signcolumn=auto
setlocal nosmartindent
setlocal softtabstop=0
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=160
if &syntax != 'make'
setlocal syntax=make
endif
setlocal tabstop=4
setlocal tagcase=
setlocal tagfunc=
setlocal tags=
setlocal termwinkey=
setlocal termwinscroll=10000
setlocal termwinsize=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal undolevels=-123456
setlocal varsofttabstop=
setlocal vartabstop=
setlocal wincolor=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
5
normal! zo
9
normal! zo
12
normal! zo
20
normal! zo
let s:l = 11 - ((5 * winheight(0) + 3) / 7)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
11
normal! 073|
wincmd w
argglobal
if bufexists("src/lib_to_fake_faked.h") | buffer src/lib_to_fake_faked.h | else | edit src/lib_to_fake_faked.h | endif
lnoremap <buffer> ALPHA Α
lnoremap <buffer> BETA Β
lnoremap <buffer> CHI Χ
lnoremap <buffer> Dhh ┅
lnoremap <buffer> Dhv ┇
lnoremap <buffer> Dlh ┄
lnoremap <buffer> Dlv ┆
lnoremap <buffer> DELTA Δ
lnoremap <buffer> ETA Η
lnoremap <buffer> EPSILON Ε
lnoremap <buffer> GAMMA Γ
lnoremap <buffer> PHI Φ
lnoremap <buffer> RHO Ρ
lnoremap <buffer> THETA Θ
lnoremap <buffer> ZETA Ζ
lnoremap <buffer> aumlaut ä
lnoremap <buffer> alpha α
lnoremap <buffer> a4 à
lnoremap <buffer> a3 ǎ
lnoremap <buffer> a2 á
lnoremap <buffer> a1 ā
lnoremap <buffer> bvv │
let s:cpo_save=&cpo
set cpo&vim
lnoremap <buffer> bhh ─
lnoremap <buffer> bul ┘
lnoremap <buffer> bur └
lnoremap <buffer> bdl ┐
lnoremap <buffer> bdr ┌
lnoremap <buffer> beta β
lnoremap <buffer> chi χ
lnoremap <buffer> dul ┛
lnoremap <buffer> dur ┗
lnoremap <buffer> ddl ┓
lnoremap <buffer> ddr ┏
lnoremap <buffer> dhh ┉
lnoremap <buffer> dhv ┋
lnoremap <buffer> dlh ┈
lnoremap <buffer> dlv ┊
lnoremap <buffer> dac ▾
lnoremap <buffer> dae ▸
lnoremap <buffer> delta δ
lnoremap <buffer> deg °
lnoremap <buffer> eumlaut ë
lnoremap <buffer> eta η
lnoremap <buffer> epsilon ε
lnoremap <buffer> e4 è
lnoremap <buffer> e3 ě
lnoremap <buffer> e2 é
lnoremap <buffer> e1 ē
lnoremap <buffer> geq ≥
lnoremap <buffer> gamma γ
lnoremap <buffer> iumlaut ï
lnoremap <buffer> i4 ì
lnoremap <buffer> i3 ǐ
lnoremap <buffer> i2 í
lnoremap <buffer> i1 ī
lnoremap <buffer> lar ←
lnoremap <buffer> leq ≤
lnoremap <buffer> lambda λ
lnoremap <buffer> mu µ
lnoremap <buffer> oumlaut ö
lnoremap <buffer> ohm Ω
lnoremap <buffer> o4 ò
lnoremap <buffer> o3 ǒ
lnoremap <buffer> o2 ó
lnoremap <buffer> o1 ō
lnoremap <buffer> pm ±
lnoremap <buffer> phi ϕ
lnoremap <buffer> rho ρ
lnoremap <buffer> theta θ
lnoremap <buffer> uumlaut ü
lnoremap <buffer> u4 ù
lnoremap <buffer> u3 ǔ
lnoremap <buffer> u2 ú
lnoremap <buffer> u1 ū
lnoremap <buffer> zeta ζ
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=symbols
setlocal noarabic
setlocal autoindent
setlocal backupcopy=
setlocal balloonexpr=
setlocal nobinary
setlocal nobreakindent
setlocal breakindentopt=
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal cindent
setlocal cinkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
set colorcolumn=+1
setlocal colorcolumn=+1
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'cpp'
setlocal filetype=cpp
endif
setlocal fixendofline
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
set foldmethod=indent
setlocal foldmethod=indent
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal formatprg=
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=-1
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal lispwords=
set list
setlocal list
setlocal makeencoding=
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=bin,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=ccomplete#Complete
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
set relativenumber
setlocal relativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal scrolloff=-1
setlocal shiftwidth=4
setlocal noshortname
setlocal sidescrolloff=-1
setlocal signcolumn=auto
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=160
if &syntax != 'cpp'
setlocal syntax=cpp
endif
setlocal tabstop=4
setlocal tagcase=
setlocal tagfunc=
setlocal tags=
setlocal termwinkey=
setlocal termwinscroll=10000
setlocal termwinsize=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal undolevels=-123456
setlocal varsofttabstop=
setlocal vartabstop=
setlocal wincolor=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
let s:l = 7 - ((2 * winheight(0) + 2) / 4)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
7
normal! 0
wincmd w
argglobal
if bufexists("src/lib_to_fake_faked.c") | buffer src/lib_to_fake_faked.c | else | edit src/lib_to_fake_faked.c | endif
lnoremap <buffer> ALPHA Α
lnoremap <buffer> BETA Β
lnoremap <buffer> CHI Χ
lnoremap <buffer> Dhh ┅
lnoremap <buffer> Dhv ┇
lnoremap <buffer> Dlh ┄
lnoremap <buffer> Dlv ┆
lnoremap <buffer> DELTA Δ
lnoremap <buffer> ETA Η
lnoremap <buffer> EPSILON Ε
lnoremap <buffer> GAMMA Γ
lnoremap <buffer> PHI Φ
lnoremap <buffer> RHO Ρ
lnoremap <buffer> THETA Θ
lnoremap <buffer> ZETA Ζ
lnoremap <buffer> aumlaut ä
lnoremap <buffer> alpha α
lnoremap <buffer> a4 à
lnoremap <buffer> a3 ǎ
lnoremap <buffer> a2 á
lnoremap <buffer> a1 ā
lnoremap <buffer> bvv │
let s:cpo_save=&cpo
set cpo&vim
lnoremap <buffer> bhh ─
lnoremap <buffer> bul ┘
lnoremap <buffer> bur └
lnoremap <buffer> bdl ┐
lnoremap <buffer> bdr ┌
lnoremap <buffer> beta β
lnoremap <buffer> chi χ
lnoremap <buffer> dul ┛
lnoremap <buffer> dur ┗
lnoremap <buffer> ddl ┓
lnoremap <buffer> ddr ┏
lnoremap <buffer> dhh ┉
lnoremap <buffer> dhv ┋
lnoremap <buffer> dlh ┈
lnoremap <buffer> dlv ┊
lnoremap <buffer> dac ▾
lnoremap <buffer> dae ▸
lnoremap <buffer> delta δ
lnoremap <buffer> deg °
lnoremap <buffer> eumlaut ë
lnoremap <buffer> eta η
lnoremap <buffer> epsilon ε
lnoremap <buffer> e4 è
lnoremap <buffer> e3 ě
lnoremap <buffer> e2 é
lnoremap <buffer> e1 ē
lnoremap <buffer> geq ≥
lnoremap <buffer> gamma γ
lnoremap <buffer> iumlaut ï
lnoremap <buffer> i4 ì
lnoremap <buffer> i3 ǐ
lnoremap <buffer> i2 í
lnoremap <buffer> i1 ī
lnoremap <buffer> lar ←
lnoremap <buffer> leq ≤
lnoremap <buffer> lambda λ
lnoremap <buffer> mu µ
lnoremap <buffer> oumlaut ö
lnoremap <buffer> ohm Ω
lnoremap <buffer> o4 ò
lnoremap <buffer> o3 ǒ
lnoremap <buffer> o2 ó
lnoremap <buffer> o1 ō
lnoremap <buffer> pm ±
lnoremap <buffer> phi ϕ
lnoremap <buffer> rho ρ
lnoremap <buffer> theta θ
lnoremap <buffer> uumlaut ü
lnoremap <buffer> u4 ù
lnoremap <buffer> u3 ǔ
lnoremap <buffer> u2 ú
lnoremap <buffer> u1 ū
lnoremap <buffer> zeta ζ
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=symbols
setlocal noarabic
setlocal autoindent
setlocal backupcopy=
setlocal balloonexpr=
setlocal nobinary
setlocal nobreakindent
setlocal breakindentopt=
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal cindent
setlocal cinkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
set colorcolumn=+1
setlocal colorcolumn=+1
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'c'
setlocal filetype=c
endif
setlocal fixendofline
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
set foldmethod=indent
setlocal foldmethod=indent
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal formatprg=
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=-1
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal lispwords=
set list
setlocal list
setlocal makeencoding=
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=bin,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=ccomplete#Complete
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
set relativenumber
setlocal relativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal scrolloff=-1
setlocal shiftwidth=4
setlocal noshortname
setlocal sidescrolloff=-1
setlocal signcolumn=auto
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=160
if &syntax != 'c'
setlocal syntax=c
endif
setlocal tabstop=4
setlocal tagcase=
setlocal tagfunc=
setlocal tags=
setlocal termwinkey=
setlocal termwinscroll=10000
setlocal termwinsize=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal undolevels=-123456
setlocal varsofttabstop=
setlocal vartabstop=
setlocal wincolor=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
let s:l = 1 - ((0 * winheight(0) + 0) / 1)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
1
normal! 0
wincmd w
argglobal
if bufexists("src/lib_to_fake.h") | buffer src/lib_to_fake.h | else | edit src/lib_to_fake.h | endif
lnoremap <buffer> ALPHA Α
lnoremap <buffer> BETA Β
lnoremap <buffer> CHI Χ
lnoremap <buffer> Dhh ┅
lnoremap <buffer> Dhv ┇
lnoremap <buffer> Dlh ┄
lnoremap <buffer> Dlv ┆
lnoremap <buffer> DELTA Δ
lnoremap <buffer> ETA Η
lnoremap <buffer> EPSILON Ε
lnoremap <buffer> GAMMA Γ
lnoremap <buffer> PHI Φ
lnoremap <buffer> RHO Ρ
lnoremap <buffer> THETA Θ
lnoremap <buffer> ZETA Ζ
lnoremap <buffer> aumlaut ä
lnoremap <buffer> alpha α
lnoremap <buffer> a4 à
lnoremap <buffer> a3 ǎ
lnoremap <buffer> a2 á
lnoremap <buffer> a1 ā
lnoremap <buffer> bvv │
let s:cpo_save=&cpo
set cpo&vim
lnoremap <buffer> bhh ─
lnoremap <buffer> bul ┘
lnoremap <buffer> bur └
lnoremap <buffer> bdl ┐
lnoremap <buffer> bdr ┌
lnoremap <buffer> beta β
lnoremap <buffer> chi χ
lnoremap <buffer> dul ┛
lnoremap <buffer> dur ┗
lnoremap <buffer> ddl ┓
lnoremap <buffer> ddr ┏
lnoremap <buffer> dhh ┉
lnoremap <buffer> dhv ┋
lnoremap <buffer> dlh ┈
lnoremap <buffer> dlv ┊
lnoremap <buffer> dac ▾
lnoremap <buffer> dae ▸
lnoremap <buffer> delta δ
lnoremap <buffer> deg °
lnoremap <buffer> eumlaut ë
lnoremap <buffer> eta η
lnoremap <buffer> epsilon ε
lnoremap <buffer> e4 è
lnoremap <buffer> e3 ě
lnoremap <buffer> e2 é
lnoremap <buffer> e1 ē
lnoremap <buffer> geq ≥
lnoremap <buffer> gamma γ
lnoremap <buffer> iumlaut ï
lnoremap <buffer> i4 ì
lnoremap <buffer> i3 ǐ
lnoremap <buffer> i2 í
lnoremap <buffer> i1 ī
lnoremap <buffer> lar ←
lnoremap <buffer> leq ≤
lnoremap <buffer> lambda λ
lnoremap <buffer> mu µ
lnoremap <buffer> oumlaut ö
lnoremap <buffer> ohm Ω
lnoremap <buffer> o4 ò
lnoremap <buffer> o3 ǒ
lnoremap <buffer> o2 ó
lnoremap <buffer> o1 ō
lnoremap <buffer> pm ±
lnoremap <buffer> phi ϕ
lnoremap <buffer> rho ρ
lnoremap <buffer> theta θ
lnoremap <buffer> uumlaut ü
lnoremap <buffer> u4 ù
lnoremap <buffer> u3 ǔ
lnoremap <buffer> u2 ú
lnoremap <buffer> u1 ū
lnoremap <buffer> zeta ζ
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=symbols
setlocal noarabic
setlocal autoindent
setlocal backupcopy=
setlocal balloonexpr=
setlocal nobinary
setlocal nobreakindent
setlocal breakindentopt=
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal cindent
setlocal cinkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
set colorcolumn=+1
setlocal colorcolumn=+1
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'cpp'
setlocal filetype=cpp
endif
setlocal fixendofline
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
set foldmethod=indent
setlocal foldmethod=indent
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal formatprg=
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=-1
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal lispwords=
set list
setlocal list
setlocal makeencoding=
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=bin,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=ccomplete#Complete
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
set relativenumber
setlocal relativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal scrolloff=-1
setlocal shiftwidth=4
setlocal noshortname
setlocal sidescrolloff=-1
setlocal signcolumn=auto
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=160
if &syntax != 'cpp'
setlocal syntax=cpp
endif
setlocal tabstop=4
setlocal tagcase=
setlocal tagfunc=
setlocal tags=
setlocal termwinkey=
setlocal termwinscroll=10000
setlocal termwinsize=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal undolevels=-123456
setlocal varsofttabstop=
setlocal vartabstop=
setlocal wincolor=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
let s:l = 6 - ((0 * winheight(0) + 0) / 1)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
6
normal! 0
wincmd w
argglobal
if bufexists("src/lib_to_fake.c") | buffer src/lib_to_fake.c | else | edit src/lib_to_fake.c | endif
lnoremap <buffer> ALPHA Α
lnoremap <buffer> BETA Β
lnoremap <buffer> CHI Χ
lnoremap <buffer> Dhh ┅
lnoremap <buffer> Dhv ┇
lnoremap <buffer> Dlh ┄
lnoremap <buffer> Dlv ┆
lnoremap <buffer> DELTA Δ
lnoremap <buffer> ETA Η
lnoremap <buffer> EPSILON Ε
lnoremap <buffer> GAMMA Γ
lnoremap <buffer> PHI Φ
lnoremap <buffer> RHO Ρ
lnoremap <buffer> THETA Θ
lnoremap <buffer> ZETA Ζ
lnoremap <buffer> aumlaut ä
lnoremap <buffer> alpha α
lnoremap <buffer> a4 à
lnoremap <buffer> a3 ǎ
lnoremap <buffer> a2 á
lnoremap <buffer> a1 ā
lnoremap <buffer> bvv │
let s:cpo_save=&cpo
set cpo&vim
lnoremap <buffer> bhh ─
lnoremap <buffer> bul ┘
lnoremap <buffer> bur └
lnoremap <buffer> bdl ┐
lnoremap <buffer> bdr ┌
lnoremap <buffer> beta β
lnoremap <buffer> chi χ
lnoremap <buffer> dul ┛
lnoremap <buffer> dur ┗
lnoremap <buffer> ddl ┓
lnoremap <buffer> ddr ┏
lnoremap <buffer> dhh ┉
lnoremap <buffer> dhv ┋
lnoremap <buffer> dlh ┈
lnoremap <buffer> dlv ┊
lnoremap <buffer> dac ▾
lnoremap <buffer> dae ▸
lnoremap <buffer> delta δ
lnoremap <buffer> deg °
lnoremap <buffer> eumlaut ë
lnoremap <buffer> eta η
lnoremap <buffer> epsilon ε
lnoremap <buffer> e4 è
lnoremap <buffer> e3 ě
lnoremap <buffer> e2 é
lnoremap <buffer> e1 ē
lnoremap <buffer> geq ≥
lnoremap <buffer> gamma γ
lnoremap <buffer> iumlaut ï
lnoremap <buffer> i4 ì
lnoremap <buffer> i3 ǐ
lnoremap <buffer> i2 í
lnoremap <buffer> i1 ī
lnoremap <buffer> lar ←
lnoremap <buffer> leq ≤
lnoremap <buffer> lambda λ
lnoremap <buffer> mu µ
lnoremap <buffer> oumlaut ö
lnoremap <buffer> ohm Ω
lnoremap <buffer> o4 ò
lnoremap <buffer> o3 ǒ
lnoremap <buffer> o2 ó
lnoremap <buffer> o1 ō
lnoremap <buffer> pm ±
lnoremap <buffer> phi ϕ
lnoremap <buffer> rho ρ
lnoremap <buffer> theta θ
lnoremap <buffer> uumlaut ü
lnoremap <buffer> u4 ù
lnoremap <buffer> u3 ǔ
lnoremap <buffer> u2 ú
lnoremap <buffer> u1 ū
lnoremap <buffer> zeta ζ
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=symbols
setlocal noarabic
setlocal autoindent
setlocal backupcopy=
setlocal balloonexpr=
setlocal nobinary
setlocal nobreakindent
setlocal breakindentopt=
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal cindent
setlocal cinkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
set colorcolumn=+1
setlocal colorcolumn=+1
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'c'
setlocal filetype=c
endif
setlocal fixendofline
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
set foldmethod=indent
setlocal foldmethod=indent
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal formatprg=
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=-1
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal lispwords=
set list
setlocal list
setlocal makeencoding=
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=bin,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=ccomplete#Complete
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
set relativenumber
setlocal relativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal scrolloff=-1
setlocal shiftwidth=4
setlocal noshortname
setlocal sidescrolloff=-1
setlocal signcolumn=auto
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal noswapfile
setlocal synmaxcol=160
if &syntax != 'c'
setlocal syntax=c
endif
setlocal tabstop=4
setlocal tagcase=
setlocal tagfunc=
setlocal tags=
setlocal termwinkey=
setlocal termwinscroll=10000
setlocal termwinsize=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal undolevels=-123456
setlocal varsofttabstop=
setlocal vartabstop=
setlocal wincolor=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
let s:l = 1 - ((0 * winheight(0) + 7) / 15)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
1
normal! 0
wincmd w
argglobal
if bufexists("src/lib_to_fake_faked.c") | buffer src/lib_to_fake_faked.c | else | edit src/lib_to_fake_faked.c | endif
lnoremap <buffer> ALPHA Α
lnoremap <buffer> BETA Β
lnoremap <buffer> CHI Χ
lnoremap <buffer> Dhh ┅
lnoremap <buffer> Dhv ┇
lnoremap <buffer> Dlh ┄
lnoremap <buffer> Dlv ┆
lnoremap <buffer> DELTA Δ
lnoremap <buffer> ETA Η
lnoremap <buffer> EPSILON Ε
lnoremap <buffer> GAMMA Γ
lnoremap <buffer> PHI Φ
lnoremap <buffer> RHO Ρ
lnoremap <buffer> THETA Θ
lnoremap <buffer> ZETA Ζ
lnoremap <buffer> aumlaut ä
lnoremap <buffer> alpha α
lnoremap <buffer> a4 à
lnoremap <buffer> a3 ǎ
lnoremap <buffer> a2 á
lnoremap <buffer> a1 ā
lnoremap <buffer> bvv │
let s:cpo_save=&cpo
set cpo&vim
lnoremap <buffer> bhh ─
lnoremap <buffer> bul ┘
lnoremap <buffer> bur └
lnoremap <buffer> bdl ┐
lnoremap <buffer> bdr ┌
lnoremap <buffer> beta β
lnoremap <buffer> chi χ
lnoremap <buffer> dul ┛
lnoremap <buffer> dur ┗
lnoremap <buffer> ddl ┓
lnoremap <buffer> ddr ┏
lnoremap <buffer> dhh ┉
lnoremap <buffer> dhv ┋
lnoremap <buffer> dlh ┈
lnoremap <buffer> dlv ┊
lnoremap <buffer> dac ▾
lnoremap <buffer> dae ▸
lnoremap <buffer> delta δ
lnoremap <buffer> deg °
lnoremap <buffer> eumlaut ë
lnoremap <buffer> eta η
lnoremap <buffer> epsilon ε
lnoremap <buffer> e4 è
lnoremap <buffer> e3 ě
lnoremap <buffer> e2 é
lnoremap <buffer> e1 ē
lnoremap <buffer> geq ≥
lnoremap <buffer> gamma γ
lnoremap <buffer> iumlaut ï
lnoremap <buffer> i4 ì
lnoremap <buffer> i3 ǐ
lnoremap <buffer> i2 í
lnoremap <buffer> i1 ī
lnoremap <buffer> lar ←
lnoremap <buffer> leq ≤
lnoremap <buffer> lambda λ
lnoremap <buffer> mu µ
lnoremap <buffer> oumlaut ö
lnoremap <buffer> ohm Ω
lnoremap <buffer> o4 ò
lnoremap <buffer> o3 ǒ
lnoremap <buffer> o2 ó
lnoremap <buffer> o1 ō
lnoremap <buffer> pm ±
lnoremap <buffer> phi ϕ
lnoremap <buffer> rho ρ
lnoremap <buffer> theta θ
lnoremap <buffer> uumlaut ü
lnoremap <buffer> u4 ù
lnoremap <buffer> u3 ǔ
lnoremap <buffer> u2 ú
lnoremap <buffer> u1 ū
lnoremap <buffer> zeta ζ
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=symbols
setlocal noarabic
setlocal autoindent
setlocal backupcopy=
setlocal balloonexpr=
setlocal nobinary
setlocal nobreakindent
setlocal breakindentopt=
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal cindent
setlocal cinkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
set colorcolumn=+1
setlocal colorcolumn=+1
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'c'
setlocal filetype=c
endif
setlocal fixendofline
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
set foldmethod=indent
setlocal foldmethod=indent
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal formatprg=
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=-1
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal lispwords=
set list
setlocal list
setlocal makeencoding=
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal modeline
setlocal modifiable
setlocal nrformats=bin,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=ccomplete#Complete
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
set relativenumber
setlocal relativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal scrolloff=-1
setlocal shiftwidth=4
setlocal noshortname
setlocal sidescrolloff=-1
setlocal signcolumn=auto
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=160
if &syntax != 'c'
setlocal syntax=c
endif
setlocal tabstop=4
setlocal tagcase=
setlocal tagfunc=
setlocal tags=
setlocal termwinkey=
setlocal termwinscroll=10000
setlocal termwinsize=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal undolevels=-123456
setlocal varsofttabstop=
setlocal vartabstop=
setlocal wincolor=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
8
normal! zo
let s:l = 7 - ((3 * winheight(0) + 16) / 32)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
7
normal! 0
wincmd w
argglobal
if bufexists("build/TestSuite-results.md") | buffer build/TestSuite-results.md | else | edit build/TestSuite-results.md | endif
lnoremap <buffer> ALPHA Α
lnoremap <buffer> BETA Β
lnoremap <buffer> CHI Χ
lnoremap <buffer> Dhh ┅
lnoremap <buffer> Dhv ┇
lnoremap <buffer> Dlh ┄
lnoremap <buffer> Dlv ┆
lnoremap <buffer> DELTA Δ
lnoremap <buffer> ETA Η
lnoremap <buffer> EPSILON Ε
lnoremap <buffer> GAMMA Γ
lnoremap <buffer> PHI Φ
lnoremap <buffer> RHO Ρ
lnoremap <buffer> THETA Θ
lnoremap <buffer> ZETA Ζ
lnoremap <buffer> aumlaut ä
lnoremap <buffer> alpha α
lnoremap <buffer> a4 à
lnoremap <buffer> a3 ǎ
lnoremap <buffer> a2 á
lnoremap <buffer> a1 ā
lnoremap <buffer> bvv │
let s:cpo_save=&cpo
set cpo&vim
lnoremap <buffer> bhh ─
lnoremap <buffer> bul ┘
lnoremap <buffer> bur └
lnoremap <buffer> bdl ┐
lnoremap <buffer> bdr ┌
lnoremap <buffer> beta β
lnoremap <buffer> chi χ
lnoremap <buffer> dul ┛
lnoremap <buffer> dur ┗
lnoremap <buffer> ddl ┓
lnoremap <buffer> ddr ┏
lnoremap <buffer> dhh ┉
lnoremap <buffer> dhv ┋
lnoremap <buffer> dlh ┈
lnoremap <buffer> dlv ┊
lnoremap <buffer> dac ▾
lnoremap <buffer> dae ▸
lnoremap <buffer> delta δ
lnoremap <buffer> deg °
lnoremap <buffer> eumlaut ë
lnoremap <buffer> eta η
lnoremap <buffer> epsilon ε
lnoremap <buffer> e4 è
lnoremap <buffer> e3 ě
lnoremap <buffer> e2 é
lnoremap <buffer> e1 ē
lnoremap <buffer> geq ≥
lnoremap <buffer> gamma γ
lnoremap <buffer> iumlaut ï
lnoremap <buffer> i4 ì
lnoremap <buffer> i3 ǐ
lnoremap <buffer> i2 í
lnoremap <buffer> i1 ī
lnoremap <buffer> lar ←
lnoremap <buffer> leq ≤
lnoremap <buffer> lambda λ
lnoremap <buffer> mu µ
lnoremap <buffer> oumlaut ö
lnoremap <buffer> ohm Ω
lnoremap <buffer> o4 ò
lnoremap <buffer> o3 ǒ
lnoremap <buffer> o2 ó
lnoremap <buffer> o1 ō
lnoremap <buffer> pm ±
lnoremap <buffer> phi ϕ
lnoremap <buffer> rho ρ
lnoremap <buffer> theta θ
lnoremap <buffer> uumlaut ü
lnoremap <buffer> u4 ù
lnoremap <buffer> u3 ǔ
lnoremap <buffer> u2 ú
lnoremap <buffer> u1 ū
lnoremap <buffer> zeta ζ
let &cpo=s:cpo_save
unlet s:cpo_save
setlocal keymap=symbols
setlocal noarabic
setlocal autoindent
setlocal backupcopy=
setlocal balloonexpr=
setlocal nobinary
setlocal nobreakindent
setlocal breakindentopt=
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal nocindent
setlocal cinkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal cinoptions=
setlocal cinwords=if,else,while,do,for,switch
set colorcolumn=+1
setlocal colorcolumn=+1
setlocal comments=fb:*,fb:-,fb:+,n:>
setlocal commentstring=<!--%s-->
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal nocopyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'markdown'
setlocal filetype=markdown
endif
setlocal fixendofline
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=MarkdownFold()
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
set foldmethod=indent
setlocal foldmethod=expr
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=MarkdownFoldText()
setlocal formatexpr=
setlocal formatoptions=tcqln
setlocal formatlistpat=^\\s*\\d\\+\\.\\s\\+\\|^[-*+]\\s\\+\\|^\\[^\\ze[^\\]]\\+\\]:
setlocal formatprg=
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=-1
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},0),0],:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
setlocal nolinebreak
setlocal nolisp
setlocal lispwords=
set list
setlocal list
setlocal makeencoding=
setlocal makeprg=
setlocal matchpairs=(:),{:},[:],<:>
setlocal modeline
setlocal modifiable
setlocal nrformats=bin,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=htmlcomplete#CompleteTags
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal readonly
set relativenumber
setlocal relativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal scrolloff=-1
setlocal shiftwidth=4
setlocal noshortname
setlocal sidescrolloff=-1
setlocal signcolumn=auto
setlocal nosmartindent
setlocal softtabstop=4
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=160
if &syntax != 'markdown'
setlocal syntax=markdown
endif
setlocal tabstop=4
setlocal tagcase=
setlocal tagfunc=
setlocal tags=
setlocal termwinkey=
setlocal termwinscroll=10000
setlocal termwinsize=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal undolevels=-123456
setlocal varsofttabstop=
setlocal vartabstop=
setlocal wincolor=
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
let s:l = 4 - ((3 * winheight(0) + 2) / 4)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
4
normal! 0
wincmd w
6wincmd w
exe '1resize ' . ((&lines * 7 + 20) / 41)
exe 'vert 1resize ' . ((&columns * 88 + 88) / 177)
exe '2resize ' . ((&lines * 4 + 20) / 41)
exe 'vert 2resize ' . ((&columns * 88 + 88) / 177)
exe '3resize ' . ((&lines * 1 + 20) / 41)
exe 'vert 3resize ' . ((&columns * 88 + 88) / 177)
exe '4resize ' . ((&lines * 1 + 20) / 41)
exe 'vert 4resize ' . ((&columns * 88 + 88) / 177)
exe '5resize ' . ((&lines * 15 + 20) / 41)
exe 'vert 5resize ' . ((&columns * 88 + 88) / 177)
exe '6resize ' . ((&lines * 32 + 20) / 41)
exe 'vert 6resize ' . ((&columns * 88 + 88) / 177)
exe '7resize ' . ((&lines * 4 + 20) / 41)
tabnext 4
set stal=1
badd +138 /cygdrive/c/chromation-dropbox/Dropbox/c/LIS-770i/mBrd/src/mBrd.c
badd +1582 ~/chromation/dev-kit-mike/README.md
badd +1 ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/manual-lis-config-example.c
badd +1 ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/SensorVis.c
badd +1 ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/lis-config-old-working.lst
badd +1 ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/lis-config-new-fixed.lst
badd +53 ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/lis-config-new-broken.lst
badd +1 /cygdrive/c/Program\ Files\ (x86)/Atmel/Studio/7.0/toolchain/avr8/avr8-gnu-toolchain/avr/include/avr/iom328p.h
badd +1 ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/Spi-Hardware.h
badd +1 ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/WriteSpiMaster-is-a-func-ptr.lst
badd +1 ~/chromation/dev-kit-mike/firmware/vis-spi-out/build/vis-spi-out.lst
badd +6562 /cygdrive/c/Users/Mike/Documents/chromation/txt/updates/notes-for-update/notes_for_update.md
badd +1307 ~/chromation/dev-kit-mike/firmware/vis-spi-out/test/test_SensorVis.c
badd +148 ~/chromation/dev-kit-mike/firmware/lib/src/Lis.c
badd +162 ~/chromation/dev-kit-mike/firmware/vis-spi-out/test/test_runner.c
badd +109 ~/chromation/dev-kit-mike/firmware/lib/src/Lis.h
badd +976 ~/.vimrc
badd +45 /cygdrive/c/chromation-dropbox/Dropbox/c/LIS-770i/lib/src/Lis.c
badd +518 ~/chromation/dev-kit-mike/firmware/vis-spi-out/build/p_vis-spi-out.lst
badd +937 /cygdrive/c/chromation-dropbox/Dropbox/c/LIS-770i/mBrd/build/mBrd.lst
badd +75 ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/vis-spi-out.c
badd +30 ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/UartSpi-Hardware.h
badd +153 ~/chromation/dev-kit-mike/firmware/lib/src/UartSpi.c
badd +10 /cygdrive/c/chromation-dropbox/Dropbox/c/LIS-770i/lib/src/ReadWriteBits.h
badd +1 ~/chromation/dev-kit-mike/firmware/vis-spi-out/build/vis-spi-out.elf
badd +2091 ~/chromation/dev-kit-mike/firmware/usb-bridge/build/usb-bridge.lst
badd +66 ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/SensorVis.h
badd +47 ~/chromation/dev-kit-mike/firmware/lib/src/Pwm.c
badd +27 ~/chromation/dev-kit-mike/firmware/lib/test/fake/Lis-Hardware.h
badd +41 ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/Lis-Hardware.h
badd +2 /cygdrive/c/Program\ Files\ (x86)/Atmel/Studio/7.0/toolchain/avr8/avr8-gnu-toolchain/avr/include/avr/io.h
badd +21 /cygdrive/c/chromation-dropbox/Dropbox/c/LIS-770i/lib/src/Lis.h
badd +6 ~/chromation/dev-kit-mike/firmware/lib/src/ReadWriteBits.c
badd +63 ~/chromation/dev-kit-mike/firmware/lib/src/BiColorLed.h
badd +142 ~/chromation/dev-kit-mike/firmware/lib/src/Ft1248.c
badd +32 ~/chromation/dev-kit-mike/firmware/lib/src/ReadWriteBits.h
badd +1 In\ file\ included\ from\ ../lib/src/ReadWriteBits.c
badd +1 In\ file\ included\ from\ src/vis-spi-out.c
badd +6 ~/chromation/dev-kit-mike/firmware/lib/src/deprecated/ReadWriteBits.c
badd +36 ~/chromation/dev-kit-mike/firmware/lib/src/deprecated/ReadWriteBits.h
badd +11 ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/deprecated/UartSpi-initial.c
badd +147 ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/deprecated/old-vis-spi-out.c
badd +1 ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/deprecated/Spi-Commands.h
badd +28 ~/chromation/dev-kit-mike/firmware/lib/src/UartSpi.h
badd +42 /cygdrive/c/chromation-dropbox/Dropbox/c/LIS-770i/mBrd/src/Lis-Hardware.h
badd +33 /cygdrive/c/chromation-dropbox/Dropbox/c/LIS-770i/mBrd/src/UartSpi-Hardware.h
badd +55 ~/chromation/dev-kit-mike/firmware/lib/test/test_Auto.c
badd +40 /cygdrive/c/chromation-dropbox/Dropbox/c/LIS-770i/mBrd/src/Spi-Commands.h
badd +1 /cygdrive/c/chromation-dropbox/Dropbox/c/LIS-770i/mBrd/src/Spi-Hardware.h
badd +1 /cygdrive/c/chromation-dropbox/Dropbox/c/LIS-770i/mBrd/src/Pwm-Hardware.h
badd +31 /cygdrive/c/chromation-dropbox/Dropbox/c/LIS-770i/lib/src/UartSpi.h
badd +4 ~/chromation-reference.md
badd +2 /cygdrive/c/chromation-dropbox/Dropbox/labview\ programs/LabVIEW\ 2012/2018/LisSweep/map-files/maps/0911-27_map.txt
badd +260 ~/chromation/dev-kit-mike/firmware/lib/test/test_UartSpi.c
badd +476 ~/chromation/dev-kit-mike/firmware/usb-bridge/src/bridge.c
badd +19 ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/deprecated/old-peak-counts-algorithm.c
badd +99 ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/LisReadOnePixel-remove-global-pframe.c
badd +148 ~/chromation/dev-kit-mike/firmware/lib/test/test_Spi.c
badd +86 ~/chromation/dev-kit-mike/firmware/lib/src/Spi.c
badd +24 ~/chromation/dev-kit-mike/firmware/lib/src/Spi.h
badd +333 ~/chromation/dev-kit-mike/firmware/lib/test/test_runner.c
badd +665 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/mock-c/test/unity/unity_internals.h
badd +1 ~/chromation/dev-kit-mike/firmware/lib/make:\ \*\*\*\ \[Makefile
badd +61 ~/chromation/dev-kit-mike/firmware/vis-spi-out/test/test_SensorVis.h
badd +3 ~/chromation/dev-kit-mike/firmware/lib/test/test_Lis.c
badd +22 ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/bad-embedded-c-ProgramPhotodiodeArray.c
badd +50 ~/chromation/dev-kit-mike/firmware/vis-spi-out/test/deprecated_tests_for_RepresentConfigAs28bits.c
badd +1 /cygdrive/c/chromation-dropbox/Dropbox/labview\ programs/LabVIEW\ 2012/2018/LisSweep/map-files/maps/1002-06_map.txt
badd +7 /cygdrive/c/chromation-dropbox/Dropbox/python/serial_number_project/example.py
badd +291 ~/.bashrc
badd +13 /cygdrive/c/chromation-dropbox/Dropbox/c/LIS-770i/readme-flash.md
badd +1 /cygdrive/c/chromation-dropbox/Dropbox/ChromationData/spectrometer/1002-04/cal/wavelength/meas/nm-sweep/raw-response-matrix.txt
badd +1 /cygdrive/c/chromation-dropbox/Dropbox/ChromationData/spectrometer/1002-20/cal/wavelength/analysis/map.txt
badd +1 /cygdrive/c/chromation-dropbox/Dropbox/ChromationData/spectrometer/1002-23/cal/wavelength/meas/nm-sweep/raw-response-matrix.txt
badd +103 ~/chromation/dev-kit-mike/firmware/vis-spi-out/Makefile
badd +1 In\ file\ included\ from\ ../lib/src/Pwm.h
badd +1 from\ src/vis-spi-out.c
badd +353 ~/chromation/dev-kit-mike/firmware/vis-spi-out/src/deprecated/SensorVis-old.c
badd +27 ~/chromation/dev-kit-mike/firmware/lib/test/fake/Spi-Hardware.h
badd +6529 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff/fff.h
badd +48 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff/README.md
badd +14 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff/test/fff_test_c.c
badd +38 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff/test/fff_test_global_c.c
badd +1 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff/test/fff_test_cpp.cpp
badd +1 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff/test/global_fakes.c
badd +27 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff/test/global_fakes.h
badd +81 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff/test/Makefile
badd +16 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff/test/test_cases.include
badd +11 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff/examples/driver_testing/driver.test.fff.cpp
badd +18 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff/examples/embedded_ui/UI_test_ansic.c
badd +77 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff/examples/embedded_ui/UI_test_cpp.cpp
badd +92 src/main.c
badd +23 Makefile
badd +1 ~/chromation/dev-kit-mike/firmware/vis-spi-out/make:\ \*\*\*\ \[Makefile
badd +1 build/TestSuite-results.md
badd +1 make:\ \*\*\*\ \[Makefile
badd +3769 ~/programming-reference.md
badd +7074 ~/read-the-docs/man-gcc.man
badd +1443 build/preproc.c
badd +5 src/lib_to_fake.h
badd +9 src/lib_to_fake.c
badd +12 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff/examples/embedded_ui/UI.c
badd +14 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff/examples/embedded_ui/Makefile
badd +6 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff/examples/embedded_ui/UI.h
badd +1 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff/gtest/gtest-main.cc
badd +54 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff/gtest/gtest.h
badd +382 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff/gtest/gtest-all.cc
badd +1 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff/gtest/Makefile
badd +11 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff/examples/embedded_ui/test_suite_template.c
badd +1 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff/examples/embedded_ui/make:\ \*\*\*\ \[Makefile
badd +4 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff/Makefile
badd +3 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff/examples/weak_linking/test/display.fake.c
badd +25 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff/examples/embedded_ui/Kata.txt
badd +13 /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/fff/examples/embedded_ui/DISPLAY.h
badd +19 src/lib_to_fake_faked.c
badd +5 src/lib_to_fake_faked.h
badd +1 build/TestSuite-results.md
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
nohlsearch
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
