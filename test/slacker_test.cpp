#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <sstream>
#include "slacker.cpp"

TEST_CASE("get_commands returns all whitelisted commands") {
  auto& cmds = get_commands();
  CHECK(cmds.size() == 20);
  CHECK(cmds.find("ls") != cmds.end());
  CHECK(cmds.find("cp") != cmds.end());
  CHECK(cmds.find("mv") != cmds.end());
  CHECK(cmds.find("cat") != cmds.end());
  CHECK(cmds.find("rm") != cmds.end());
  CHECK(cmds.find("mkdir") != cmds.end());
  CHECK(cmds.find("rmdir") != cmds.end());
  CHECK(cmds.find("touch") != cmds.end());
  CHECK(cmds.find("ln") != cmds.end());
  CHECK(cmds.find("head") != cmds.end());
  CHECK(cmds.find("tail") != cmds.end());
  CHECK(cmds.find("wc") != cmds.end());
  CHECK(cmds.find("less") != cmds.end());
  CHECK(cmds.find("echo") != cmds.end());
  CHECK(cmds.find("date") != cmds.end());
  CHECK(cmds.find("whoami") != cmds.end());
  CHECK(cmds.find("pwd") != cmds.end());
  CHECK(cmds.find("grep") != cmds.end());
  CHECK(cmds.find("find") != cmds.end());
  CHECK(cmds.find("which") != cmds.end());
  CHECK(cmds.find("foobar") == cmds.end());
}

TEST_CASE("usage prints formatted command list") {
  ostringstream ss;
  usage(ss, get_commands());
  string out = ss.str();
  CHECK(out.find("Usage: slacker <command> [args...]") != string::npos);
  CHECK(out.find("ls") != string::npos);
  CHECK(out.find("List directory contents") != string::npos);
  CHECK(out.find("cp") != string::npos);
  CHECK(out.find("Copy files and directories") != string::npos);
}

TEST_CASE("dispatch with no args returns ShowUsage") {
  const char* av[] = {"slacker"};
  CHECK(dispatch(1, (char**)av, get_commands()) == DispatchResult::ShowUsage);
}

TEST_CASE("dispatch with known command returns Ok") {
  const char* av[] = {"slacker", "ls"};
  CHECK(dispatch(2, (char**)av, get_commands()) == DispatchResult::Ok);
}

TEST_CASE("dispatch with known command with extra args returns Ok") {
  const char* av[] = {"slacker", "cp", "-r", "a", "b"};
  CHECK(dispatch(5, (char**)av, get_commands()) == DispatchResult::Ok);
}

TEST_CASE("get_basename extracts filename from path") {
  CHECK(get_basename("/usr/bin/ls") == "ls");
  CHECK(get_basename("ls") == "ls");
  CHECK(get_basename("/a/b/c/slacker") == "slacker");
  CHECK(get_basename("slacker") == "slacker");
}

TEST_CASE("validate_command Ok for known command") {
  CHECK(validate_command("ls", get_commands()) == DispatchResult::Ok);
}

TEST_CASE("validate_command Unknown for unknown command") {
  CHECK(validate_command("foobar", get_commands()) == DispatchResult::Unknown);
}

TEST_CASE("dispatch with unknown command returns Unknown") {
  const char* av[] = {"slacker", "foobar"};
  CHECK(dispatch(2, (char**)av, get_commands()) == DispatchResult::Unknown);
}
