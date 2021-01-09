#include <cassert>
#include <cerrno>
#include <cfloat>
#include <climits>
#if __has_include(<cstdalign>)
#include <cstdalign>
#endif
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <initializer_list>
#include <limits>
#include <new>
#include <stdexcept>
#include <string>
#include <system_error>
#include <typeinfo>

#if __has_include(<string_view>)
#include <string_view>
#endif

#include <algorithm>
#include <array>
#include <cfenv>
#include <cmath>
#include <deque>
#include <forward_list>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <list>
#include <map>
#include <numeric>
#include <ostream>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <stack>
#include <streambuf>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#if __has_include(<filesystem>)
#include <filesystem>
#endif

#include <cinttypes>
#include <cstdio>

#include <atomic>
#include <condition_variable>
#include <future>
#include <mutex>
#include <regex>
#include <shared_mutex>
#include <thread>

using namespace std::literals;

auto expect = [](auto line, auto expected, auto actual) {
  if (expected == actual)
    return;

  std::cerr << line << ": "s << expected << " expected, but got "s
            << actual << "\n";
  exit(1);
};

// ARRAY_ITERATOR:
//   1: iterator class with index
//   2: iterator class with pointer
//   3: pointer as iterator
#define ARRAY_ITERATOR 3

// Result                 S S S S S S
//------------------------------------
// ARRAY_ITERATOR 1       X     X
//                2         X     X
//                3           X     X
// compiler       clang++ X X X
//                g++           X X X
//
