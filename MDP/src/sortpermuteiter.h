#ifndef SORTPERMUTEITER_H
#define SORTPERMUTEITER_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>

using namespace std;

using namespace std;

template <typename T1, typename T2>
void sortByPerm(vector<T1>& list1, vector<T2>& list2) {
  const auto len = list1.size();
  if (!len || len != list2.size()) throw;

  // create permutation vector
  vector<size_t> perms;
  for (size_t i = 0; i < len; i++) perms.push_back(i);
  sort
    ( perms.begin(), perms.end()
    , [&](size_t a, size_t b) {
         return list1[a].size() < list1[b].size();
      }
    );

  // order input vectors by permutation
  for (size_t i = 0; i < len - 1; i++) {
    swap(list1[i], list1[perms[i]]);
    swap(list2[i], list2[perms[i]]);

    // adjust permutation vector if required
    if (i < perms[i]) {
      auto d = distance(perms.begin(), find(perms.begin() + i, perms.end(), i));
      swap(perms[i], perms[d]);
    }
  }
}

#endif // SORTPERMUTEITER_H
