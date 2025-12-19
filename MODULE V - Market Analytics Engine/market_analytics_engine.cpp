#include<bits/stdc++.h>
using namespace std;

// A Truck (Node) Data Structure to save trucks data and preform an AVL tree
// for truck prices
struct Truck{
    int price;
    Truck* left;
    Truck* right;
    int height;
    int size;

    Truck(int price, Truck* left = nullptr, Truck* right = nullptr) 
        : price(price), left(left), right(right), height(1), size(1) {}
};

// Helper Functions for getting (Height/Size) and setting (Height/Size)

int getHeight(Truck* t) {
    if (t == nullptr) return 0;
    return t->height;
}

int getSize(Truck* t) {
    if (t == nullptr) return 0;
    return t->size;
}

void updateHeight(Truck* t) {
    if (t == nullptr) return;
    t->height = 1 + max(getHeight(t->left), getHeight(t->right));
}

void updateSize(Truck* t) {
    if (t == nullptr) return;
    t->size = 1 + getSize(t->left) + getSize(t->right);
}

// Updating Stats after each insertion
void updateStats(Truck* t) {
    updateHeight(t);
    updateSize(t);
}

// AVL property checker
int getBalance(Truck* t) {
    if (t == nullptr) return 0;
    return getHeight(t->left) - getHeight(t->right);
}

// A Right Rotation on a Truck
Truck* rightRotation(Truck* t) {
    Truck* newRoot = t->left;
    Truck* newRightChild = t;
    Truck* newLeftChildOfRightChild = t->left->right;

    newRoot->right = newRightChild;
    newRightChild->left = newLeftChildOfRightChild;
    updateStats(newRightChild);
    updateStats(newRoot);
    return newRoot;
}

// A Left Rotation on a Truck
Truck* leftRotation(Truck* t) {
    Truck* newRoot = t->right;
    Truck* newLeftChild = t;
    Truck* newRightChildOfLeftChild = t->right->left;

    newRoot->left = newLeftChild;
    newLeftChild->right = newRightChildOfLeftChild;
    updateStats(newLeftChild);
    updateStats(newRoot);
    return newRoot;
}

// Handleing 4 types of rotations
Truck* rebalanceTree(Truck* t) {
    // Getting Balances and how to deal with them
    int balance = getBalance(t);
    bool leftHeavy = true;
    if (balance <= 1 && balance >= -1) return t;
    else if (balance < -1) leftHeavy = false;

    Truck* chosenChild = (leftHeavy) ? t->left:t->right;
    int childBalance = getBalance(chosenChild);
    // Left-Heavy Case
    if (leftHeavy) {
        if (childBalance < 0) t->left = leftRotation(chosenChild); // -> Left-Right Case
        return rightRotation(t);
    }
    // Right-Heavy
    if (!leftHeavy) {
        if (childBalance > 0) t->right = rightRotation(chosenChild); // -> Right-Left Case
        return leftRotation(t);
    }
}

// Inserting in an AVL Tree and keeping the balance
Truck* insert(Truck* t, int price){
    if (t == nullptr) {
        return new Truck(price);
    }
    if (price < t->price) t->left = insert(t->left, price);
    if (price >= t->price) t->right = insert(t->right, price);
    updateStats(t);
    return rebalanceTree(t);
}

// Answering Queries
int findLowerThan(Truck* t, int maxPrice) {
    if (t == nullptr) return 0;
    if (t->price > maxPrice) {
        return findLowerThan(t->left, maxPrice);
    }
    else {
        int currentAndLeft = 1 + getSize(t->left);
        return currentAndLeft + findLowerThan(t->right, maxPrice);
    }
}

int main(){
    int queries_num;
    cin >> queries_num;
    Truck* root = nullptr;
    while(queries_num--) {
        int type, price;
        cin >> type >> price;
        switch (type)
        {
        case 1:
            root = insert(root, price);
            break;
        
        case 2:
            cout << findLowerThan(root, price) << endl;
            break;
        }
    }
}