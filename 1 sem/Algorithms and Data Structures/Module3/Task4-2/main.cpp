/*
 * 4_2. Порядковые статистики.? Дано число N и N строк.
 * Каждая строка содержащит команду добавления или удаления натуральных чисел,
 * а также запрос на получение k-ой порядковой статистики.
 * Команда добавления числа A задается положительным числом A, команда удаления числа A задается отрицательным числом “-A”.
 * Запрос на получение k-ой порядковой статистики задается числом k.
 * Требуемая скорость выполнения запроса - O(log n).
 */
#include <iostream>


using namespace std;

struct Node {
    int key;
    int height;
    int size;
    Node *left;
    Node *right;

    Node() : key(0), height(1), size(1), left(NULL), right(NULL) {}

    Node(int key) : key(key), height(1), size(1), left(NULL), right(NULL) {}
};

class Tree {
private:

public:
    Node *root;

    Tree() : root(NULL) {}

    ~Tree() {
        RemoveNode(root);
    }

    void AddItem(int key, Node *&root) {
        if (root == NULL) { //добавляем, если узел пустой
            root = new Node(key);
            return;
        }

        if (key < root->key) // значение нового узла, меньше текущего(на котором остановились)
            AddItem(key, root->left); // рукурсивно идем влево
        else {
            AddItem(key, root->right); // рукурсивно идем вправо
        }
        root->height = max(GetHeight(root->left), GetHeight(root->right)) + 1; // обновляем высоту дерева
        root->size = GetSize(root->left) + GetSize(root->right) + 1; // обновляем размер дерева
        BalanceTree(root); // сразу же балансировка
    }

    void RemoveItem(int key, Node *&root) {
        if (root == NULL) { // нет такого
            return;
        }
        if (key < root->key) { // значение удаляемого узла, меньше текущего(на котором остановились) узла
            RemoveItem(key, root->left); // рукурсивно идем влево
        } else if (key > root->key) {
            RemoveItem(key, root->right); // рукурсивно идем вправо
        } else {
            // нашли узел, и проверяем на три возможных случая
            if ((root->left == NULL) ||
                (root->right == NULL)) { // 1) один дочерний элемент или 2) ни одного дочернего элемента
                Node *buffNode = root->left ? root->left
                                            : root->right; // в buffNode хранится указатель на дочерний узел, если есть
                if (buffNode == NULL) { //Ни одного дочернего узла
                    //buffNode = root;
                    root = NULL; // обнуляем удаляемый узел
                } else {  // Один дочерний узел
                    *root = *buffNode; // переносим в текущий узел, указатель на его дочерний элемент, тем самым удалив нужный нам элемент
                }
                delete (buffNode);
            } else { // Два дочерних узла
                //Заменяем ключ удаляемого узла на ключ минимального узла из правого поддерева, удаляя последний.
                Node *buffNode = MinNode(root->right);
                root->key = buffNode->key;
                RemoveItem(buffNode->key, root->right);
            }
        }
        if (root == NULL) {
            return;
        }
        root->height = max(GetHeight(root->left), GetHeight(root->right)) + 1;
        root->size = GetSize(root->left) + GetSize(root->right) + 1;
        BalanceTree(root);
    }

    Node *MinNode(Node *root) {
        //находим минимальное значение
        Node *node = root;
        while (node->left) {
            node = node->left;
        }
        return node;
    }

    int FindStat(int k, Node *root) {
        if (k == GetSize(root->left) + 1) {
            return root->key;
        } else if (k < GetSize(root->left) + 1) {
            FindStat(k, root->left);
        } else {
            FindStat(k - GetSize(root->left) - 1, root->right);
        }
    }

    int GetHeight(Node *root) {
        if (root == NULL) {
            return 0;
        }
        return root->height;
    }

    int GetSize(Node *root) {
        if (root == NULL) {
            return 0;
        }
        return root->size;
    }

    int AnalyzeBalance(Node *root) {
        if (root == NULL) {
            return 0;
        }
        return GetHeight(root->left) - GetHeight(root->right);
    }

    void RotateLeft(Node *&a) {
        // переменные названы, как в лекции
        Node *b = a->right;
        Node *C = b->left;
        b->left = a;
        a->right = C;

        //перезаписываем новую высоту и размеры поддеревьев
        a->height = max(GetHeight(a->left), GetHeight(a->right)) + 1;
        b->height = max(GetHeight(b->left), GetHeight(b->right)) + 1;
        a->size = GetSize(a->left) + GetSize(a->right) + 1;
        b->size = GetSize(b->left) + GetSize(b->right) + 1;

        a = b;
    }

    void RotateRight(Node *&a) {
        // переменные названы, как в лекции
        Node *b = a->left;
        Node *C = b->right;
        b->right = a;
        a->left = C;

        //перезаписываем новую высоту и размеры поддеревьев
        a->height = max(GetHeight(a->left), GetHeight(a->right)) + 1;
        b->height = max(GetHeight(b->left), GetHeight(b->right)) + 1;
        a->size = GetSize(a->left) + GetSize(a->right) + 1;
        b->size = GetSize(b->left) + GetSize(b->right) + 1;

        a = b;
    }

    void BalanceTree(Node *&root) {
        int balanceFactor = AnalyzeBalance(root);// разность высот, вычитаем из высоты левого поддерева правое

        if (balanceFactor > 1) { // если левое поддерево больше, чем правое на 1
            if (GetHeight(root->left->left) >= GetHeight(root->left->right)) { // если у левого поддерева высота левого поддерево меньше правого
                //Правое малое вращение
                RotateRight(root);
            } else {
                //Правое большое вращение
                RotateLeft(root->left);
                RotateRight(root);
            }
        } else if (balanceFactor < -1) { // если правое поддерево больше, чем левое на 1
            if (GetHeight(root->right->right) > GetHeight(root->right->left)) { // если у правого поддерева высота левогог поддерево меньше правого
                //Левое малое вращение
                RotateLeft(root);
            } else {
                //Левое большое вращение
                RotateRight(root->right);
                RotateLeft(root);
            }
        }
    }

    void RemoveNode(Node *&root) {
        if (root) {
            RemoveNode(root->left);
            RemoveNode(root->right);
            delete (root);
        }
    }
};

int main() {
    Tree tree;
    int n = 0;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        int a = 0, k = 0;
        cin >> a >> k;
        if (a >= 0) {
            tree.AddItem(a, tree.root);
        } else {
            tree.RemoveItem(a * -1, tree.root);
        }
        cout << tree.FindStat(k + 1, tree.root) << endl;
    }
    return 0;
}