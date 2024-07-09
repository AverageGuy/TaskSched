#ifndef SIMPLE_LIST_H
#define SIMPLE_LIST_H

#include "SafePtr.h"

/**
 * @brief A simple linked list implementation
 * 
 * @tparam T The type of elements stored in the list
 */
template<typename T>
class SimpleList {
private:
    /**
     * @brief Node structure for the linked list
     */
    struct Node {
        T data;     ///< The data stored in the node (SafePtr<Task> when T is Task)
        Node* next; ///< Pointer to the next node

        /**
         * @brief Construct a new Node object
         * 
         * @param value The value to be stored in the node
         */
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head;           ///< Pointer to the first node in the list
    Node* tail;           ///< Pointer to the last node in the list
    Node* read_position;  ///< Current read position for the read() method
    size_t size;          ///< Number of elements in the list

public:
    /**
     * @brief Construct a new empty SimpleList object
     */
    SimpleList() : head(nullptr), tail(nullptr), read_position(nullptr), size(0) {}

    /**
     * @brief Destroy the SimpleList object and free all allocated memory
     */
    ~SimpleList() {
        clear();
    }

    /**
     * @brief Add a new element to the end of the list
     * 
     * @param value The value to be added
     */
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

    /**
     * @brief Remove the first element from the list
     */
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

    /**
     * @brief Read the current element and move the read position to the next element
     * 
     * @return T The current element (empty SafePtr<Task> if read_position is null)
     */
    T read() {
        if (read_position) {
            T value = read_position->data;
            read_position = read_position->next;
            return value;
        }
        return T();  // Return empty SafePtr<Task>
    }

    /**
     * @brief Reset the read position to the beginning of the list
     */
    void rewind() {
        read_position = head;
    }

    /**
     * @brief Check if all elements have been read
     * 
     * @return true if read_position is null, false otherwise
     */
    bool is_exhausted() const {
        return read_position == nullptr;
    }

    /**
     * @brief Get the number of elements in the list
     * 
     * @return size_t The number of elements
     */
    size_t get_size() const {
        return size;
    }

    /**
     * @brief Remove all elements from the list
     */
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

    /**
     * @brief Iterator class for SimpleList
     */
    class iterator {
    private:
        Node* current; ///< Pointer to the current node

    public:
        /**
         * @brief Construct a new iterator object
         */
        iterator() : current(nullptr) {}

        /**
         * @brief Construct a new iterator object
         * 
         * @param node Pointer to the starting node
         */
        iterator(Node* node) : current(node) {}

        /**
         * @brief Dereference operator
         * 
         * @return T& Reference to the data in the current node
         */
        T& operator*() { return current->data; }

        /**
         * @brief Arrow operator
         * 
         * @return T* Pointer to the data in the current node
         */
        T* operator->() { return &current->data; }

        /**
         * @brief Prefix increment operator
         * 
         * @return iterator& Reference to the incremented iterator
         */
        iterator& operator++() { if (current) current = current->next; return *this; }

        /**
         * @brief Inequality comparison operator
         * 
         * @param other Another iterator to compare with
         * @return true if the iterators are not equal, false otherwise
         */
        bool operator!=(const iterator& other) const { return current != other.current; }

        /**
         * @brief Equality comparison operator
         * 
         * @param other Another iterator to compare with
         * @return true if the iterators are equal, false otherwise
         */
        bool operator==(const iterator& other) const { return current == other.current; }
    };

    /**
     * @brief Get an iterator pointing to the beginning of the list
     * 
     * @return iterator Iterator to the first element
     */
    iterator begin() { return iterator(head); }

    /**
     * @brief Get an iterator pointing to the end of the list
     * 
     * @return iterator Iterator to the element after the last element
     */
    iterator end() { return iterator(nullptr); }
};

#endif // SIMPLE_LIST_H
