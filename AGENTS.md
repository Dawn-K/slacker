# AGENTS.md

## Build & test commands

```bash
make              # build out/slacker
make test         # build out/slacker_test and run it
make run          # build and run out/slacker (shows usage)
make clean        # remove out/
```

## Project structure

```
src/slacker.cpp         — dispatcher: whitelist, usage(), dispatch(), main()
test/slacker_test.cpp   — unit tests, #includes slacker.cpp directly
test/doctest.h      — vendored test framework (doctest 2.5)
out/                — build artifacts (gitignored)
```

## Code style

- C++17, clang++, `-Wall`
- `using namespace std` is allowed (slacker project, not a library)
- `dispatch()` returns `enum class DispatchResult { Ok, ShowUsage, Unknown }`
- `usage()` writes to `ostream&` so tests can capture output via `ostringstream`

## Architecture

The dispatcher supports two invocation modes, distinguished by `argv[0]` basename:

**Subcommand mode** (`argv[0]` basename is `slacker`):
1. `get_commands()` — static whitelist of command-name → description
2. `dispatch(argc, argv, commands)` — validates the subcommand, returns `DispatchResult`
3. `main()` — calls dispatch(), prints usage/error, or `execvp(argv[1], argv+1)`

**Symlink mode** (`argv[0]` basename is anything else, e.g. `ls`):
1. `get_basename(argv[0])` — extracts the command name
2. `validate_command(name, commands)` — checks whitelist
3. `main()` — calls `execvp(name, argv)`, passing through all arguments

Both modes share the same whitelist. Create symlinks with `ln -s slacker ls`, then `./ls -la`.

## Testing

- Framework: doctest (header-only, vendored in `test/`)
- Tests in `test/slacker_test.cpp` get a standalone `main()` via `DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN`
- The real `main()` in `src/slacker.cpp` is guarded with `#ifndef DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN`
- Test binary is compiled with `-Itest -Isrc` so `#include "doctest.h"` and `#include "slacker.cpp"` resolve
- `dispatch()` is the main test target — pure logic, no I/O, no execvp
- `usage()` is tested by passing `ostringstream` instead of `cout`

## Adding a command

1. Add an entry to the whitelist in `get_commands()`
2. Add a `CHECK(cmds.find("newcmd") != cmds.end())` in `slacker_test.cpp`
3. Run `make test` to confirm

## Rules

- **TDD must pass** — `make test` must succeed before claiming work is done
- **No malicious TDD passing** — never weaken or remove assertions to make tests pass; fix the code, not the test
- **Avoid hardcoding** — no magic numbers, no fixed-size buffers. If hardcoding is the pragmatic choice, get explicit consent first
- **Keep README in sync** — if a change alters user-visible behavior or usage, update README.md in the same change
