// define to reproduce issues
//#define ISSUE_1
//#define ISSUE_2
//#define ISSUE_3

// ARRAY_ITERATOR:
//   1: iterator class with index
//   2: iterator class with pointer
//   3: pointer as iterator
#define ARRAY_ITERATOR 3

// Result                 F F S S S S
//------------------------------------
// ARRAY_ITERATOR 1       X     X
//                2         X     X
//                3           X     X
// compiler       clang++ X X X
//                g++           X X X
//

auto expect = [](auto line, auto expected, auto actual) {
  if (expected == actual)
    return;

  std::cerr << line << ": "s << expected << " expected, but got "s
            << actual << "\n";
  exit(1);
};

extern bool quiet_mode;
