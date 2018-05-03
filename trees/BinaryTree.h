#include <iostream>
#include <vector>
#include <random>

template <typename Value>
class Node {
public:
    explicit Node(Value value) : 
        value_(value), parent_(nullptr),
        left_(nullptr), right_(nullptr) 
    {}

    Value get_value() const {
        return value_;
    }

    // all methods are defined for a subtree of this node
    Node* min() const;
    Node* max() const;
    Node* next() const;
    Node* prev() const;
    Node* lower_bound(const Value& value);
    Node* insert(Node* node);
    Node* remove(const Value& value);

private:
    Value value_;
    Node* parent_;
    Node* right_;
    Node* left_;

public:
    Node<Value>* min() {
        Node<Value> *tmp = this;
        while (tmp->left_ != nullptr) {
            tmp = tmp->left_;
        }
        return tmp;
    }

    Node<Value>* max() {
        Node<Value> *tmp = this;
        while (tmp->right_ != nullptr) {
            tmp = tmp->right_;
        }
        return tmp;
    }

    Node<Value>* next() {
        return right_ == nullptr ? nullptr : right_->min();
    }

    Node<Value>* prev() {
        return left_ == nullptr ? nullptr : left_->max();
    }
};

template <typename Value>
Node<Value> * Node<Value>::lower_bound(const Value& key) {
    Node<Value> *tmp = this;
    while (tmp->value_ != key) {
        if ((tmp->right_ != nullptr) && (tmp->value_ < key)) {
            tmp = tmp->right_;

        } else if ((tmp->left_ != nullptr) && (tmp->value_ > key)) {
            tmp = tmp->left_;
        } else {
            return tmp;
        }
    }
    return tmp;
}

template <typename Value>
Node<Value>* Node<Value>::insert(Node<Value>* node) {
    Node<Value>* bound = lower_bound(node->value_);

    if (bound->value_ > node->value_) {
        bound->left_  = node;
        node->parent_ = bound;

        return node;
    } else if (bound->value_ < node->value_) {
        bound->right_ = node;
        node->parent_ = bound;

        return node;
    }
    return bound;
}

template <typename Value>
Node<Value>* Node<Value>::remove(const Value& value) {
    Node<Value>* bound = lower_bound(value);
    if (bound->value_ != value) {
        return nullptr;
    }
    Node<Value>* heir = nullptr;
    if (bound->left_ != nullptr) {
        heir = prev();
    } else {
        heir = next();
    }

    if (heir == nullptr) {
        heir = bound;
    } else {
        bound->value_ = heir->value_;
    }

    if (heir->parent_ != nullptr) {
        if (heir->parent_->value_ > heir->value_) {
            heir->parent_->left_ = nullptr;
        } else {
            heir->parent_->right_ = nullptr;
        }
    }

    return heir;
}
