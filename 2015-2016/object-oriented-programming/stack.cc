#include <iostream>

using namespace std;

class StackException {
    string message_;

    public:
        StackException(string message) 
            : message_(message)
        {}

        string get_message() const {
            return message_;
        }
};

class Stack {
    const static int INITIAL_SIZE = 2;
    int *data_;
    int top_;
    int size_;

    public:
        Stack() {
            size_ = INITIAL_SIZE;
            data_ = new int[INITIAL_SIZE];
            top_ = -1;
        }

        Stack(const Stack& other) {
            size_ = other.size_;
            top_ = other.top_;
            data_ = new int[size_];
            for (int i = 0; i < size_; i++) {
                data_[i] = other.data_[i];
            }
        }

        void push(int value) {
            if (top_ >= size_ - 1) {
                resize();
            }

            data_[++top_] = value;
        }

        int pop() {
            if (top_ < 0) {
                throw StackException("Cannot pop from an empty stack!");
            }

            return data_[top_--];
        }

        ~Stack() {
            delete[] data_;
        }

        Stack& operator=(const Stack& other) {
            if (this != &other) {
                size_ = other.size_;
                top_ = other.top_;

                delete[] data_;

                data_ = new int[size_];
                for (int i = 0; i < size_; i++) {
                    data_[i] = other.data_[i];
                }
            }

            return *this;
        }
    private:
        void resize() {
            int *temp_data = data_;
            int new_size = size_ + INITIAL_SIZE;

            data_ = new int[new_size];
            for (int i = 0; i < size_; i++) {
                data_[i] = temp_data[i];
            }

            delete[] temp_data;
            size_ = new_size;
        }
};

int main() {
    Stack s1;
    s1.push(42);
    s1.push(666);
    s1.push(1337);

    Stack s2 = s1;
    Stack s3;
    s3 = s2;
    try {
        for (int i = 0; i <= 3; i++) {
            cout << s3.pop() << endl;
        }
    } catch (const StackException &ex) {
        cout << ex.get_message() << endl;
    }

    return 0;
}
