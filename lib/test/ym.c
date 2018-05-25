#include "ym.h"
[x] Modify ;yh to decouple file creation and #include insertion in .c
    ;yh creates the file if it needs to.
    separately, ;yh should check to see if the #include .h is already in the .c
    file and add the line if it is not
    - I refactored AddFunctionToHeader().
[x] Modify ;yh to not add the function signature if it is already in the header
    file.

DOF lib: Ft1248
DOF: FtOutputCmdOnMiosio
=====[ Put this in proj/src/Ft1248.c ]=====
The FUT lib is in the test file name. The DOF lib is a little harder to figure
out. It is the #include "mock_blah.h". If there is more than one mock_ header,
type the macro that is already defined in the mocked lib header. The
compiler is fine with _MOCK_FT1248_H; as a line of do-nothing code. And Vim
can use this with the tags file to jump to that file.

I thought about parsing comments instead, but using macros means the compiler is
checking for me that the file exists. The downside to this is that the file has
to already exist, and the line with the Expect_ call looks a bit ugly. But then
again, it is temporary. I can make a clean command that scours the file and
deletes every instance of _MOCK_blah_H; Also, if the test file only uses one
mock_ file, I can gleam that from the #includes at the top of the file.

[x] Determine the FUT lib. -- Uses the file name.

[x] Determine the DOF lib. -- Inspects the #include headers or uses an explicit _MOCK_blah_H;

Also See :h tagsrch for help with using _MOCK_blah_H. I could not find a way to collect the filename without actually switching buffers.

[ ] if the DOF_Implementation already exists, ;ym will cause a second definition
    of the DOF_Implementation, which will be caught by the compiler.
    But it would nice if...
    if the DOF_Implementation already exists, ;ym should do nothing to this file
[ ] if the DOF already exists but has no implementation, ;ym will cause a second
    definition of the DOF, which will be caught by the compiler
    But it would nice if...
    if the DOF already exists, ;ym should rename the DOF as DOF_Implementation,
    then create the function pointer.
static void FtOutputCmdOnMiosio_Implementation(uint8_t FtCmd)
{}
void (*FtOutputCmdOnMiosio)(uint8_t FtCmd) = FtOutputCmdOnMiosio_Implementation;
=====[ Put this in proj/src/Ft1248.h ]=====
;yh on the fp will do it
[ ] make ;yh detect when it is on a function pointer and add the `extern` prefix
[ ] It would be nice if...
    if the prototype for a non-function pointer version exists in the header,
    delete it
extern void (*FtOutputCmdOnMiosio)(uint8_t FtCmd);
=====[ Put this in proj/test/test_Ft1248.c ]=====
#include "Ft1248.h" // DOF lib
[ ] if the include already exists, do not include it again

FUT lib: Ft1248
FUT: FtSendCommand
=====[ Put this in proj/test/test_Ft1248.c ]=====
SetUp_FUT and TearDown_FUT will always exist already. I have to make those to
get the basic test plumbing up and running.
[ ] do not add the calls to SetUpMock_FUT and TearDownMock_FUT if they already exist.
    Search for those calls first. There will never be two SetUp_FUT calls that
    call the same SetUpMock_FUT call.
[ ] search for SetUp_FUT and TearDown_FUT and add these as the first calls.
void SetUp_FtSendCommand(void){
    SetUpMock_FtSendCommand();    // create the mock object to record calls
    // other setup code
}
void TearDown_FtSendCommand(void){
    TearDownMock_FtSendCommand(); // destroy the mock object
    // other teardown code
}
#include "test_Ft1248_MockUps.h"    // mockups for testing each FUT in FUT lib
[ ] if the include already exists, do not include it again

FUT lib: Ft1248
FUT: FtSendCommand
=====[ Put this in proj/test/test_Ft1248_MockUps.c ]=====
[ ] if SetUpMock_FUT already exists, do not create a new definition.
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
[ ] if function definitions already exist, do not add prototypes to header.
;yh on the function definitions will do it
void SetUpMock_FtSendCommand(void); void TearDownMock_FtSendCommand(void);

DOF lib: Ft1248
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
[ ] Insert the Stub_FUT as the last call in the definition.
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

DOF lib: Ft1248
DOF: FtOutputCmdOnMiosio
=====[ Put this in proj/test/mock_Ft1248.c ]=====
[ ] if the Mock_DOF definition already exists then do nothing to this file or to
    its .h
[ ] Include these headers only if they are not already included
#include "mock_Ft1248.h"
#include <Mock.h>           // RecordExpectedCall, RecordActualCall
#include <RecordedCall.h>   // Class: RecordedCall
#include <RecordedArg.h>    // Class: RecordedArg

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


void (*FtOutputCmdOnMiosio)(uint8_t FtCmd);
void (*FtOutputCmdOnMiosio)(uint8_t FtCmd);


