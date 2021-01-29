#include "main.h"

#ifdef FOO
namespace ns {
template <typename T>
struct bidirectional_link_list_node {
  T value;
  bidirectional_link_list_node *prev;
  bidirectional_link_list_node *next;

  bidirectional_link_list_node(T value,
                               bidirectional_link_list_node *prev,
                               bidirectional_link_list_node *next) {
    this->value = value;
    this->prev = prev;
    this->next = next;
  }
};

template <typename T>
struct bidirectional_link_list_iterator {
  using iterator = bidirectional_link_list_iterator;

  // --- boilerplate code
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  using reference = T &;
  using pointer = T *;
  using iterator_category = std::bidirectional_iterator_tag;
  // --- boilerplate code

  bidirectional_link_list<T> *list;
  bidirectional_link_list_node<T> *ptr;

  bidirectional_link_list_iterator(bidirectional_link_list<T> *list,
                                   bidirectional_link_list_node<T> *ptr)
      : list(list), ptr(ptr) {}

  T &operator*() noexcept { return ptr->value; }

  iterator &operator++() noexcept {
    ptr = ptr->next;
    return *this;
  }
  iterator operator++(int) noexcept {
    auto temp = *this;
    ++*this;
    return temp;
  }

  iterator &operator--() noexcept {
    ptr = ptr->prev;
    return *this;
  }
  iterator operator--(int) noexcept {
    auto temp = *this;
    --*this;
    return temp;
  }

  bool operator==(iterator const &r) const noexcept {
    return ptr == r.ptr;
  }
};

template <typename T>
struct bidirectional_link_list {
  bidirectional_link_list_node<T> *first;
  bidirectional_link_list_node<T> *last;
  bidirectional_link_list_iterator<T> begin() {
    bidirectional_link_list_iterator temp(this, first);
    return nullptr;
  }
  bidirectional_link_list_iterator<T> end() {
    return bidirectional_link_list_iterator(this, nullptr);
  }
};
} // namespace ns

#endif

/**
 * ns::bidirectional_iterator
 */
static void test_advance1_1() {

#ifdef FOO
  // clang-format off
  ns::bidirectional_link_list_node<int> node0( 0, nullptr, nullptr);
  ns::bidirectional_link_list_node<int> node1( 0, &node0, nullptr);
  ns::bidirectional_link_list_node<int> node2( 0, &node1, nullptr);
  ns::bidirectional_link_list_node<int> node3( 0, &node2, nullptr);
  ns::bidirectional_link_list_node<int> node4( 0, &node3, nullptr);
  // clang-format on
  node0.next = &node1;
  node1.next = &node2;
  node2.next = &node3;
  node3.next = &node4;

  ns::bidirectional_link_list<int> list{&node0, &node4};
  //  bidirectional_link_list_iterator begin(&list0), end(&end_l);
  auto begin = list.begin();
  auto end = list.end();

  expect(__LINE__, 1, *std::next(begin));
  expect(__LINE__, 3, *std::next(begin, 3));
  expect(__LINE__, 4, *std::prev(end));
  expect(__LINE__, 2, *std::prev(end, 3));

/*
expect(__LINE__, 0, *iter);
std::advance(iter, 1);
expect(__LINE__, 1, *iter);
std::advance(iter, 3);
expect(__LINE__, 4, *iter);

std::advance(iter, -2);
expect(__LINE__, 2, *iter);
std::advance(iter, 0);
expect(__LINE__, 2, *iter);

expect(__LINE__, 3, std::distance(iter, last));
*/
#endif
}

void test_all_list() { test_advance1_1(); }
