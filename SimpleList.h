#ifndef SIMPLE_LIST_H
#define SIMPLE_LIST_H

#include "SafePtr.h"

template<typename T>
class SimpleList {
private:
    struct Node {
        T data;  // This will now be SafePtr<Task>
        Node* next;
        
        Node(const T& value) : data(value), next(nullptr) {}
    };
    
    Node* head;
    Node* tail;
    Node* read_position;
    size_t size;

public:
    SimpleList() : head(nullptr), tail(nullptr), read_position(nullptr), size(0) {}

    ~SimpleList() {
        clear();
    }

    void push_back(const T& value) {
        Node* new_node = new Node(value);
        if (!head) {
            head = tail = read_position = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
        ++size;
    }

    void pop_front() {
        if (head) {
            Node* temp = head;
            head = head->next;
            if (read_position == temp) {
                read_position = head;
            }
            delete temp;
            --size;
            if (!head) {
                tail = nullptr;
                read_position = nullptr;
            }
        }
    }

    T read() {
        if (read_position) {
            T value = read_position->data;
            read_position = read_position->next;
            return value;
        }
        return T();  // Return empty SafePtr<Task>
    }

    void rewind() {
        read_position = head;
    }

    bool is_exhausted() const {
        return read_position == nullptr;
    }

    size_t get_size() const {
        return size;
    }

    void clear() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        read_position = nullptr;
        size = 0;
    }

    class iterator {
    private:
        Node* current;
    public:
        iterator() : current(nullptr) {}
        iterator(Node* node) : current(node) {}
        
        T& operator*() { return current->data; }
        T* operator->() { return &current->data; }
        iterator& operator++() { if (current) current = current->next; return *this; }
        bool operator!=(const iterator& other) const { return current != other.current; }
        bool operator==(const iterator& other) const { return current == other.current; }
    };

    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }
};

#endif // SIMPLE_LIST_H

