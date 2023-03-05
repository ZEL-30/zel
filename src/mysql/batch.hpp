#pragma once

#include "utility/value.h"

#include <mysql.h>
#include <vector>

namespace zel {
namespace mysql {

template <typename T>
class Batch {
  public:
    Batch();
    Batch(const Batch<T>& other) = delete;
    Batch(Batch<T>&& other) noexcept;
    ~Batch();

    Batch& operator=(const Batch<T>& other) = delete;
    Batch& operator=(Batch<T>&& other) noexcept;

    void size(int size);
    int size() const;

    void total(int total);
    int total() const;

    void result(MYSQL_RES* res);

    class iterator {
        friend class Batch<T>;

      public:
        iterator() : batch_(nullptr), next_(nullptr) {}

        iterator(Batch<T>* batch) : batch_(batch), next_(batch->next()) {}

        iterator(const iterator& other) = delete;

        iterator(iterator&& other) noexcept {
            batch_ = other.batch_;
            next_ = other.next_;
            other.next_ = nullptr;
        }

        ~iterator() {
            if (next_ != nullptr) {
                delete next_;
                next_ = nullptr;
            }
        }

        iterator& operator=(const iterator& other) = delete;
        iterator& operator=(iterator&& other) noexcept {
            batch_ = other.batch_;
            next_ = other.next_;
            other.next_ = nullptr;
            return *this;
        }

        bool operator!=(const iterator& other) { return next_ != other.next_; }

        iterator& operator++() // 前缀++
        {
            if (next_ != nullptr) {
                delete next_;
                next_ = nullptr;
            }
            next_ = batch_->next();
            return *this;
        }

        iterator operator++(int) // 后缀++
        {
            iterator it = std::move(*this);
            ++(*this);
            return it;
        }

        std::vector<T>& operator*() { return *next_; }

        std::vector<T>* operator->() { return next_; }

        typename std::vector<T>::iterator begin() { return next_->begin(); }

        typename std::vector<T>::iterator end() { return next_->end(); }

      private:
        Batch<T>* batch_;
        std::vector<T>* next_;
    };

    iterator begin();
    iterator end();

  private:
    std::vector<T>* next();

  private:
    MYSQL_RES* res_;
    int size_;
    int total_;
};

template <typename T>
Batch<T>::Batch() : res_(nullptr),
                    size_(0),
                    total_(0) {}

template <typename T>
Batch<T>::Batch(Batch<T>&& other) noexcept {
    res_ = other.res_;
    size_ = other.size_;
    total_ = other.total_;
    other.res_ = nullptr;
}

template <typename T>
Batch<T>::~Batch() {
    if (res_ != nullptr) {
        mysql_free_result(res_);
        res_ = nullptr;
    }
}

template <typename T>
Batch<T>& Batch<T>::operator=(Batch<T>&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    res_ = other.res_;
    size_ = other.size_;
    total_ = other.total_;
    other.res_ = nullptr;
    return *this;
}

template <typename T>
void Batch<T>::size(int size) {
    size_ = size;
}

template <typename T>
int Batch<T>::size() const {
    return size_;
}

template <typename T>
void Batch<T>::total(int total) {
    total_ = total;
}

template <typename T>
int Batch<T>::total() const {
    return total_;
}

template <typename T>
void Batch<T>::result(MYSQL_RES* res) {
    res_ = res;
}

template <typename T>
std::vector<T>* Batch<T>::next() {
    std::vector<T>* batch = nullptr;
    int fields = mysql_num_fields(res_);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res_)) != NULL) {
        if (batch == nullptr) {
            batch = new std::vector<T>();
        }
        T one;
        unsigned long* lengths = mysql_fetch_lengths(res_);
        for (int i = 0; i < fields; i++) {
            MYSQL_FIELD* field = mysql_fetch_field_direct(res_, i);
            char* data = row[i];
            unsigned int length = lengths[i];
            zel::utility::Value v;
            v = std::string(data, length);
            if (IS_NUM(field->type)) {
                if (length == 0) {
                    v.type(zel::utility::Value::V_NULL);
                } else {
                    if (field->type == MYSQL_TYPE_DECIMAL || field->type == MYSQL_TYPE_FLOAT ||
                        field->type == MYSQL_TYPE_DOUBLE) {
                        v.type(zel::utility::Value::V_DOUBLE);
                    } else {
                        v.type(zel::utility::Value::V_INT);
                    }
                }
            } else if (field->type == MYSQL_TYPE_NULL) {
                v.type(zel::utility::Value::V_NULL);
            } else {
                v.type(zel::utility::Value::V_STRING);
            }
            one[field->name] = v;
        }
        batch->push_back(one);
        if (batch->size() >= size_) {
            break;
        }
    }
    return batch;
}

template <typename T>
typename Batch<T>::iterator Batch<T>::begin() {
    return iterator(this);
}

template <typename T>
typename Batch<T>::iterator Batch<T>::end() {
    return iterator();
}

} // namespace mysql
} // namespace zel