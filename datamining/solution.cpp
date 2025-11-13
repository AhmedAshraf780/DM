#include "actors.cpp"
#include "inventory.cpp"
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

vector<pair<vector<string>, int>> getPrefixPaths(const string &item,
                                                 HeaderTable &entry) {
  vector<pair<vector<string>, int>> paths;
  Node *node = entry.head;
  while (node) {
    int cnt = node->count;
    Node *parent = node->parent;
    vector<string> path;
    while (parent && parent->item != "") {
      path.push_back(parent->item);
      parent = parent->parent;
    }

    if (!path.empty()) {
      reverse(path.begin(), path.end());
      paths.emplace_back(path, cnt);
    }
    node = node->nextOne;
  }
  return paths;
}

void combinePaths(vector<pair<vector<string>, int>> &paths) {
  unordered_map<string, int> combined;

  // Serialize each path as a single string key
  for (auto &pc : paths) {
    string key;
    for (auto &it : pc.first)
      key += it + ","; // comma-delimited key
    combined[key] += pc.second;
  }

  // Rebuild the merged paths
  paths.clear();
  for (auto &[key, cnt] : combined) {
    vector<string> path;
    string temp;
    for (char c : key) {
      if (c == ',') {
        if (!temp.empty()) {
          path.push_back(temp);
          temp.clear();
        }
      } else {
        temp += c;
      }
    }
    paths.push_back({path, cnt});
  }
}
void filteringPaths(vector<pair<vector<string>, int>> &paths,
                    int minSupportCount) {
  unordered_map<string, int> freqItemsets;
  for (auto &pc : paths) {
    const vector<string> &path = pc.first;
    for (const string &it : path) {
      freqItemsets[it] += pc.second;
    }
  }
  for (auto &pc : paths) {
    auto &path = pc.first;
    path.erase(remove_if(path.begin(), path.end(),
                         [&](const string &it) {
                           return freqItemsets[it] < minSupportCount;
                         }),
               path.end());
  }
  combinePaths(paths);
}

FPTree generateConditionalPatternBase(vector<pair<vector<string>, int>> paths) {
  return buildCT(paths);
}

void minning(FPTree &CT, vector<string> suffix) {
  unordered_map<string, HeaderTable> headerTable = CT.getHeaderTable();
  vector<pair<string, int>> freqItems;

  for (auto &kv : headerTable) {
    freqItems.push_back({kv.first, kv.second.frequency});
  }
  sort(freqItems.begin(), freqItems.end(), [](auto &a, auto &b) {
    if (a.second == b.second)
      return a.first < b.first;
    return a.second > b.second;
  });
  for (auto &[item, cnt] : freqItems) {

    vector<string> pattern = suffix;
    pattern.insert(pattern.begin(), item);
    inventory.push_back({pattern, cnt});
    vector<pair<vector<string>, int>> paths =
        getPrefixPaths(item, headerTable[item]);
    filteringPaths(paths, MIN_SUPPORT);
    FPTree newCT = generateConditionalPatternBase(paths);
    minning(newCT, pattern);
  }
}
/*
 * 1. Loop on the freqBotomUp array # [D,E,B,C]
 * 2. generate the Conditional Pattern Base
 * 3. Filter the conditional pattern base
 * 4. mine the FP-tree
 * 5. do it again again again
 *
 *
 * */

void solve(FPTree &fpTree, int min_support, vector<string> freqBottomUp) {
  // 1. Loop on the freqBotomUp array # [D,E,B,C]
  for (int i = 0; i < freqBottomUp.size() - 1; i++) {
    // 2. generate the Conditional Pattern Base
    // 3. Filter the conditional pattern base
    // 4. mine the FP-tree
    // 5. do it again again again
    unordered_map<string, HeaderTable> headerTable = fpTree.getHeaderTable();
    vector<pair<vector<string>, int>> paths =
        getPrefixPaths(freqBottomUp[i], headerTable[freqBottomUp[i]]);
    filteringPaths(paths, min_support);
    FPTree CT = generateConditionalPatternBase(paths);
    minning(CT, {freqBottomUp[i]});
  }
}
