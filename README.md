# slacker

A busybox-style command dispatcher that forwards to system commands. Give it a command name and arguments, it runs the real thing.

## Quick start

```bash
make          # build
./out/slacker     # show usage
./out/slacker ls -la
./out/slacker echo hello world
./out/slacker which clang++
```

## Symlink mode

Create a symlink named after any whitelisted command, and slacker becomes that command:

```bash
cd out
ln -s slacker ls
./ls              # same as system ls
./ls --version    # passthrough to real ls

mv ./ls whoami
./whoami          # prints your username

mv ./whoami cat
./cat ../README.md # same as system cat
```

> **Warning:** If you put a slacker symlink (e.g. `ls`) in a directory that appears
> before `/usr/bin` in your `PATH`, running `ls` will cause an infinite `execvp`
> loop — slacker keeps re-executing itself. Keep symlinks outside your `PATH` or
> use `./ls` (explicit path) to avoid this.

## Testing

```bash
make test     # build & run all tests
```

## Layout

```
slacker/
├── Makefile
├── src/
│   └── slacker.cpp          # dispatcher
├── test/
│   ├── doctest.h        # test framework (vendored)
│   └── slacker_test.cpp     # unit tests
└── out/                 # build artifacts (gitignored)
    ├── slacker
    └── slacker_test
```
