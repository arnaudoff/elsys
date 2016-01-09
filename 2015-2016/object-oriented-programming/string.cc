#include <iostream>
#include <string.h>

using namespace std;

class StringException {
    string message_;

    public:
        StringException(string message)
            : message_(message)
        {}

        string get_message() const {
            return message_;
        }
};

class String {
    int capacity_;
    int size_;
    char *buffer_;

    void ensure_capacity(unsigned other_size) {
        if (size_ + other_size < capacity_) {
            return;
        }

        unsigned new_capacity;
        // If just doubling the capacity wouldn't work, resize with the size of the other buffer
        if (2 * capacity_ < size_ + other_size + 1) {
            new_capacity = size_ + other_size + 1;
        } else {
            new_capacity = 2 * capacity_;
        }

        char* old_buffer = buffer_;
        buffer_ = new char[new_capacity];
        strcpy(buffer_, old_buffer);
        capacity_ = new_capacity;
        delete [] old_buffer;
    }

    public:
        String(int capacity) {
            capacity_ = capacity;
            size_ = 0;
            buffer_ = new char[capacity];
        }

        String(const char *str) {
            size_ = strlen(str);
            capacity_ = size_ + 1;
            buffer_ = new char[capacity_];
            strcpy(buffer_, str);
        }

        String(const String& other) {
            size_ = other.size_;
            capacity_ = other.capacity_;
            buffer_ = new char[capacity_];
            strcpy(buffer_, other.buffer_);
        }

        String& operator=(const String& other) {
            if (this != &other) {
                delete[] buffer_;

                size_ = other.size_;
                capacity_ = other.capacity_;
                buffer_ = new char[capacity_];
                strcpy(buffer_, other.buffer_);
            }

            return *this;
        }

        ~String() {
            delete[] buffer_;
        }

        unsigned size() const {
            return size_;
        }

        unsigned length() const {
            return size_;
        }

        unsigned capacity() const {
            return capacity_;
        }

        bool empty() const {
            return size_ == 0;
        }

        void clear() {
            size_ = 0;
            capacity_ = 0;
            delete[] buffer_;
        }

        char& at(int index) {
            if (index < 0 || index > size_ - 1) {
                throw StringException("Index out of the boundaries of the string.");
            }

            return buffer_[index];
        }

        String& append(const String& other) {
            ensure_capacity(other.size_);
            size_ += other.size_;
            strcat(buffer_, other.buffer_);
            return *this;
        }

        void push_back(char ch) {
            ensure_capacity(1);
            buffer_[size_++] = ch;
            buffer_[size_] = '\0';
        }

        int find(const String& str, unsigned pos) {
            String substr(str);

            // O(n^2), could implement Rabin-Karp or KMP for better performance
            for (unsigned i = pos; i < length(); i++) {
                bool found = true;
                for (unsigned j = 0; j < substr.length(); j++) {
                    if (buffer_[i + j] != substr[j]) {
                        found = false;
                        break;
                    }
                }

                if (found) {
                    return i;
                }
            }

            return -1;
        }

        int find_first_of(const String& str, unsigned pos) {
            String substr(str);
            for (unsigned i = pos; i < str.length(); i++) {
                for (int j = 0; j < size_; j++) {
                    if (buffer_[j] == substr[i]) {
                        return j;
                    }
                }
            }
            return -1;
        }

        String substr(unsigned pos, unsigned n) {
            char *substr = new char[n + 1];
            strncpy(substr, buffer_ + pos, n);
            substr[n + 1] = '\0';
            String substr_obj(substr);
            return substr_obj;
        }

        String& operator+=(const String& other) {
            append(other);
            return *this;
        }

        char& operator[](int index) {
            return buffer_[index];
        }

        friend ostream& operator<<(ostream& out, const String& str);

        bool operator==(const String& other) const {
            return strcmp(buffer_, other.buffer_) == 0;
        }

        bool operator!=(const String& other) const {
            return !(*this == other);
        }

        bool operator<(const String& other) const {
            return strcmp(buffer_, other.buffer_) < 0;
        }

        bool operator>(const String& other) const {
            return strcmp(buffer_, other.buffer_) > 0;
        }

        bool operator<=(const String& other) const {
            return strcmp(buffer_, other.buffer_) <= 0;
        }

        bool operator>=(const String& other) const {
            return strcmp(buffer_, other.buffer_) >= 0;
        }

        class iterator {
            char* buffer_;
            unsigned index_;

            iterator(char* buffer, unsigned index)
                : buffer_(buffer), index_(index) 
            {}

            friend class String;

            public:
                iterator& operator++() {
                    ++index_;
                    return *this;
                }
                iterator operator++(int) {
                    iterator res(*this);
                    ++index_;
                    return res;
                }

                bool operator==(const iterator& other) const {
                    return index_ == other.index_;
                }

                bool operator!=(const iterator& other) const {
                    return !operator==(other);
                }

                char& operator*() {
                    return buffer_[index_];
                }
        };

        iterator begin() const {
            return iterator(buffer_, 0);
        }

        iterator end() const {
            return iterator(buffer_, size_);
        }
};

ostream& operator<<(ostream& out, const String& str) {
    if (str.size() > 0) {
        out << str.buffer_;
    }

    return out;
}

String operator+(const String& s1, const String& s2) {
    String res(s1);
    res.append(s2);
    return res;
}

unsigned count_spaces(const String& str) {
    unsigned spaces_count = 0;
    for (String::iterator it = str.begin(); it != str.end(); it++) {
        if (*it == ' ') {
            spaces_count++;
        }
    }

    return spaces_count;
}

void print_string(int number, const String& str) {
    cout << "string " << number << ": " << "<" << str << ">" << endl;
}

int main(int argc, char *argv[]) {
    String str1(argv[1]);
    String str2(argv[2]);

    print_string(1, str1);
    print_string(2, str2);

    cout << "string 1 length: " << str1.length() << endl;
    cout << "string 2 length: " << str2.length() << endl;

    cout << "string 1 spaces: " << count_spaces(str1) << endl;
    cout << "string 2 spaces: " << count_spaces(str2) << endl;

    if (str1 > str2) {
        cout << "<" << str1 << "> is greater than " << "<" << str2 << ">" << endl;
    } else if (str1 < str2) {
        cout << "<" << str1 << "> is less than " << "<" << str2 << ">" << endl;
    } else {
        cout << "<" << str1 << "> is equal to " << "<" << str2 << ">" << endl;
    }

    str1.push_back('!');
    str2.push_back('!');

    print_string(1, str1);
    print_string(2, str2);

    String str = str1 + str2;
    cout << "concatenation: " << "<" << str << ">" << endl;
    cout << "concatenation length: " << str.size() << endl;

    cout << "concatenation spaces: " << count_spaces(str) << endl;
    String s("!");
    cout << "index of '!': " << str.find_first_of(s, 0) << endl;

    cout << "substring: " << "<" << str.substr(12, 4) << ">" << endl;

    return 0;
}
