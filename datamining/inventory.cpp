#include <algorithm>
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
    if (it.first.size() == 1)
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
  for (auto &it : L1) {
    cout << it.first << " " << it.second << "\n";
  }
  cout << "======================================\n";
  cout << "L2:\n";
  for (auto &it : L2) {
    for (auto &it : it.first)
      cout << it << " ";
    cout << it.second << "\n";
  }
  cout << "======================================\n";
  cout << "L3:\n";
  for (auto &it : L3) {
    for (auto &it : it.first)
      cout << it << " ";
    cout << it.second << "\n";
  }
}
double support(const vector<string> &itemset) {
  int count = 0;
  for (auto &transaction : newTransactions) {
    bool found = true;
    for (auto &item : itemset) {
      if (find(transaction.begin(), transaction.end(), item) ==
          transaction.end()) {
        found = false;
        break;
      }
    }
    if (found)
      count++;
  }
  return (double)count / newTransactions.size();
}

void GenerateRules(double minConfidence = 0.6) {
  cout << "\nAssociation Rules:\n";
  for (auto &p : inventory) {
    const vector<string> &itemset = p.first;
    int n = itemset.size();
    if (n < 2)
      continue;
    int TotalCount = p.second;
    int SubsetCount = (1 << n);
    for (int i = 1; i < SubsetCount - 1; ++i) {
      vector<string> x, y;
      for (int j = 0; j < n; j++) {
        if (i & (1 << j))
          x.push_back(itemset[j]);
        else
          y.push_back(itemset[j]);
      }

      double SupX = support(x);
      double SupY = support(y);
      double SupXY = support(itemset);

      if (SupX == 0 || SupY == 0)
        continue;

      double confidence = SupXY / SupX;
      double lift = confidence / SupY;
      string strength;
      string correlation;
      if (confidence >= minConfidence) {
        strength = "Strong";

        cout << "{";
        for (auto &X : x)
          cout << X << " ";
        cout << "} -> {";
        for (auto &Y : y)
          cout << Y << " ";

        if (lift > 1)
          correlation = "Positive Correlation";
        else if (lift < 1)
          correlation = "Negative Correlation";
        else
          correlation = "No Correlation";
        cout << "} | conf: " << confidence << " |strength: " << strength
             << " | lift: " << lift << " |correlation: " << correlation << "\n";
      } else if (confidence < minConfidence) {
        strength = "Weak";

        cout << "{";
        for (auto &X : x)
          cout << X << " ";
        cout << "} -> {";
        for (auto &Y : y)
          cout << Y << " ";

        if (lift > 1)
          correlation = "Positive Correlation";
        else if (lift < 1)
          correlation = "Negative Correlation";
        else
          correlation = "Independent";

        cout << "} | conf: " << confidence << " |strength: " << strength
             << " | lift: " << lift << " |correlation: " << correlation << "\n";
      }
    }
  }
}
