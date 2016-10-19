#include <iostream>

using namespace std;

class ListException {
    const char *const message_;

    public:
        ListException(const char *const message)
            : message_(message)
        {}

        const char *const get_message() const {
            return message_;
        }
};

class List {
    struct Node {
        int data_;
        Node* next_;
        Node* prev_;

        Node(int val = 0)
            : data_(val), next_(0), prev_(0)
        {}
    };

    Node* head_;

    public:
        List()
            : head_(new Node(0))
        {
            head_->next_ = head_;
            head_->prev_ = head_;
        }

        List(const List& other);

        ~List() {
            while (!empty()) {
                pop_front();
            }

            delete head_;
        }

        void push_back(int val) {
            Node* current_back = head_->prev_;
            Node* new_back = new Node(val);

            head_->prev_ = new_back;
            new_back->next_ = head_;

            current_back->next_ = new_back;
            new_back->prev_ = current_back;
        }

        void pop_back() {
            if (empty()) {
                throw ListException("Cannot pop the front node of an empty list");
            }

            Node* current_back = head_->prev_;
            Node* new_back = current_back->prev_;

            head_->prev_ = new_back;
            new_back->next_ = head_;

            delete current_back;
        }

        void push_front(int val) {
            Node* current_front = head_->next_;
            Node* new_front = new Node(val);

            head_->next_ = new_front;
            new_front->prev_ = head_;

            current_front->prev_ = new_front;
            new_front->next_ = current_front;
        }

        void pop_front() {
            if (empty()) {
                throw ListException("Cannot pop the front node of an empty list");
            }

            Node* current_front = head_->next_;
            Node* new_front = current_front->next_;

            head_->next_ = new_front;
            new_front->prev_ = head_;

            delete current_front;
        }

        int front() const {
            if (empty()) {
                throw ListException("Cannot get the front node of an empty list");
            }

            return head_->next_->data_;
        }

        int back() const {
            if (empty()) {
                throw ListException("Cannot get the back node of an empty list");
            }

            return head_->prev_->data_;        
        }

        int size() const;

        bool empty() const {
            return head_->next_ == head_;
        }

        void clear() {
            while (!empty()) {
                pop_front();
            }

            head_->next_ = head_;
            head_->prev_ = head_;
        }

        void swap(List& other);

        List& operator=(const List& other);

        class iterator {
            Node* current_node_;

            iterator(Node* current_node)
                : current_node_(current_node)
            {}

            friend class List;

            public:
                int& operator*() {
                    return current_node_->data_;
                }

                iterator& operator++() {
                    current_node_ = current_node_->next_;
                    return *this;
                }

                iterator operator++(int) {
                    iterator res(*this);
                    current_node_ = current_node_->next_;
                    return res;
                }

                bool operator==(const iterator& other) const {
                    return current_node_ == other.current_node_;
                }

                bool operator!=(const iterator& other) const {
                    return !operator==(other);
                }
        };

        iterator begin() {
            return iterator(head_->next_);
        }

        iterator end() {
            return iterator(head_);
        }

        iterator insert(iterator position, int val) {
            Node* new_node = new Node(val);
            Node* current_node = position.current_node_;
            Node* prev_node = current_node->prev_;

            new_node->next_ = current_node;
            current_node->prev_ = new_node;

            prev_node->next_ = new_node;
            new_node->prev_ = prev_node;

            return iterator(new_node);
        }

        iterator erase(iterator it);

};

int main() {
    try {
        List list;
        cout << "List empty? " << list.empty() << endl;

        list.push_back(42);
        cout << "List empty? " << list.empty() << endl;

        list.push_front(666);

        cout << "List front: " << list.front() << endl;
        cout << "List back: " << list.back() << endl;

        list.pop_front();
        cout << "List new front: " << list.front() << endl;
        cout << "List back: " << list.back() << endl;

        list.pop_back();
        cout << "List empty? " << list.empty() << endl;

        list.push_back(1);
        list.push_back(10);
        list.push_back(11);
        list.push_back(100);

        List::iterator it = list.begin();
        cout << *it << endl;
        ++it;
        cout << *it << endl;	
        it++;

        for (List::iterator it = list.begin(); it != list.end(); ++it) {
            cout << *it << ' ';
        }
        cout << endl;

        list.clear();
        cout << "List empty? " << list.empty() << endl;
        for (List::iterator it = list.begin(); it != list.end(); ++it) {
            cout << *it << ' ';
        }
    } catch (const ListException &ex) {
        cout << ex.get_message() << endl;
    }

    return 0;
}
