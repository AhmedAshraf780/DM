#include <iostream>
#include <unordered_map>
#include <vector>
// First Actor is the node
// node has
//   1. item
//   2. count
//   3. parent
//   4. next

using namespace std;

class Node {
public:
  string item;
  int count;
  Node *parent;
  unordered_map<string, Node *> nexts;
  Node *nextOne;

  Node() {
    count = 0;
    parent = nullptr;
    nextOne = nullptr;
  }
  Node(string it) {
    count = 0;
    item = it;
    parent = nullptr;
    nextOne = nullptr;
  }
};

// Second Actor is the Header Table
// node has
//   1. item
//   2. frequents
//   3. neighbours
class HeaderTable {
public:
  int frequency;
  Node *head;
  HeaderTable() {
    frequency = 0;
    head = nullptr;
  }
};

// Third Actor is the FPTREE
// node has
//   1. root
//   2. insertTransactions
//   3. printIt just for debugging

class FPTree {
public:
  Node *root;
  unordered_map<string, HeaderTable> headerTable;

  FPTree() { root = new Node(); }

  void insertTransactions(vector<vector<string>> transactions) {
    for (auto t : transactions) {
      Node *curr = root;
      for (auto item : t) {
        if (curr->nexts[item] != nullptr) {
          curr = curr->nexts[item];
          curr->count++;
        } else {
          Node *newNode = new Node(item);
          newNode->count = 1;
          newNode->parent = curr;
          curr->nexts[item] = newNode;
          if (headerTable[item].head == nullptr) {
            headerTable[item].head = newNode;
          } else {
            Node *p = headerTable[item].head;
            while (p->nextOne)
              p = p->nextOne;
            p->nextOne = newNode;
          }
          curr = newNode;
        }

        headerTable[item].frequency++;
      }
    }
  }

  void makeCT(const vector<pair<vector<string>, int>> &paths) {
    for (const auto &pc : paths) {
      const vector<string> &path = pc.first;
      int cnt = pc.second;
      Node *curr = root;
      for (const auto &item : path) {

        auto itChild = curr->nexts.find(item);
        if (itChild == curr->nexts.end() || itChild->second == nullptr) {
          Node *newNode = new Node(item);
          newNode->count = cnt;
          newNode->parent = curr;
          curr->nexts[item] = newNode;

          // add to header list
          if (headerTable[item].head == nullptr) {
            headerTable[item].head = newNode;
          } else {
            Node *p = headerTable[item].head;
            while (p->nextOne)
              p = p->nextOne;
            p->nextOne = newNode;
          }
        } else {
          Node *child = itChild->second;
          child->count += cnt;
        }

        headerTable[item].frequency += cnt;

        curr = curr->nexts[item];
      }
    }
  }

  // Return a reference copy of header table is OK; if you want to avoid copies return by const ref.
  unordered_map<string, HeaderTable> getHeaderTable() const {
    return headerTable;
  }

  // Return the first child node under 'root'
  Node *getBaseNode(Node *node = nullptr) {
    if (node == nullptr) node = root;
    for (auto &kv : node->nexts) {
      return kv.second;
    }
    return nullptr;
  }

  // Count number of nodes in subtree except the "root"
  int size(Node *node = nullptr) {
    if (node == nullptr) node = root;
    int count = 0;
    for (auto &[item, child] : node->nexts) {
      count += 1;
      count += size(child);
    }
    return count;
  }


  void printTree(Node *node = nullptr, int level = 0) {
    if (node == nullptr)
      node = root;

    for (auto &[item, child] : node->nexts) {
      // Print indentation based on level
      for (int i = 0; i <= level; i++)
        cout << "  ";
      cout << child->item << " (" << child->count << ")" << endl;

      printTree(child, level + 1);
    }
  }
};

FPTree buildCT(vector<pair<vector<string>, int>> paths) {
  FPTree root;
  root.makeCT(paths);
  return root;
}
