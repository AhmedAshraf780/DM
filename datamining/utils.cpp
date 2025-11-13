#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

// Function to split a string by a delimiter
std::vector<std::string> split(const std::string &s, char delimiter) {
  std::vector<std::string> tokens;
  std::stringstream ss(s);
  std::string item;

  while (std::getline(ss, item, delimiter)) {
    // remove whitespace if any
    item.erase(remove_if(item.begin(), item.end(), ::isspace), item.end());
    tokens.push_back(item);
  }
  return tokens;
}

// Function to read CSV and return vector of transactions
std::vector<std::vector<std::string>> readCSV(const std::string &filename) {
  std::ifstream file(filename);
  std::vector<std::vector<std::string>> transactions;
  std::string line;

  if (!file.is_open()) {
    std::cerr << "Error: Cannot open file " << filename << "\n";
    return transactions;
  }

  // Skip the header
  std::getline(file, line);

  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string tid, items;

    std::getline(ss, tid, ',');
    std::getline(ss, items);

    // remove whitespace and carriage returns
    items.erase(
        std::remove_if(items.begin(), items.end(),
                       [](unsigned char c) { return c == '\r' || c == '\n'; }),
        items.end());

    // remove quotes if they exist
    if (!items.empty() && items.front() == '"')
      items.erase(items.begin());
    if (!items.empty() && items.back() == '"')
      items.pop_back();

    std::vector<std::string> transaction = split(items, ',');

    transactions.push_back(transaction);
  }

  return transactions;
}

// Function to print the transactions
void printTransactions(
    const std::vector<std::vector<std::string>> &transactions) {
  for (const auto &transaction : transactions) {
    for (const auto &item : transaction) {
      std::cout << item;
    }
    std::cout << "\n";
  }
}

std::map<std::string, int>
countItems(const std::vector<std::vector<std::string>> &transactions) {
  std::map<std::string, int> itemCounts;
  for (const auto &transaction : transactions) {
    for (const auto &item : transaction) {
      itemCounts[item]++;
    }
  }
  return itemCounts;
}

std::map<std::string, int>
filterItems(const std::map<std::string, int> itemsMap, int support) {
  std::map<std::string, int> filteredItems;
  for (auto item : itemsMap) {
    if (item.second >= support) {
      filteredItems[item.first] = item.second;
    }
  }
  return filteredItems;
}

std::vector<std::pair<std::string, int>>
sortItems(const std::map<std::string, int> &itemsMap) {
  std::vector<std::pair<std::string, int>> items(itemsMap.begin(),
                                                 itemsMap.end());

  std::sort(items.begin(), items.end(), [](const auto &a, const auto &b) {
    if (a.second == b.second)
      return a.first < b.first; // sort by ASCII (key) if values equal
    return a.second > b.second; // sort by value (descending)
  });

  return items;
}

void removeUnsupportedItems(std::vector<std::vector<std::string>> &transactions,
                            std::map<std::string, int> &itemCounts,
                            int minSupport) {
  for (auto &transaction : transactions) {
    // Use erase-remove idiom directly on the transaction
    transaction.erase(std::remove_if(transaction.begin(), transaction.end(),
                                     [&](const std::string &item) {
                                       return itemCounts[item] < minSupport;
                                     }),
                      transaction.end());
  }
}
// Reconstruct a transaction by sorting its items based on sortedMap order
std::vector<std::string> reconstructTransaction(
    const std::vector<std::string> &transaction,
    const std::vector<std::pair<std::string, int>> &sortedMap) {

  std::vector<std::string> sortedTransaction;

  // Create a lookup map for quick access to order index
  std::map<std::string, int> orderIndex;
  for (size_t i = 0; i < sortedMap.size(); ++i) {
    orderIndex[sortedMap[i].first] = static_cast<int>(i);
  }

  // Copy transaction and sort it using the order index
  sortedTransaction = transaction;
  std::sort(sortedTransaction.begin(), sortedTransaction.end(),
            [&](const std::string &a, const std::string &b) {
              return orderIndex[a] < orderIndex[b];
            });

  return sortedTransaction;
}
// Remove duplicate items from each transaction (keep first occurrence)
void removeDuplicates(std::vector<std::vector<std::string>> &transactions) {
  for (auto &transaction : transactions) {
    std::vector<std::string> unique;
    std::map<std::string, bool> seen;

    for (const auto &item : transaction) {
      if (!seen[item]) {
        unique.push_back(item);
        seen[item] = true;
      }
    }

    transaction = unique;
  }
}

std::vector<std::string>
bottomUpFrequents(std::vector<std::pair<std::string, int>> sortedItems) {
  std::vector<std::string> bottomUpFrequents;
  for (int i = 0; i < sortedItems.size(); i++) {
    if (i == 0)
      continue;
    bottomUpFrequents.push_back(sortedItems[i].first);
  }
  std::reverse(bottomUpFrequents.begin(), bottomUpFrequents.end());
  return bottomUpFrequents;
}
