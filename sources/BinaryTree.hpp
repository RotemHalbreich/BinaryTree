#pragma once

#include <memory>
#include <iostream>
#include <stack>

namespace ariel
{
    template <typename T>

    class BinaryTree
    {
        enum TreeOrder
        {
            PREORDER,
            INORDER,
            POSTORDER
        };
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // Node inner Class:
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        class Node
        {
        private: //private Node
            T data;
            Node *left = nullptr, *right = nullptr;

        public: //public Node
            Node(T d) : data(d) {}

            friend class BinaryTree;
        }; //END of class ~Node~

    private: //private BinaryTree
        Node *root = nullptr;

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // BinaryTree Class:
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    public: //public BinaryTree
        BinaryTree<T>(Node *ptr = nullptr) : root(ptr) {}

        //Deep Copy
        void d_copy(Node &node1, Node &node2)
        {
            if (node2.right != nullptr)
            {
                node1.right = new Node(node2.right->data);
                d_copy(*(node1.right), *(node2.right));
            }
            if (node2.left != nullptr)
            {
                node1.left = new Node(node2.left->data);
                d_copy(*(node1.left), *(node2.left));
            }
        }

        BinaryTree(BinaryTree &&other) noexcept
        {
            root = other.root;
            other.root = nullptr;
        }

        BinaryTree(const BinaryTree &other)
        {
            root = new Node(other.root->data);
            d_copy(*root, *other.root);
        }

        BinaryTree &operator=(const BinaryTree other)
        {
            delete root;
            root = new Node(other.root->data);
            d_copy(*root, *other.root);
            return *this;
        }

        BinaryTree &operator=(BinaryTree &&other) noexcept
        {
            delete root;
            root = other.root;
            other.root = nullptr;
            return *this;
        }

        ~BinaryTree()
        {
            delete_tree(root);
        }

        void delete_tree(Node *node)
        {
            if (node != nullptr)
            {
                delete_tree(node->right);
                delete_tree(node->left);
            }
            delete node;
        }
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // Iterator inner Class:
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        class Iterator
        {
        private:
            Node *current;
            std::stack<Node *> st;
            TreeOrder order;

        public:
            Iterator(Node *ptr = nullptr) : current(ptr) {}
            Iterator(const TreeOrder tree_order, Node *root = nullptr) : order(tree_order), current(root) { init(root); }

            T &operator*() const
            {
                return current->data;
            }

            T *operator->() const
            {
                return &(current->data);
            }

            Iterator &operator++()
            {
                if (!st.empty())
                {
                    current = st.top();
                    st.pop();
                }
                else
                {
                    current = nullptr;
                }
                return *this;
            }

            Iterator operator++(int)
            {
                Iterator temp = *this;
                ++(*this);
                return temp;
            }

            bool operator==(const Iterator &other) const
            {
                return current == other.current;
            }

            bool operator!=(const Iterator &other) const
            {
                return !(*this == other);
            }

            void get_order(Node *root)
            {
                if (root == nullptr)
                {
                    return;
                }
                if (order == PREORDER)
                {
                    get_order(root->right);
                    get_order(root->left);
                    st.push(root);
                }
                if (order == INORDER)
                {
                    get_order(root->right);
                    st.push(root);
                    get_order(root->left);
                }
                if (order == POSTORDER)
                {
                    st.push(root);
                    get_order(root->right);
                    get_order(root->left);
                }
            }

            void init(Node *root)
            {
                get_order(root);
                if (!st.empty())
                {
                    current = st.top();
                    st.pop();
                }
            }

            Node *get_current_node()
            {
                return current;
            }
        }; //END of class ~Iterator~

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // Functions implementation:
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        BinaryTree &add_root(const T &root_data)
        {
            if (root == nullptr)
            {
                root = new Node(root_data);
            }
            root->data = root_data;
            return *this;
        }

        BinaryTree &add_left(const T &father, const T &left_child)
        {
            Node *parent = node_detected(father, root);

            if (parent == nullptr)
            {
                throw std::invalid_argument("A parent wasn't found in the Binary Tree!");
            }
            if (parent->left != nullptr)
            {
                parent->left->data = left_child;
            }
            if (parent->left == nullptr)
            {
                parent->left = new Node(left_child);
            }
            return *this;
        }

        BinaryTree &add_right(const T &father, const T &right_child)
        {
            Node *parent = node_detected(father, root);

            if (parent == nullptr)
            {
                throw std::invalid_argument("A parent wasn't found in the Binary Tree!");
            }
            if (parent->right != nullptr)
            {
                parent->right->data = right_child;
            }
            if (parent->right == nullptr)
            {
                parent->right = new Node(right_child);
            }
            return *this;
        }

        friend std::ostream &operator<<(std::ostream &os, BinaryTree<T> const &BT)
        {
            return os << "~Binary Tree~";
        }

        Node *node_detected(T data, Node *node)
        {
            for (auto itr = begin_inorder(); itr != end_inorder(); ++itr)
            {
                if (*itr == data)
                {
                    return itr.get_current_node();
                }
            }
            return nullptr;
        }

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        Iterator begin() { return Iterator{INORDER, root}; };
        Iterator end() { return Iterator{}; };
        Iterator end_preorder() { return Iterator{}; }
        Iterator begin_preorder() { return Iterator{PREORDER, root}; }
        Iterator end_inorder() { return Iterator{}; }
        Iterator begin_inorder() { return Iterator{INORDER, root}; }
        Iterator end_postorder() { return Iterator{}; }
        Iterator begin_postorder() { return Iterator{POSTORDER, root}; }
    }; //END of class ~BinaryTree~
}