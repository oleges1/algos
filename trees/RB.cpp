#include <iostream>

const bool RED = true;
const bool BLACK = false;
int counter = 0;

template <typename Value>
class Node {
 public:
    Value value_;
    bool color_;
    Node* right_;
    Node* left_;

    void print_sorted();
    Node* insert(Node* node, Value val);

    explicit Node(Value value) :
        value_(value), left_(nullptr), right_(nullptr), color_(RED)
    {}

    bool search(Value val) {
        auto tmp = this;
        while (tmp->value_ != val) {
            if ((tmp->right_ != nullptr) && (tmp->value_ < val)) {
                tmp = tmp->right_;

            } else if ((tmp->left_ != nullptr) && (tmp->value_ > val)) {
                tmp = tmp->left_;
            } else {
                break;
            }
        }
        if (tmp->value_ == val) {
            return 1;
        }
        return 0;
    }

    void insert(Value val) {
        auto h = this;
        h = insert(h, val);
        h->color_ = BLACK;
    }
};

template <typename Value>
Node<Value>* rotareLeft(Node<Value>* h) {
    auto x = h->right_;
    h->right_ = x->left_;
    x->left_ = h;
    x->color_ = h->color_;
    h->color_ = RED;
    return x;
}

template <typename Value>
Node<Value>* rotareRight(Node<Value>* h) {
    auto x = h->left_;
    h->left_ = x->right_;
    x->right_ = h;
    x->color_ = h->color_;
    h->color_ = RED;
    return x;
}

template <typename Value>
void flipColors(Node<Value>* h) {
    h->color_ = !h->color_;
    h->left_->color_ = !h->left_->color_;
    h->right_->color_ = !h->right_->color_;
}

template <typename Value>
bool IsRed(Node<Value>* h) {
    if (h == nullptr) {
        return false;
    }
    return h->color_;
}

template <typename Value>
Node<Value>* Node<Value>::insert(Node<Value>* h, Value key) {
    if (h == nullptr) {
        ++counter;
        return new Node<int>(key);
    }
    if (IsRed(h->left_) && IsRed(h->right_)) {
        flipColors(h);
    }
    if (key < h->value_) {
        h->left_ = insert(h->left_, key);
    } else if (key > h->value_) {
        h->right_ = insert(h->right_, key);
    }
    if (IsRed(h->right_) && !IsRed(h->left_)) {
        h = rotareLeft(h);
    }
    if (IsRed(h->left_) && !IsRed(h->right_)) {
        h = rotareRight(h);
    }
    return h;
}

template <typename Value>
void Node<Value>::print_sorted() {
    Node<Value> *node = this;
    if (node != nullptr) {
        node->left_->print_sorted();
        std::cout << node->value_ << " ";
        if (node->left_ == nullptr) {
            std::cout << "null ";
        } else {
            std::cout << node->left_->value_ << " ";
        }
        if (node->right_ == nullptr) {
            std::cout << "null ";
        } else {
            std::cout << node->right_->value_ << " ";
        }
        if (node->color_) {
            std::cout << "R\n";
        } else {
            std::cout << "B\n";
        }
        node->right_->print_sorted();
    }
}

int main() {
    int n;
    std::cin >> n;
    int toDo, value;
    if (n > 0) {
        std::cin >> toDo >> value;
    }
    n--;
    while (toDo && (n >= 1)) {
        std::cout << "No\n";
        std::cin >> toDo >> value;
        n--;
    }
    if (n > 0) {
        counter++;
        Node<int>* tree = new Node<int>(value);
        for (int i = 0; i < n; ++i) {
            std::cin >> toDo >> value;
            if (toDo) {
                if (tree->search(value)) {
                    std::cout << "Yes\n";
                } else {
                    std::cout << "No\n";
                }
            } else {
                tree = tree->insert(tree, value);
                tree->color_ = BLACK;
            }
        }
        std::cout << counter << "\n";
        tree->color_ = BLACK;
        tree->print_sorted();
    } else {
        if (!toDo) {
            counter++;
            Node<int>* tree = new Node<int>(value);
            std::cout << counter << "\n";
            tree->color_ = BLACK;
            tree->print_sorted();
        } else {
            std::cout << "No\n0\n";
        }
    }
    return 0;
}
