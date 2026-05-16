CXX = clang++
CXXFLAGS = -std=c++17 -Wall

.PHONY: all test clean run

all: out/slacker

out/slacker: src/slacker.cpp | out
	$(CXX) $(CXXFLAGS) -o out/slacker src/slacker.cpp

out/slacker_test: test/slacker_test.cpp src/slacker.cpp test/doctest.h | out
	$(CXX) $(CXXFLAGS) -Itest -Isrc -o out/slacker_test test/slacker_test.cpp

test: out/slacker_test
	./out/slacker_test

run: out/slacker
	./out/slacker

out:
	mkdir -p out

clean:
	rm -rf out
