//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Devin Wheeler
// Version     : 2.2
// Copyright   : Copyright © 2023 SNHU COCE
// Description : Capstone Binary Search Tree (Enhanced for Filtering/Sorting)
//============================================================================

#include <iostream>
#include <time.h>
#include <vector>
#include <algorithm>
#include <limits>
#include "CSVparser.hpp"
#include <sstream>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// Convert string with dollar sign to double
double strToDouble(string str, char ch);

// Structure to hold bid information
struct Bid {
    string bidId;         // Auction ID
    string title;         // Auction Title
    string fund;          // Fund
    double amount;        // Winning Bid
    string department;    // Department
    string closeDate;     // Close Date

    Bid() {
        amount = 0.0;
    }
};

// Tree node for BST
struct Node {
    Bid bid;
    Node* left;
    Node* right;

    Node() {
        left = nullptr;
        right = nullptr;
    }

    Node(Bid aBid) : Node() {
        bid = aBid;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

class BinarySearchTree {
private:
    Node* root;
    void addNode(Node* node, Bid bid);
    void inOrder(Node* node);
    void postOrder(Node* node);
    void preOrder(Node* node);
    Node* removeNode(Node* node, string bidId);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void PostOrder();
    void PreOrder();
    void Insert(Bid bid);
    void Remove(string bidId);
    Bid Search(string bidId);
    Node* getRoot() { return root; }
    void collectFilteredBids(Node* node, double targetAmount, vector<Bid>& filtered);
};

// Constructor
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

// Destructor
BinarySearchTree::~BinarySearchTree() {
    while (root != nullptr) {
        Remove(root->bid.bidId);
    }
}

// Traverse the tree in-order
void BinarySearchTree::InOrder() {
    inOrder(root);
}

// Traverse the tree in post-order
void BinarySearchTree::PostOrder() {
    postOrder(root);
}

// Traverse the tree in pre-order
void BinarySearchTree::PreOrder() {
    preOrder(root);
}

// Insert a bid into the tree
void BinarySearchTree::Insert(Bid bid) {
    if (root == nullptr) {
        root = new Node(bid);
    }
    else {
        addNode(root, bid);
    }
}

// Remove a bid from the tree
void BinarySearchTree::Remove(string bidId) {
    root = removeNode(root, bidId);
}

// Search for a bid by ID
Bid BinarySearchTree::Search(string bidId) {
    Node* current = root;
    while (current != nullptr) {
        if (current->bid.bidId == bidId) {
            return current->bid;
        }
        else if (bidId < current->bid.bidId) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    Bid bid;
    return bid;
}

// Recursive function to add a node to the tree
void BinarySearchTree::addNode(Node* node, Bid bid) {
    if (bid.bidId < node->bid.bidId) {
        if (node->left == nullptr) {
            node->left = new Node(bid);
        }
        else {
            addNode(node->left, bid);
        }
    }
    else {
        if (node->right == nullptr) {
            node->right = new Node(bid);
        }
        else {
            addNode(node->right, bid);
        }
    }
}

// In-order traversal of tree
void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);
        cout << node->bid.bidId << ": " << node->bid.title << " | " << node->bid.amount << " | " << node->bid.fund << endl;
        inOrder(node->right);
    }
}

// Post-order traversal of tree
void BinarySearchTree::postOrder(Node* node) {
    if (node != nullptr) {
        postOrder(node->left);
        postOrder(node->right);
        cout << node->bid.bidId << ": " << node->bid.title << " | " << node->bid.amount << " | " << node->bid.fund << endl;
    }
}

// Pre-order traversal of tree
void BinarySearchTree::preOrder(Node* node) {
    if (node != nullptr) {
        cout << node->bid.bidId << ": " << node->bid.title << " | " << node->bid.amount << " | " << node->bid.fund << endl;
        preOrder(node->left);
        preOrder(node->right);
    }
}

// Recursive function to remove a bid from the tree
Node* BinarySearchTree::removeNode(Node* node, string bidId) {
    if (node == nullptr) return nullptr;

    if (bidId < node->bid.bidId) {
        node->left = removeNode(node->left, bidId);
    }
    else if (bidId > node->bid.bidId) {
        node->right = removeNode(node->right, bidId);
    }
    else {
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            return nullptr;
        }
        else if (node->left != nullptr && node->right == nullptr) {
            Node* temp = node->left;
            delete node;
            return temp;
        }
        else if (node->left == nullptr && node->right != nullptr) {
            Node* temp = node->right;
            delete node;
            return temp;
        }
        else {
            Node* temp = node->right;
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            node->bid = temp->bid;
            node->right = removeNode(node->right, temp->bid.bidId);
        }
    }
    return node;
}

// Collect bids with amount less than or equal to target
void BinarySearchTree::collectFilteredBids(Node* node, double targetAmount, vector<Bid>& filtered) {
    if (node == nullptr) return;
    collectFilteredBids(node->left, targetAmount, filtered);
    if (node->bid.amount <= targetAmount) {
        filtered.push_back(node->bid);
    }
    collectFilteredBids(node->right, targetAmount, filtered);
}

// Display a bid with useful details
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | " << bid.department << " | " << bid.closeDate << endl;
}

// Convert dollar string to double
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

// Load bid data from CSV file into BST
void loadBids(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;
    csv::Parser file = csv::Parser(csvPath);
    try {
        for (unsigned int i = 0; i < file.rowCount(); i++) {
            Bid bid;
            bid.title = file[i][0];
            bid.bidId = file[i][1];
            bid.department = file[i][2];
            bid.closeDate = file[i][3];
            bid.amount = strToDouble(file[i][4], '$');
            bid.fund = file[i][8];
            bst->Insert(bid);
        }
    }
    catch (csv::Error& e) {
        cerr << e.what() << endl;
    }
}

// Helper function needed to sort by date. Converts dd/mm/yy to an int to be poperly compared 
int dateToInt(const string& dateStr) {
    int month, day, year;
    char sep;
    istringstream iss(dateStr);
    iss >> month >> sep >> day >> sep >> year;
    return year * 10000 + month * 100 + day;
}

// Sort filtered bids by department or date
void sortFilteredBids(vector<Bid>& bids, const string& sortBy) {
    if (sortBy == "department") {
        sort(bids.begin(), bids.end(), [](Bid a, Bid b) {
            return a.department < b.department;
            });
    }
    else if (sortBy == "date") {
        sort(bids.begin(), bids.end(), [](Bid a, Bid b) {
            return dateToInt(a.closeDate) < dateToInt(b.closeDate);
            });
    }
}


// Main program entry point
int main(int argc, char* argv[]) {
    string csvPath = "eBid_Monthly_Sales.csv";
    BinarySearchTree* bst = new BinarySearchTree();
    Bid bid;
    int choice = 0;

    while (choice != 9) {
        cout << "\nMenu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  5. Filter by Winning Bid and Sort" << endl;
        cout << "  9. Exit" << endl;

        cout << "Enter choice: ";
        while (!(cin >> choice) || (choice < 1 && choice != 9) || choice > 5 && choice != 9) {
            cout << "Invalid input. Please enter a number from the menu options (1-5 or 9): ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice) {
        case 1:
            loadBids(csvPath, bst);
            break;
        case 2:
            bst->InOrder();
            break;
        case 3: {
            string bidKey;
            cout << "Enter bid ID: ";
            cin >> bidKey;
            bid = bst->Search(bidKey);
            if (!bid.bidId.empty()) displayBid(bid);
            else cout << "Bid not found." << endl;
            break;
        }
        case 4: {
            string bidKey;
            cout << "Enter bid ID to remove: ";
            cin >> bidKey;
            bst->Remove(bidKey);
            break;
        }
        case 5: {
            // Logic to filter ans sort bids
            double targetAmount;
            int sortOption;
            string sortBy;

            cout << "Enter maximum winning bid amount to filter: $";
            while (!(cin >> targetAmount) || targetAmount < 0) {
                cout << "Invalid amount. Please enter a valid non-negative number: $";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            cout << "Sort by:\n  1. Department\n  2. Close Date\nEnter choice (1 or 2): ";
            while (!(cin >> sortOption) || (sortOption != 1 && sortOption != 2)) {
                cout << "Invalid input. Please enter 1 (Department) or 2 (Close Date): ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            sortBy = (sortOption == 1) ? "department" : "date";

            vector<Bid> filtered;
            bst->collectFilteredBids(bst->getRoot(), targetAmount, filtered);
            sortFilteredBids(filtered, sortBy);

            if (filtered.empty()) {
                cout << "No bids found under $" << targetAmount << "." << endl;
            }
            else {
                for (const Bid& b : filtered) {
                    displayBid(b);
                }
                cout << filtered.size() << " bid(s) matched your filter.\n";
            }
            break;
        }
        }
    }

    cout << "Good bye." << endl;
    return 0;
}
