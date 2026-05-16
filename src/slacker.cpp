#include <cstdio>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <unistd.h>

using namespace std;

const map<string, string>& get_commands() {
  static const map<string, string> commands = {
      {"cat",    "Concatenate and print files"       },
      {"cp",     "Copy files and directories"         },
      {"date",   "Display or set system date/time"    },
      {"echo",   "Display a line of text"             },
      {"find",   "Search for files in a directory"    },
      {"grep",   "Print lines matching a pattern"     },
      {"head",   "Output the first part of files"     },
      {"less",   "View file contents page by page"    },
      {"ln",     "Create links between files"         },
      {"ls",     "List directory contents"            },
      {"mkdir",  "Create directories"                 },
      {"mv",     "Move/rename files and directories"  },
      {"pwd",    "Print working directory"            },
      {"rm",     "Remove files or directories"        },
      {"rmdir",  "Remove empty directories"           },
      {"tail",   "Output the last part of files"      },
      {"touch",  "Change file timestamps"             },
      {"wc",     "Count lines, words, and bytes"      },
      {"which",  "Locate a command"                   },
      {"whoami", "Print current user name"            },
  };
  return commands;
}

enum class DispatchResult { Ok, ShowUsage, Unknown };

string get_basename(const char* path) {
  string s = path;
  auto pos = s.find_last_of('/');
  return pos == string::npos ? s : s.substr(pos + 1);
}

DispatchResult validate_command(const string& cmd,
                                const map<string, string>& commands) {
  if (commands.find(cmd) == commands.end())
    return DispatchResult::Unknown;
  return DispatchResult::Ok;
}

void usage(ostream& os, const map<string, string>& commands) {
  os << "Usage: slacker <command> [args...]\n\n";
  os << "Commands:\n";
  for (const auto& [name, desc] : commands) {
    os << "  " << left << setw(8) << name << " " << desc << "\n";
  }
}

DispatchResult dispatch(int argc, char** argv,
                        const map<string, string>& commands) {
  if (argc < 2)
    return DispatchResult::ShowUsage;

  string cmd = argv[1];
  if (commands.find(cmd) == commands.end())
    return DispatchResult::Unknown;

  return DispatchResult::Ok;
}

#ifndef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
int main(int argc, char** argv) {
  auto& cmds = get_commands();
  string name = get_basename(argv[0]);

  if (name != "slacker") {
    auto rc = validate_command(name, cmds);
    if (rc == DispatchResult::Unknown) {
      cerr << "Unknown command: " << name << "\n\n";
      usage(cout, cmds);
      return 1;
    }
    execvp(name.c_str(), argv);
    perror("execvp failed");
    return 1;
  }

  auto rc = dispatch(argc, argv, cmds);

  if (rc == DispatchResult::Unknown) {
    cerr << "Unknown command: " << argv[1] << "\n\n";
    usage(cout, cmds);
    return 1;
  }
  if (rc == DispatchResult::ShowUsage) {
    usage(cout, cmds);
    return 0;
  }

  execvp(argv[1], argv + 1);
  perror("execvp failed");
  return 1;
}
#endif
