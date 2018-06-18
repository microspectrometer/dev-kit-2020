This file lists the functionality required by my Vim tool `;me` for mocking expected calls.
I implemented everything here. See the unfinished task at the end.

Changed to ;me (mock expected call)
;yh changed to ;fh (function to header)
added ;eh (extern declaration to header)

DOF lib: Ft1248
DOF: FtOutputCmdOnMiosio
=====[ Put this in proj/src/Ft1248.c ]=====
The FUT lib is in the test file name. The DOF lib is a little harder to figure
out. It is the #include "mock_blah.h". If there is more than one mock_ header,
type the macro that is already defined in the mocked lib header. The
compiler is fine with _MOCK_FT1248_H; as a line of do-nothing code. And Vim
can use this with the tags file to jump to that file.

[x] Determine the FUT lib. -- Uses the file name.

[x] Determine the DOF lib. -- Inspects the #include headers or uses an explicit _MOCK_blah_H;

Also See :h tagsrch for help with using _MOCK_blah_H. I could not find a way to collect the filename without actually switching buffers.
:h character-classes

[x] determine the datatype of the input argument

[x] if a DOF_Implementation already exists, ;ym does nothing.

[x] if the DOF already exists but has no implementation, ;ym does nothing.

- TODO: ;ym should rename the DOF as DOF_Implementation, then create the
  function pointer.

[x] The following are appended to the end of the DOF lib C file.

static void FtOutputCmdOnMiosio_Implementation(uint8_t FtCmd)
{}
void (*FtOutputCmdOnMiosio)(uint8_t FtCmd) = FtOutputCmdOnMiosio_Implementation;

expectmocking.vim supports everything up to here.
=====[ Put this in proj/src/Ft1248.h ]=====
[x] use ;eh to place extern declaration of function pointer in .h

extern void (*FtOutputCmdOnMiosio)(uint8_t FtCmd);
=====[ Put this in proj/test/test_Ft1248.c ]=====
#include "Ft1248.h" // DOF lib
[x] Require that DOL.c and DOL.h already exist.

FUT lib: Ft1248
FUT: FtSendCommand
=====[ Put this in proj/test/test_Ft1248.c ]=====
SetUp_FUT and TearDown_FUT will always exist already. I have to make those to
get the basic test plumbing up and running.
[x] do not add the calls to SetUpMock_FUT and TearDownMock_FUT if they already exist.
    Search for those calls first. There will never be two SetUp_FUT calls that
    call the same SetUpMock_FUT call.
[x] search for SetUp_FUT and TearDown_FUT and add these as the first calls.
void SetUp_FtSendCommand(void){
    SetUpMock_FtSendCommand();    // create the mock object to record calls
    // other setup code
}
void TearDown_FtSendCommand(void){
    TearDownMock_FtSendCommand(); // destroy the mock object
    // other teardown code
}

[x] Do nothing if test_LUT_MockUps .c and .h do not exist.

[ ] TODO: create test_LUT_MockUps .c and .h if they do not exist.

#include "test_Ft1248_MockUps.h"    // mockups for testing each FUT in FUT lib
[x] if the include already exists, do not include it again

FUT lib: Ft1248
FUT: FtSendCommand
=====[ Put this in proj/test/test_Ft1248_MockUps.c ]=====
[x] if SetUpMock_FUT already exists, do not create a new definition.
void SetUpMock_FtSendCommand(void)  // FUT
{
    mock = Mock_new();
    //
}
void TearDownMock_FtSendCommand(void)  // FUT
{
    Mock_destroy(mock); mock = NULL;
    //
}
=====[ Put this in proj/test/test_Ft1248_MockUps.h ]=====
[x] if function definitions already exist, do not add prototypes to header.
;yh on the function definitions will do it
void SetUpMock_FtSendCommand(void); void TearDownMock_FtSendCommand(void);

FUT lib: Ft1248
DOF: FtOutputCmdOnMiosio
=====[ Put this in proj/test/test_Ft1248_MockUps.c ]=====
static void (*FtOutputCmdOnMiosio_Saved)(uint8_t FtCmd);  // DOF
static void Stub_FtOutputCmdOnMiosio(void) {
    FtOutputCmdOnMiosio_Saved = FtOutputCmdOnMiosio;
    FtOutputCmdOnMiosio = FtOutputCmdOnMiosio_Stubbed;
}
static void Unstub_FtOutputCmdOnMiosio(void) {
    FtOutputCmdOnMiosio = FtOutputCmdOnMiosio_Saved;
}

[x] Insert the Stub_FUT as the last call in the definition.
    The SetUpMock_FUT and TearDownMock_FUT will already exist. If not, then this
    call of ;ym will have created them.
void SetUpMock_FtSendCommand(void)
{
    mock = Mock_new();
    //
    Stub_FtOutputCmdOnMiosio();  // DOF
}
void TearDownMock_FtSendCommand(void)
{
    Mock_destroy(mock); mock=NULL;
    //
    Unstub_FtOutputCmdOnMiosio();  // DOF
}

The hard part here is dealing with arg1, arg2, etc. I need to generate these.
Work with dof.argtypes to generate (type1 arg1, type2 arg2, etc.).
Then generate lines for each arg.

DOF lib: Ft1248 (dof.lib)
DOF: FtOutputCmdOnMiosio (dof.Mock)
=====[ Put this in proj/test/mock_Ft1248.c ]=====
[x] Check if Mock_DOF definition already exists (:tag 'Mock_'.dof.name)
    if the Mock_DOF definition already exists then do nothing to this file or to
    its .h
[x] Include these headers only if they are not already included
#include "mock_Ft1248.h"
#include <Mock.h>           // RecordExpectedCall, RecordActualCall
#include <RecordedCall.h>   // Class: RecordedCall
#include <RecordedArg.h>    // Class: RecordedArg

[x] Write this code
static RecordedCall * Mock_FtOutputCmdOnMiosio(uint8_t arg1)
{
    char const *call_name = "FtOutputCmdOnMiosio";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_uint8);
    *((uint8_t *)record_of_arg1->pArg) = arg1;
    RecordArg(record_of_this_call, record_of_arg1);
    return record_of_this_call;
}
void Expect_FtOutputCmdOnMiosio(uint8_t arg1) {
    RecordExpectedCall(mock, Mock_FtOutputCmdOnMiosio(arg1));
}
void FtOutputCmdOnMiosio_Stubbed(uint8_t arg1) {
    RecordActualCall(mock, Mock_FtOutputCmdOnMiosio(arg1));
}
=====[ Put this in proj/test/mock_Ft1248.h ]=====
void Expect_FtOutputCmdOnMiosio(uint8_t arg1);
void FtOutputCmdOnMiosio_Stubbed(uint8_t arg1);


No, this does not look right. This is already defined in `dol.src.files.h`.
Do not add these:
void (*FtOutputCmdOnMiosio)(uint8_t FtCmd);
void (*FtOutputCmdOnMiosio)(uint8_t FtCmd);

---Left off here, writing MockExpectedCall().

[ ] Final step: go back and auto-generate files that do not exist yet instead of refusing to do anything if files do not exist.
test_LUT_MockUps.c and .h should be auto-generated. The mock_DOL.c and .h too.
