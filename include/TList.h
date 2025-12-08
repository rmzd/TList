#pragma once
#include <iostream>
#include <stdexcept>

template <class T>
class TList {
private:
    class TListNode {
    private:
        T value;
        TListNode* prev;
        TListNode* next;

    public:
        TListNode(const T& val = T(), TListNode* p = nullptr, TListNode* n = nullptr)
            : value(val), prev(p), next(n) {
        }

        T GetValue() const { return value; }
        T& GetValueRef() { return value; }
        TListNode* GetPrev() const { return prev; }
        TListNode* GetNext() const { return next; }

        void SetValue(const T& val) { value = val; }
        void SetPrev(TListNode* p) { prev = p; }
        void SetNext(TListNode* n) { next = n; }
    };

    TListNode* head;
    size_t size;

public:
    static const size_t MAX_SIZE = 10000;

    TList() : head(nullptr), size(0) {}

    TList(size_t initialSize) : head(nullptr), size(0) {
        if (initialSize > MAX_SIZE) {
            throw std::out_of_range("Size exceeds maximum allowed");
        }
        for (size_t i = 0; i < initialSize; ++i) {
            AddEnd(T());
        }
    }

    TList(const TList& other) : head(nullptr), size(0) {
        TListNode* current = other.head;
        if (current) {
            for (size_t i = 0; i < other.size; ++i) {
                AddEnd(current->GetValue());
                current = current->GetNext();
            }
        }
    }

    TList(TList&& other) noexcept : head(other.head), size(other.size) {
        other.head = nullptr;
        other.size = 0;
    }

    ~TList() {
        while (size > 0) {
            RemoveStart();
        }
    }

    TList& operator=(const TList& other) {
        if (this != &other) {
            while (size > 0) {
                RemoveStart();
            }

            TListNode* current = other.head;
            if (current) {
                for (size_t i = 0; i < other.size; ++i) {
                    AddEnd(current->GetValue());
                    current = current->GetNext();
                }
            }
        }
        return *this;
    }

    TList& operator=(TList&& other) noexcept {
        if (this != &other) {
            while (size > 0) {
                RemoveStart();
            }

            head = other.head;
            size = other.size;
            other.head = nullptr;
            other.size = 0;
        }
        return *this;
    }

    size_t GetSize() const { return size; }
    bool IsEmpty() const { return size == 0; }
    bool IsFull() const { return size >= MAX_SIZE; }

    void AddStart(const T& value) {
        if (IsFull()) {
            throw std::overflow_error("List is full");
        }

        TListNode* newNode = new TListNode(value);

        if (IsEmpty()) {
            head = newNode;
            head->SetNext(head);
            head->SetPrev(head);
        }
        else {
            TListNode* tail = head->GetPrev();
            newNode->SetNext(head);
            newNode->SetPrev(tail);
            tail->SetNext(newNode);
            head->SetPrev(newNode);
            head = newNode;
        }
        ++size;
    }

    void AddEnd(const T& value) {
        if (IsFull()) {
            throw std::overflow_error("List is full");
        }

        if (IsEmpty()) {
            AddStart(value);
        }
        else {
            TListNode* tail = head->GetPrev();
            TListNode* newNode = new TListNode(value, tail, head);
            tail->SetNext(newNode);
            head->SetPrev(newNode);
            ++size;
        }
    }

    T RemoveStart() {
        if (IsEmpty()) {
            throw std::underflow_error("List is empty");
        }

        T value = head->GetValue();

        if (size == 1) {
            delete head;
            head = nullptr;
        }
        else {
            TListNode* newHead = head->GetNext();
            TListNode* tail = head->GetPrev();

            tail->SetNext(newHead);
            newHead->SetPrev(tail);

            delete head;
            head = newHead;
        }
        --size;
        return value;
    }

    T RemoveEnd() {
        if (IsEmpty()) {
            throw std::underflow_error("List is empty");
        }

        if (size == 1) {
            return RemoveStart();
        }

        TListNode* tail = head->GetPrev();
        TListNode* newTail = tail->GetPrev();
        T value = tail->GetValue();

        newTail->SetNext(head);
        head->SetPrev(newTail);

        delete tail;
        --size;
        return value;
    }

    void Add(const T& value, int index) {
        if (index < 0 || index > static_cast<int>(size)) {
            throw std::out_of_range("Invalid index");
        }
        if (IsFull()) {
            throw std::overflow_error("List is full");
        }

        if (index == 0) {
            AddStart(value);
            return;
        }
        if (index == static_cast<int>(size)) {
            AddEnd(value);
            return;
        }

        TListNode* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->GetNext();
        }

        TListNode* newNode = new TListNode(value, current->GetPrev(), current);
        current->GetPrev()->SetNext(newNode);
        current->SetPrev(newNode);
        ++size;
    }

    T Remove(int index) {
        if (index < 0 || index >= static_cast<int>(size)) {
            throw std::out_of_range("Invalid index");
        }

        if (index == 0) {
            return RemoveStart();
        }
        if (index == static_cast<int>(size) - 1) {
            return RemoveEnd();
        }

        TListNode* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->GetNext();
        }

        T value = current->GetValue();

        TListNode* prevNode = current->GetPrev();
        TListNode* nextNode = current->GetNext();

        prevNode->SetNext(nextNode);
        nextNode->SetPrev(prevNode);

        delete current;
        --size;
        return value;
    }

    class Iterator {
    private:
        TListNode* current;
        TListNode* start;
        size_t position;
        size_t total;

    public:
        Iterator(TListNode* node = nullptr, size_t pos = 0, size_t totalSize = 0)
            : current(node), start(node), position(pos), total(totalSize) {
        }

        Iterator(const Iterator& other)
            : current(other.current), start(other.start), position(other.position), total(other.total) {
        }

        T& operator*() {
            if (!current) throw std::runtime_error("Dereferencing invalid iterator");
            return current->GetValueRef();
        }

        const T& operator*() const {
            if (!current) throw std::runtime_error("Dereferencing invalid iterator");
            return current->GetValue();
        }

        T* operator->() {
            if (!current) throw std::runtime_error("Accessing invalid iterator");
            return &current->GetValueRef();
        }

        const T* operator->() const {
            if (!current) throw std::runtime_error("Accessing invalid iterator");
            return &current->GetValue();
        }

        Iterator& operator++() {
            if (!current) throw std::runtime_error("Incrementing invalid iterator");
            current = current->GetNext();
            position = (position + 1) % total;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        Iterator& operator--() {
            if (!current) throw std::runtime_error("Decrementing invalid iterator");
            current = current->GetPrev();
            position = (position - 1 + total) % total;
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            --(*this);
            return temp;
        }

        bool operator==(const Iterator& other) const {
            return current == other.current && position == other.position;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

        TListNode* GetNode() const { return current; }
    };

    Iterator begin() {
        return Iterator(head, 0, size);
    }

    Iterator end() {
        return Iterator(head, size, size);
    }

    void insertAfter(Iterator it, const T& value) {
        if (IsFull()) {
            throw std::overflow_error("List is full");
        }

        if (it.GetNode() == nullptr) {
            AddEnd(value);
            return;
        }

        TListNode* node = it.GetNode();
        TListNode* nextNode = node->GetNext();
        TListNode* newNode = new TListNode(value, node, nextNode);

        node->SetNext(newNode);
        nextNode->SetPrev(newNode);
        ++size;
    }

    void remove(Iterator it) {
        if (IsEmpty()) {
            throw std::underflow_error("List is empty");
        }

        TListNode* node = it.GetNode();
        if (node == nullptr) {
            throw std::invalid_argument("Invalid iterator");
        }

        if (size == 1) {
            delete head;
            head = nullptr;
        }
        else {
            TListNode* prevNode = node->GetPrev();
            TListNode* nextNode = node->GetNext();

            prevNode->SetNext(nextNode);
            nextNode->SetPrev(prevNode);

            if (node == head) {
                head = nextNode;
            }

            delete node;
        }
        --size;
    }

    TList operator+(const TList& other) const {
        TList result(*this);

        if (result.size + other.size > MAX_SIZE) {
            throw std::overflow_error("Result would exceed maximum size");
        }

        if (other.size > 0) {
            TListNode* otherCurrent = other.head;
            for (size_t i = 0; i < other.size; ++i) {
                result.AddEnd(otherCurrent->GetValue());
                otherCurrent = otherCurrent->GetNext();
            }
        }
        return result;
    }

    bool operator==(const TList& other) const {
        if (size != other.size) return false;
        if (size == 0) return true;

        TListNode* thisCurrent = head;
        TListNode* otherCurrent = other.head;

        for (size_t i = 0; i < size; ++i) {
            if (thisCurrent->GetValue() != otherCurrent->GetValue()) {
                return false;
            }
            thisCurrent = thisCurrent->GetNext();
            otherCurrent = otherCurrent->GetNext();
        }
        return true;
    }

    bool operator!=(const TList& other) const {
        return !(*this == other);
    }

    T& operator[](int index) {
        if (index < 0 || index >= static_cast<int>(size)) {
            throw std::out_of_range("Index out of range");
        }

        TListNode* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->GetNext();
        }
        return current->GetValueRef();
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= static_cast<int>(size)) {
            throw std::out_of_range("Index out of range");
        }

        TListNode* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->GetNext();
        }
        return current->GetValue();
    }

    friend std::ostream& operator<<(std::ostream& os, const TList<T>& list) {
        if (list.IsEmpty()) {
            os << "[]";
            return os;
        }

        os << "[";
        typename TList<T>::TListNode* current = list.head;
        for (size_t i = 0; i < list.size; ++i) {
            os << current->GetValue();
            if (i < list.size - 1) {
                os << " <-> ";
            }
            current = current->GetNext();
        }
        os << "]";
        return os;
    }

    friend std::istream& operator>>(std::istream& is, TList<T>& list) {
        while (list.size > 0) {
            list.RemoveStart();
        }

        T value;
        while (is >> value) {
            if (list.IsFull()) {
                throw std::overflow_error("List is full during input");
            }
            list.AddEnd(value);
        }
        if (is.eof()) {
            is.clear();
        }
        return is;
    }
};