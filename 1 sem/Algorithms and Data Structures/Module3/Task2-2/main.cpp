#include <iostream>

using namespace std;

struct CBinaryNode {
    int key;
    CBinaryNode *left;
    CBinaryNode *right;

    CBinaryNode() : key(0), left(NULL), right(NULL) {}

    CBinaryNode(int data) : key(data), left(NULL), right(NULL) {}
};

class CBinaryTree {
private:
    void Clear(CBinaryNode *root);

public:
    CBinaryTree();

    ~CBinaryTree();

    void Add(int data, CBinaryNode *&root);

    void Print(CBinaryNode *root);

    CBinaryNode *root;
};

CBinaryTree::CBinaryTree() : root(NULL) {}

CBinaryTree::~CBinaryTree() {
    Clear(root);
}

void CBinaryTree::Clear(CBinaryNode *root) {
    if (root) {
        Clear(root->left);
        Clear(root->right);
        delete (root);
    }
}

void CBinaryTree::Add(int data, CBinaryNode *&root) {
    if (!root) {//если свободный корень root у узла
        root = new CBinaryNode(data);
        return;
    }
    if (root->key <= data) {
        Add(data, root->right);
    } else if (root->key > data) {
        Add(data, root->left);
    }
}

void CBinaryTree::Print(CBinaryNode *root) {
    if (root) {
        cout << root->key << " ";
        Print(root->left);
        Print(root->right);
    }
}

int main() {
    int size = 0;
    cin >> size;

    CBinaryTree tree;

    for (int i = 0; i < size; ++i) {
        int q = 0;
        cin >> q;
        tree.Add(q, tree.root);
    }
    tree.Print(tree.root);
    return 0;
}