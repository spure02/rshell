#include "gtest/gtest.h"

#include "../header/Parser.h"
#include "../header/cmd.h"
#include "../header/Connector.h"
#include "../header/TestCmd.h"
#include "../header/Semicolon.h"
#include "../header/Ampersand.h"
#include "../header/dPipe.h"
#include "../header/inputRed.h"
#include "../header/outputRed.h"
#include "../header/Append.h"
#include "../header/sPipe.h"

#include <iostream>

/*********************** ASSIGNMENT 2 TESTS ************************/

//testing parse for ;
TEST(Assignment2, SemicolonTest1) {
    Parser semiParser;
    std::vector<std::string> semiVec;

    semiVec.push_back("ls");
    semiVec.push_back("-a");
    semiVec.push_back(";");
    semiVec.push_back("echo");
    semiVec.push_back("yeet");

    semiParser.parse("ls -a ; echo yeet");

    EXPECT_EQ(semiVec, semiParser.getData());
}//SemicolonTest1

//testing execute for ;
TEST(Assignment2, SemicolonTest2) {
    std::vector<std::string> lefty;
    std::vector<std::string> righty;

    lefty.push_back("ls");
    lefty.push_back("-a");
    righty.push_back("echo");
    righty.push_back("yeet");

    Cmd* arg1 = new Cmd(lefty);
    Cmd* arg2 = new Cmd(righty);

    Semicolon* semiTest = new Semicolon(arg1, arg2);

    EXPECT_EQ(true, semiTest->execute(0, 1));
}//SemicolonTest2

//testing parse for &&
TEST(Assignment2, AmpersandTest1) {
    Parser ampParser;
    std::vector<std::string> ampVec;

    ampVec.push_back("echo");
    ampVec.push_back("hello");
    ampVec.push_back("&&");
    ampVec.push_back("echo");
    ampVec.push_back("world");

    ampParser.parse("echo hello && echo world");

    EXPECT_EQ(ampVec, ampParser.getData());
}//AmpersandTest1

//testing execute for &&
TEST(Assignment2, AmpersandTest2) {
    std::vector<std::string> lefty;
    std::vector<std::string> righty;

    lefty.push_back("echo");
    lefty.push_back("hello");
    righty.push_back("echo");
    righty.push_back("world");

    Cmd* arg1 = new Cmd(lefty);
    Cmd* arg2 = new Cmd(righty);

    Ampersand* ampTest = new Ampersand(arg1, arg2);

    EXPECT_EQ(true, ampTest->execute(0, 1));
}//AmpersandTest2

//testing parse for ||
TEST(Assignment2, dPipeTest1) {
    Parser dPipeParser;
    std::vector<std::string> dPipeVec;

    dPipeVec.push_back("echo");
    dPipeVec.push_back("wheat");
    dPipeVec.push_back("||");
    dPipeVec.push_back("echo");
    dPipeVec.push_back("yeet");

    dPipeParser.parse("echo wheat || echo yeet");

    EXPECT_EQ(dPipeVec, dPipeParser.getData());
}//dPipeTest1

//testing execute for ||
TEST(Assignment2, dPipeTest2) {
    std::vector<std::string> lefty;
    std::vector<std::string> righty;

    lefty.push_back("echo");
    lefty.push_back("wheat");
    righty.push_back("echo");
    righty.push_back("yeet");

    Cmd* arg1 = new Cmd(lefty);
    Cmd* arg2 = new Cmd(righty);

    dPipe* dpipeTest = new dPipe(arg1, arg2);

    EXPECT_EQ(true, dpipeTest->execute(0, 1));
}//dPipeTest2

//testing parse for multiple connectors
TEST(Assignment2, MultipleConnectorsTest1) {
    Parser multiParser;
    std::vector<std::string> multipleConVec;

    multipleConVec.push_back("ls");
    multipleConVec.push_back("-a");
    multipleConVec.push_back(";");
    multipleConVec.push_back("echo");
    multipleConVec.push_back("hello");
    multipleConVec.push_back("&&");
    multipleConVec.push_back("echo");
    multipleConVec.push_back("world");
    multipleConVec.push_back("||");
    multipleConVec.push_back("ls");

    multiParser.parse("ls -a ; echo hello && echo world || ls");

    EXPECT_EQ(multipleConVec, multiParser.getData());
}//MultipleConnectorsTest1

//testing execute for multiple connectors
TEST(Assignment2, MultipleConnectorsTest2) {
    std::vector<std::string> lsVec;
    lsVec.push_back("ls");
    lsVec.push_back("-a");

    std::vector<std::string> echoHello;
    echoHello.push_back("echo");
    echoHello.push_back("hello");

    Cmd* lsArg = new Cmd(lsVec);
    Cmd* helloArg = new Cmd(echoHello);

    Connector* semi1 = new Semicolon(lsArg, helloArg);

    std::vector<std::string> echoWorld;
    echoWorld.push_back("echo");
    echoWorld.push_back("world");

    Cmd* worldArg = new Cmd(echoWorld);

    Connector* amp1 = new Ampersand(semi1, worldArg);

    std::vector<std::string> justLsVec;
    justLsVec.push_back("ls");

    Cmd* justLsArg = new Cmd(justLsVec);

    Connector* dpipe1 = new dPipe(amp1, justLsArg);

    EXPECT_EQ(true, dpipe1->execute(0, 0));
}//MultipleConnectorsTest2

/*********************** ASSIGNMENT 3 TESTS ************************/

//testing execute for test -e
TEST(Assignment3, eTestCmdTest) {
    std::vector<std::string> eTestVec;

    eTestVec.push_back("test");
    eTestVec.push_back("-e");
    eTestVec.push_back("rshell");

    Cmd* eTest = new TestCmd(eTestVec);
    EXPECT_EQ(true, eTest->execute(0, 0));
}//eTestCmdTest

//testing execute for test -f 
TEST(Assignment3, fTestCmdTest) {
    std::vector<std::string> f_TestVec;

    f_TestVec.push_back("test");
    f_TestVec.push_back("-f");
    f_TestVec.push_back("Makefile");

    Cmd* f_TestCmd = new TestCmd(f_TestVec);
    EXPECT_EQ(true, f_TestCmd->execute(0, 0));
}//fTestCmdTest

//testing execute for test -f
TEST(Assignment3, dTestCmdTest) {
    std::vector<std::string> dTestVec;

    dTestVec.push_back("test");
    dTestVec.push_back("-d");
    dTestVec.push_back("hello");

    Cmd* dTestCmd = new TestCmd(dTestVec);
    EXPECT_EQ(false, dTestCmd->execute(0, 0));
}//dTestCmdTest

//testing parse for symbolic testing [ ]
TEST(Assignment3, symbolicTest_e1) {
    Parser symbolic_eParser;
    std::vector<std::string> symbolicVec;

    symbolicVec.push_back("test");
    symbolicVec.push_back("-e");
    symbolicVec.push_back("rshell");
    //symbolicVec.push_back("]");

    symbolic_eParser.parse("[ -e rshell ]");
    EXPECT_EQ(symbolicVec, symbolic_eParser.getData());
}//symbolicTest_e1

/*********************** ASSIGNMENT 4 TESTS ************************/

//testing parsing for <
TEST(Assignment4, inputRedirectTest1) {
    Parser inputRedParser;
    std::vector<std::string> inputRedVec;

    inputRedVec.push_back("sort");
    inputRedVec.push_back("<");
    inputRedVec.push_back("OUTPUT.TXT");

    inputRedParser.parse("sort < OUTPUT.TXT");

    EXPECT_EQ(inputRedVec, inputRedParser.getData());
}//inputRedirectTest1

//testing execute for <
TEST(Assignment4, inputRedirectTest2) {
    std::vector<std::string> lefty;
    std::vector<std::string> righty;

    lefty.push_back("sort");
    righty.push_back("OUTPUT.TXT");

    Cmd* arg1 = new Cmd(lefty);
    Cmd* arg2 = new Cmd(righty);

    inputRed* inRtest = new inputRed(arg1, arg2);
    EXPECT_EQ(true, inRtest->execute(0, 1));
}//inputRedirectTest2

//testing parse for >
TEST(Assignment4, outputRedirectTest1) {
    Parser outputRedParser;
    std::vector<std::string> outputRedVec;

    outputRedVec.push_back("echo");
    outputRedVec.push_back("yeeeeeeeeet");
    outputRedVec.push_back(">");
    outputRedVec.push_back("NEWFILE5.TXT");

    outputRedParser.parse("echo yeeeeeeeeet > NEWFILE5.TXT");
    EXPECT_EQ(outputRedVec, outputRedParser.getData());
}//outputRedirectTest1

//testing execute for >
TEST(Assignment4, outputRedirectTest2) {
    std::vector<std::string> lefty;
    std::vector<std::string> righty;

    lefty.push_back("echo");

    //replace this with anything and it will overwrite NEWFILE4.TXT
    lefty.push_back("omggggg"); 

    righty.push_back("NEWFILE4.TXT");

    Cmd* arg1 = new Cmd(lefty);
    Cmd* arg2 = new Cmd(righty);
    outputRed* outRtest = new outputRed(arg1, arg2);

    EXPECT_EQ(true, outRtest->execute(0, 1));
}//outputRedirectTest2

//testing parsing for >>
TEST(Assignment4, AppendTest1) {
    Parser appendParser;
    std::vector<std::string> appendVec;

    appendVec.push_back("ls");
    appendVec.push_back("-a");
    appendVec.push_back(">>");
    appendVec.push_back("newfile1.txt");

    appendParser.parse("ls -a >> newfile1.txt");

    EXPECT_EQ(appendVec, appendParser.getData());
}//AppendTest1

//testing execute for >>
TEST(Assignment4, AppendTest2) {
    std::vector<std::string> lefty;
    std::vector<std::string> righty;

    //so we know for sure the file is being opened/created
    std::vector<std::string> eTestVec; 

    lefty.push_back("echo");
    lefty.push_back("bruhhhhhhhhhhhhhhhh");
    righty.push_back("OUTPUT.TXT");

    eTestVec.push_back("test");
    eTestVec.push_back("-e");
    eTestVec.push_back("OUTPUT.TXT");

    Cmd* arg1 = new Cmd(lefty);
    Cmd* arg2 = new Cmd(righty);
    Append* appsTest = new Append(arg1, arg2);
    Cmd* eTest = new TestCmd(eTestVec);

    EXPECT_EQ(true, appsTest->execute(0, 1));
    EXPECT_EQ(true, eTest->execute(0, 0));
}//AppendTest2

//testing parsing for single pipe
TEST(Assignment4, PipeTest1) {
    Parser pipeParser;
    std::vector<std::string> pipeVec;

    pipeVec.push_back("echo");
    pipeVec.push_back("yeet");
    pipeVec.push_back("|");
    pipeVec.push_back("tr");
    pipeVec.push_back("-d");
    pipeVec.push_back("'e'");

    pipeParser.parse("echo yeet | tr -d 'e'");

    EXPECT_EQ(pipeVec, pipeParser.getData());
}//PipeTest1

//testing execute for single pipe
TEST(Assignment4, PipeTest2) {
    std::vector<std::string> lefty;
    std::vector<std::string> righty;

    lefty.push_back("echo");
    lefty.push_back("yeet");
    righty.push_back("tr");
    righty.push_back("-d");
    righty.push_back("'e'");

    Cmd* arg1 = new Cmd(lefty);
    Cmd* arg2 = new Cmd(righty);
    sPipe* spipeTest = new sPipe(arg1, arg2);

    EXPECT_EQ(true, spipeTest->execute(0, 1));
}//PipeTest2

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}