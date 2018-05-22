#include <stdio.h>         // printf
#include <Mock.h>          // used by all test client code
#include <RecordedCall.h>  // used by *DOF* `mock_lib.c`
#include <RecordedArg.h>   // used by *DOF* `mock_lib.c`

void (*setUp)(void); void (*tearDown)(void);
void SetUp_Mock(void){ mock = Mock_new(); }
void TearDown_Mock(void){ Mock_destroy(mock); mock = NULL; }
Mock_s *mock;

void PrintDocumentation(void)
{
    printf("# Linking object files\n");
    printf("## The right way\n"
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
        "\n"
        );
    printf("## The temporary hack way\n"
        "\nThe *Makefile* recipe lists the object files as prerequisites:\n"
        "```make\n"
        "build/%%.exe: src/%%.c ${unit-test-lib-objects}\n"
        "    ${compiler} $^ -o $@ $(LFLAGS) $(CFLAGS) -I src/\n"
        "```\n"
        "and `CFLAGS` has the include paths for the header files:\n"
        "```make\n"
        "CFLAGS = -I ${mock-c-lib}include/\n"
        "```\n"
        );
    printf("## Linking aginst the unity object file"
           " requires definitions of setUp and tearDown.\n"
        "nThe *Makefile* recipe lists `unity.o` as a prerequisite.\n"
        "This is part of `${unit-test-lib-objects}`.\n"
        "The `unity.o` filename,\n"
        "and the path to this file in my `mock-c` project,\n"
        "are part of the `${unit-test-lib-objects}` variable.\n"
        "\nThe `unity.o` object file has undefined symbols for\n"
        "`setUp` and `tearDown`. This is because\n"
        "my modification of `unity.c` gives `setUp` and `tearDown`\n"
        "*external linkage* and makes them function pointers.\n"
        "Interestingly, the mere act of linking against `unity.o`\n"
        "causes *clang* to throw an *undefined reference* error.\n"
        "Therefore, even without including `unity.h` in this file,\n"
        "these function pointers must be *defined and assigned*.\n"
        "\nI *define* them globally:\n"
        "```c\n"
        "void (*setUp)(void); void (*tearDown)(void);\n"
        "```\n"
        "\nAnd inside `main()` I *assign* them to definitions:\n"
        "```c\n"
        "setUp = SetUp_Mock; tearDown = TearDown_Mock;\n"
        "```\n"
        );
}

//=====[ working out how to include files using this client code example ]=====
// *Expect_DOF* and *DOF_Stubbed* prototypes go in the *DOF* `mock_lib.h`.
// #include <mock_lib.h>
void Expect_TakesOneArg(uint8_t b1);
void TakesOneArg_Stubbed(uint8_t b1);
int main()
{
    setUp = SetUp_Mock; tearDown = TearDown_Mock;
    setUp();
    printf("# Example using a mock object\n");
    printf("## Print the *Recorded Call* Lists\n");
    //=====[ Generate the list of expected calls ]=====
    Expect_TakesOneArg(0x12);
    //=====[ Simulate stubbed DOF calls that happen when FUT is called ]=====
    TakesOneArg_Stubbed(0x21);
    //=====[ Walk the lists of calls ]=====
    PrintAllCalls(mock);
    if (RanAsHoped(mock)) printf("\n---Ran as hoped.---\n");
    else printf(
        "\n## Print the *Why it failed* message\n%s\n", WhyDidItFail(mock));
    tearDown();
    PrintDocumentation();
}

// These all go in the *DOF* `mock_lib.c`.
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
