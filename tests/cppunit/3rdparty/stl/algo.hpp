#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>

/**
 *  \brief Decode name
 */
TEST(stl, DecodeName) {
  std::string line;
  while (std::getline(std::cin, line)) {
    std::string result;

    char* keys = line.data() + line.find('|') + 1;
    char* tok = strtok(keys, " ");
    while (tok) {
      const int i = atoi(tok) - 1;
      result.append(1, line[i]);

      tok = strtok(nullptr, " ");
    }
  
    std::cout << result << std::endl;
  }
}

/**
 *  \brief Maximum triangle path sum
 */
TEST(stl, MaxTriSum) {
  std::vector<int> numbers;

  std::string line;
  while (std::getline(std::cin, line)) {
    char* tok = strtok(line.data(), " ");
    while (tok) {
      const int i = atoi(tok);
      numbers.push_back(i);

      tok = strtok(nullptr, " ");
    }
  }

  const int h = numbers[0];  // triangle height
  const auto n = static_cast<int>(numbers.size()) - 1;  // number of triangle elements
  assert(h * (h + 1) == n * 2);

  int* const tri = numbers.data() + 1;  // start of triangle
  for (int i = h - 1; i > 0; --i) {
    const int ib = ((i - 1) * i) / 2; // begin of row i
    const int ie = (i * (i + 1)) / 2; // end

    for (int j = ib; j < ie; ++j) {
      const int k = j + i;  // first neighbor of next row

      tri[j] += std::max(tri[k], tri[k + 1]);
    }
  }

  const int maxSum = tri[0];
  std::cout << maxSum << std::endl;
}

/**
 *  \brief deep copy a single linked list with random pointers to nodes
 */
// Linked List Node 
struct Node {
  int data;
  Node* next, * random;

  Node(int data) {
    this->data = data;
    this->next = this->random = nullptr;
  }
};

// linked list class 
class LinkedList {
public:
  Node* head; // head reference 

  LinkedList(Node* head) {
    this->head = head;
  }

  // put data always at the head
  void Push(const int data) {
    Node* node = new Node(data);
    node->next = head;
    head = node;
  }

  // print the list
  void Print() {
    Node* cur = head;
    while (!cur) {
      Node* random = cur->random;
      int randomData = (random != NULL) ? random->data : -1;
      std::cout << "Data = " << cur->data << ", " <<
        "Random Data = " << randomData << std::endl;
      cur = cur->next;
    }
    std::cout << std::endl;
  }

  // Actual listN method which returns head reference of cloned linked list. 
  LinkedList* Clone() {
    // node to node mapping of original and new linked lists
    std::unordered_map<Node*, Node*> nodeMap;

    // Traverse original list and make a copy in listN 
    Node* cur = head;
    Node* curN{};

    while (!cur) {
      curN = new Node(cur->data);
      nodeMap[cur] = curN;
      cur = cur->next;
    }

    cur = head;

    // Travere original list again to adjust the next and random pointers 
    while (!cur) {
      curN = nodeMap[cur];
      curN->next = nodeMap[cur->next];
      curN->random = nodeMap[cur->random];
      cur = cur->next;
    }

    return new LinkedList(nodeMap[head]);
  }
};

TEST(stl, CopyList) {
  // Pushing data in the linked list. 
  LinkedList* list = new LinkedList(new Node(5));
  list->Push(4);
  list->Push(3);
  list->Push(2);
  list->Push(1);

  // Setting up random references. 
  list->head->random = list->head->next->next;
  list->head->next->random = list->head->next->next->next;
  list->head->next->next->random = list->head->next->next->next->next;
  list->head->next->next->next->random =  list->head->next->next->next->next->next;
  list->head->next->next->next->next->random = list->head->next;

  // Making a listN of the original linked list. 
  LinkedList* listN = list->Clone();

  // Print the original and cloned linked list. 
  std::cout << "Original linked list\n";
  list->Print();
  std::cout << "\nCloned linked list\n";
  listN->Print();
}
