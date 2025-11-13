#include "solution.cpp"
#include "utils.cpp"

int main() {
  std::vector<std::vector<std::string>> transactions =
      readCSV("Horizontal_Format.xlsx - Sheet1.csv");
  removeDuplicates(transactions);

  std::map<std::string, int> itemCounts = countItems(transactions);
  std::map<std::string, int> filteredItems = filterItems(itemCounts, 3);
  sortedItems = sortItems(filteredItems);

  removeUnsupportedItems(transactions, itemCounts, 3);

  for (const auto &t : transactions) {
    auto sortedT = reconstructTransaction(t, sortedItems);
    if (!sortedT.empty())
      newTransactions.push_back(sortedT);
  }
  FPTree fpTree;
  fpTree.insertTransactions(newTransactions);

  // vector<pair<string, int>> freqBottomUp = removeDominant(sortedItems);

  solve(fpTree, 3, bottomUpFrequents(sortedItems));

  cout << "The frequent paths:\n";
  for (auto item : inventory) {
    for (auto item : item.first) {
      cout << item << " ";
    }
    cout << item.second << "\n";
  }
  cout << "====================================\n";
  testLists();
  return 0;
}
