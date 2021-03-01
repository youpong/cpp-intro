#include "main.h"

// support std::scope_exit()
// #define SCOPE_EXIT

template <typename T, typename Allocator = std::allocator<T>>
class vector {
public:
  // clang-format off
  using value_type      = T;
  using pointer         = T *;
  using const_pointer   = const pointer;
  using reference       = value_type &;
  using const_reference = const value_type &;
  using allocator_type  = Allocator;
  using size_type       = std::size_t;
  using difference_type = std::ptrdiff_t;

  using iterator               = pointer;
  using const_iterator         = const_pointer;
  using reverse_iterator       = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  // clang-format on
private:
  using traits = std::allocator_traits<allocator_type>;

  pointer first = nullptr;
  pointer last = nullptr;
  pointer reserved_last = nullptr;
  allocator_type alloc;

  //
  // allocate/deallocate
  //

  /**
   * @brief allocates n elements and returns a pointer to the allocated
   * memory.
   */
  pointer allocate(size_type n) { return traits::allocate(alloc, n); }

  /**
   * free pointer first.
   */
  void deallocate() { traits::deallocate(alloc, first, capacity()); }

  //
  // construct/destroy
  //

  void construct(pointer ptr) { traits::construct(alloc, ptr); }
  void construct(pointer ptr, const_reference value) {
    traits::construct(alloc, ptr, value);
  }
  void construct(pointer ptr, value_type &&value) {
    traits::construct(alloc, ptr, std::move(value));
  }
  void destroy(pointer ptr) { traits::destroy(alloc, ptr); }
  void destroy_until(reverse_iterator rend) {
    for (auto riter = rbegin(); riter != rend; ++riter, --last) {
      destroy(&*riter);
    }
  }
  void clear() noexcept { destroy_until(rend()); }

public:
  //
  // constructors ...
  //

  //  vector(std::size_t n = 0, Allocator a = Allocator()); original
  vector(size_type size = 0,
         const allocator_type &alloc = allocator_type())
      : alloc(alloc) {
    resize(size);
  }
  vector(size_type size, const_reference value,
         const allocator_type &alloc = allocator_type())
      : alloc(alloc) {
    resize(size, value);
  }

  template <typename InputIterator>
  vector(InputIterator first, InputIterator last,
         const allocator_type &alloc = allocator_type())
      : alloc(alloc) {
    reserve(std::distance(first, last));
    for (auto i = first; i != last; ++i) {
      push_back(*i);
    }
  }

  vector(std::initializer_list<value_type> init,
         const allocator_type &alloc = allocator_type())
      : vector(std::begin(init), std::end(init), alloc) {}

  vector(const vector &x);

  ~vector() {
    clear();
    deallocate();
  }

  void reserve(size_type sz) {
    if (sz <= capacity())
      return;

    auto ptr = allocate(sz);

    auto old_first = first;
    auto old_last = last;
    auto old_capacity = capacity();

    first = ptr;
    last = first;
    reserved_last = first + sz;

#ifdef SCOPE_EXIT
    std::scope_exit e(
        [&] { traits::deallocate(alloc, old_first, old_capacity); });
#endif

    for (auto old_iter = old_first; old_iter != old_last;
         ++old_iter, ++last)
      construct(last, std::move(*old_iter));

    for (auto riter = reverse_iterator(old_last);
         riter != reverse_iterator(old_first); ++riter)
      destroy(&*riter);

#ifndef SCOPE_EXIT
    traits::deallocate(alloc, old_first, old_capacity);
#endif
  }

  void resize(size_type sz) {
    if (sz < size()) {
      auto diff = size() - sz;
      destroy_until(rbegin() + diff);
      last = first + sz;
    } else if (sz > size()) {
      reserve(sz);
      for (; last != reserved_last; ++last) {
        construct(last);
      }
    }
  }

  void resize(size_type sz, const_reference value) {
    if (sz < size()) {
      auto diff = size() - sz;
      destroy_until(rbegin() + diff);
      last = first + sz;
    } else if (sz > size()) {
      reserve(sz);
      for (; last != reserved_last; ++last) {
        construct(last, value);
      }
    }
  }

  void shrink_to_fit() {
    if (size() == capacity())
      return;

    auto current_size = size();
    auto ptr = allocate(current_size);

    for (auto raw_ptr = ptr, iter = begin(); iter != end();
         ++iter, ++raw_ptr) {
      construct(raw_ptr, *iter);
    }

    clear();
    deallocate();

    first = ptr;
    last = ptr + current_size;
    reserved_last = last;
  }

  // TODO
  vector &operator=(const vector &x);

  void push_back(const_reference value) {
    if (size() + 1 > capacity()) {
      auto c = size();
      if (c == 0)
        c = 1;
      else
        c *= 2;

      reserve(c);
    }

    construct(last, value);
    ++last;
  }

  reference operator[](size_type i) { return first[i]; }
  const_reference operator[](size_type i) const { return first[i]; }
  reference at(size_type i) {
    if (i >= size())
      throw std::out_of_range("index is out of range");
    return first[i];
  }
  const_reference at(size_type i) const {
    if (i >= size())
      throw std::out_of_range("index is out of range");
    return first[i];
  }

  // clang-format off
  reference        front()                 { return *first; }
  const_reference  front()  const          { return *first; }  
  reference        back()                  { return *(last - 1); }
  const_reference  back()   const          { return *(last - 1); }

  pointer          data()         noexcept { return first; }
  const_pointer    data()   const noexcept { return first; }

  iterator         begin()        noexcept { return first; }
  iterator         end()          noexcept { return last;  }
  iterator         begin()  const noexcept { return first; }  
  iterator         end()    const noexcept { return last;  }
  
  const_iterator   cbegin() const noexcept { return first; }
  const_iterator   cend()   const noexcept { return last;  }
  reverse_iterator rbegin()       noexcept { return reverse_iterator{last};  }
  reverse_iterator rend()         noexcept { return reverse_iterator{first}; }
  const_reverse_iterator crbegin() const noexcept 
                                           { return reverse_iterator{last};  }
  const_reverse_iterator crend()   const noexcept
                                           { return reverse_iterator{first}; }
  // clang-format on

  /**
   * @return number of elements
   */
  size_type size() const noexcept {
    //    return end() - begin();
    return std::distance(begin(), end());
  }

  size_type empty() const noexcept {
    //    return begin() == end();
    return size() == 0;
  }

  size_type capacity() const noexcept { return reserved_last - first; }
};

template <typename Vector>
static void test_vector() {
  Vector v(100);
  for (auto i = 0; i != 100; ++i)
    v[i] = i;

  expect(__LINE__, 0, v[0]);
  expect(__LINE__, 99, v[99]);
  return;
  std::for_each(std::begin(v), std::end(v),
                [](auto x) { std::cout << x; });

  std::copy(std::istream_iterator<int>(std::cin),
            std::istream_iterator<int>(), std::back_inserter(v));
}

static void test_nested_typename() {
  using vec = std::vector<int>;
  // using vec = vector<int>;
  vec v = {0, 1, 2};

  vec::value_type val = v[1];
  expect(__LINE__, 1, val);
  vec::iterator i = v.begin();
  expect(__LINE__, 0, *i);
  vec::size_type s = v.size();
  expect(__LINE__, 3, s);
  auto j = i + 3;
  vec::difference_type d = j - i;
  expect(__LINE__, 3, d);
}

/*
static void test_foo() {
  //vector<int> vec = { 0, 1, 2 };
  vector<int>::size_type s = 1;
}
*/

static void test_foo() {
  std::vector<int> v(1);
  //  vector<int> v(1);

  // std::vector<int>::iterator
  auto i = v.begin();
  *i = 0;

  auto const &cv = v;
  // std::vector<int>::const_iterator
  [[maybe_unused]] auto ci = cv.begin();
  // error
  // can't assign pointed by const_iterator
  //  *ci = 0;
}

static void test_vector2() {
  vector<int> v;
  expect(__LINE__, 0, v.capacity());
  expect(__LINE__, 0, v.size());

  v.push_back(0);
  expect(__LINE__, 0, v[0]);
  v.push_back(1);
  expect(__LINE__, 1, v[1]);
  expect(__LINE__, 2, v.size());
}

// case: sz <= capacity()
static void test_reserve_le() {
  vector<int> v;
  v.reserve(0);
}

// case: sz > capacity()
static void test_reserve_gt() {
  vector<int> v;
  v.reserve(1);
}

template <typename Vector>
static void test_reserve() {
  Vector v;

  v.reserve(5);
  expect(__LINE__, true, v.capacity() >= 5);

  // no reallocation
  auto old_first = v.data();
  for (int i = 0; i < v.capacity(); ++i)
    v.push_back(i);
  expect(__LINE__, true, old_first == v.data());

  // reallocation
  v.push_back(346);
  expect(__LINE__, false, old_first == v.data());
}

template <typename Vector>
static void test_shrink_to_fit() {
  Vector v;
  v.shrink_to_fit();
}

static void test_vector() {
  vector<int> v;
  expect(__LINE__, 0, v.size());
  expect(__LINE__, true, v.capacity() >= 0);
}

// case: sz < size()
static void test_resize_lt() {
  //  size == 1
  vector<int> v = {0};

  v.resize(0);
  expect(__LINE__, 0, v.size());
}

// case: sz > size()
static void test_resize_gt() {
  // size == 1
  vector<int> v = {0};

  v.resize(2);
  expect(__LINE__, 2, v.capacity());

  v[1] = 65;
  expect(__LINE__, 65, v[1]);
}

// case: sz == size()
static void test_resize_eq() {
  // size == 1
  vector<int> v = {0};
  v.resize(1);
}

static void test_front_back() {
  vector<int> v = {0, 1, 2, 3, 4};
  expect(__LINE__, 1, ++v.front());
  expect(__LINE__, 5, ++v.back());
}

static void test_const_front_back() {
  const vector<int> v = {0, 1, 2, 3, 4};
  expect(__LINE__, 0, v.front());
  expect(__LINE__, 4, v.back());
}

static void test_data() {
  vector<int> v = {0, 1, 2, 3, 4};
  expect(__LINE__, 0, *(v.data()));
}

static void test_const_data() {
  const vector<int> v = {0, 1, 2, 3, 4};
  expect(__LINE__, 0, *(v.data()));
}

static void test_begin_end() {
  vector<int> v = {0, 1, 2, 3, 4};

  auto total = 0;
  for (auto i = v.begin(); i != v.end(); ++i) {
    total += ++*i;
  }
  expect(__LINE__, 15, total);
}

static void test_begin_end_const() {
  const vector<int> v = {0, 1, 2, 3, 4};

  auto total = 0;
  for (auto i = v.begin(); i != v.end(); ++i) {
    total += *i;
  }
  expect(__LINE__, 10, total);
}

static void test_cbegin_cend() {
  vector<int> v = {0, 1, 2, 3, 4};

  auto total = 0;
  for (auto i = v.cbegin(); i != v.cend(); ++i) {
    total += *i;
  }
  expect(__LINE__, 10, total);
}

static void test_rbegin_rend() {
  vector<int> org = {0, 1, 2, 3, 4};
  vector<int> tmp;
  vector<int> trgt = {8, 6, 4, 2, 0};

  for (auto ri = org.rbegin(); ri != org.rend(); ++ri) {
    *ri *= 2;
    tmp.push_back(*ri);
  }

  expect(__LINE__, true,
         std::equal(std::begin(tmp), std::end(tmp), std::begin(trgt),
                    std::end(trgt)));
}

template <typename Vector>
static void test_crbegin_crend() {
  Vector v1 = {0, 2, 4, 6, 8};
  Vector v2 = {8, 6, 4, 2, 0};

  auto i2 = v2.begin();
  auto i1 = v1.crbegin();
  for (; i1 != v1.crend(); ++i1, ++i2) {
    expect(__LINE__, true, *i1 == *i2);

    // error: cannot assign to return value because function 'operator*'
    // returns a const value
    //    ++*i1;

    // std::cout << *i1 << "\n"s;
  }
}

static void test_size() {
  vector<int> v;
  expect(__LINE__, 0, v.size());
}

static void test_empty() {
  vector<int> v;
  expect(__LINE__, true, v.empty());
}

static void test_capacity() {
  vector<int> v;
  expect(__LINE__, 0, v.capacity());
}

static void test_index() {
  vector<int> v = {0, 1, 2, 3, 4};
  expect(__LINE__, 0, v[0]++);
  expect(__LINE__, 1, v.at(0)++);
  expect(__LINE__, 2, v.front());
}

static void test_index_const() {
  const vector<int> v = {0, 1, 2, 3, 4};
  expect(__LINE__, 0, v[0]);
  expect(__LINE__, 1, v.at(1));
}

template <typename Vector>
static void test_push_back() {
  Vector v;

  v.resize(0);
  expect(__LINE__, 0, v.capacity());
  v.push_back(78);

  v.shrink_to_fit();
  expect(__LINE__, 1, v.capacity());
  v.push_back(89);

  expect(__LINE__, 78, v[0]);
  expect(__LINE__, 89, v[1]);
}

void test_all_vector() {

  test_vector();
  test_index();
  test_index_const();

  test_resize_lt();
  test_resize_gt();
  test_resize_eq();

  test_reserve<vector<int>>();
  test_reserve<std::vector<int>>();
  test_reserve_le();
  test_reserve_gt();

  test_push_back<std::vector<int>>();
  test_push_back<vector<int>>();
  test_front_back();
  test_const_front_back();
  test_data();
  test_const_data();
  test_begin_end();
  test_begin_end_const();
  test_cbegin_cend();
  test_rbegin_rend();
#ifdef ISSUE_5
  test_crbegin_crend<vector<int>>();
#endif
  test_crbegin_crend<std::vector<int>>();
  test_size();
  test_empty();
  test_capacity();
  test_shrink_to_fit<vector<int>>();

  test_vector2();
  test_size();
  test_vector<std::vector<int>>();
  test_nested_typename();
  test_foo();
}

/*
template <typename T, typename Allocator = std::allocator<T>>
class S {
  using allocator_type = Allocator;
  using traits = std::allocator_traits<allocator_type>;
  allocator_type alloc;

  //  template < typename Allocator
  void f() {
    traits::allocate(alloc, 1);
  }
};
*/
