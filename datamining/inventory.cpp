#include <iostream>
#include <vector>
using namespace std;

std::vector<std::vector<std::string>> newTransactions;
std::vector<std::pair<std::string, int>> sortedItems;

std::vector<pair<vector<string>, int>> inventory;
std::vector<pair<string, int>> L1;
std::vector<pair<vector<string>, int>> L2;
std::vector<pair<vector<string>, int>> L3;
int MIN_SUPPORT = 3;

void generateL1() {
  for (auto it : sortedItems) {
    L1.push_back({it.first, it.second});
  }
}
void generateL2() {
  for (auto it : inventory) {
    if (it.first.size() == 2)
      L2.push_back({it.first, it.second});
  }
}
void generateL3() {
  for (auto it : inventory) {
    if (it.first.size() == 3)
      L3.push_back({it.first, it.second});
  }
}

void testLists() {
  generateL1();
  generateL2();
  generateL3();

  cout << "L1:\n";
  for (auto it : L1) {
    cout << it.first << " " << it.second << "\n";
  }
  cout << "======================================\n";
  cout << "L2:\n";
  for (auto it : L2) {
    for (auto it : it.first)
      cout << it << " ";
    cout << it.second << "\n";
  }
  cout << "======================================\n";
  cout << "L3:\n";
  for (auto it : L3) {
    for (auto it : it.first)
      cout << it << " ";
    cout << it.second << "\n";
  }
}
