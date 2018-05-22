#include <stdio.h>         // printf
#include <Mock.h>          // used by all test client code
#include <RecordedCall.h>  // used by *DOF* `mock_lib.c`
#include <RecordedArg.h>   // used by *DOF* `mock_lib.c`

void (*setUp)(void); void (*tearDown)(void);
void SetUp_Mock(void){ mock = Mock_new(); }
void TearDown_Mock(void){ Mock_destroy(mock); mock = NULL; }
Mock_s *mock;

int main()
{
    setUp = SetUp_Mock; tearDown = TearDown_Mock;
    setUp();
    printf("# Linking object files.\n");
    printf(
        "\nThe right way to link in the unit test libs I created in `mock-c`\n"
        "is to turn those `.o` files into a *Static Library*:\n"
        "https://medium.com/@meghamohan/"
        "everything-you-need-to-know-about-libraries-in-c-e8ad6138cbb4\n"
        "and link that library with `-llibrary`.\n"
        "Note there is *no space* between `-l` and the library name.\n"
        "Here is *GNU* documentation on the `-l` flag for `gcc`:\n"
        "> Search the library named library when linking.\n"
        "> \n"
        "> The linker searches a standard list of directories for the\n"
        "> library, which is actually a file named liblibrary.a.  The linker\n"
        "> then uses this file as if it had been specified precisely by name.\n"
        "> The directories searched include several standard system\n"
        "> directories plus any that you specify with -L.\n"
        "> \n"
        "> Normally the files found this way are library files---archive\n"
        "> files whose members are object files.  The linker handles an\n"
        "> archive file by scanning through it for members which define\n"
        "> symbols that have so far been referenced but not defined.\n"
        "\nThe *Makefile* recipe lists `unity.o` as a prerequisite.\n"
        "My modification of `unity.c` gives `setUp` and `tearDown`\n"
        "*external linkage* and makes them function pointers.\n"
        "Even without including `unity.h` in this file,\n"
        "these function pointers must be defined *and* assigned.\n"
        "\nI define them globally:\n"
        "```c\n"
        "void (*setUp)(void); void (*tearDown)(void);\n"
        "```\n"
        "\nAnd inside `main()` I assign them to definitions:\n"
        "```c\n"
        "setUp = SetUp_Mock; tearDown = TearDown_Mock;\n"
        "```\n"
        );
    tearDown();
}

//=====[ sort includes with this client code example ]=====
// These all go in the *DOF* `mock_lib.c`.
// *Expect_DOF* and *DOF_Stubbed* prototypes go in the *DOF* `mock_lib.h`.
static RecordedCall * Mock_TakesOneArg(uint8_t b1)
{
    char const *call_name = "TakesOneArg";
    RecordedCall *record_of_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_uint8);
    *((uint8_t *)record_of_arg1->pArg) = b1;
    RecordArg(record_of_call, record_of_arg1);
    return record_of_call;
}
void Expect_TakesOneArg(uint8_t b1) {
    RecordExpectedCall(mock, Mock_TakesOneArg(b1));
}
void TakesOneArg_Stubbed(uint8_t b1) {
    RecordActualCall(mock, Mock_TakesOneArg(b1));
}
