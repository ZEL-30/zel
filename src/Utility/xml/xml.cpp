#include "xml.h"

#include <cstddef>
#include <list>
#include <sstream>

namespace zel {

namespace utility {

Xml::Xml() : name_(nullptr), text_(nullptr), attrs_(nullptr), child_(nullptr) {}

Xml::Xml(const char* name) : name_(nullptr), text_(nullptr), attrs_(nullptr), child_(nullptr) {
    name_ = new std::string(name);
}

Xml::Xml(const std::string& name)
    : name_(nullptr),
      text_(nullptr),
      attrs_(nullptr),
      child_(nullptr) {
    name_ = new std::string(name);
}

Xml::Xml(const Xml& other) {
    name_ = other.name_;
    text_ = other.text_;
    attrs_ = other.attrs_;
    child_ = other.child_;
}

Xml& Xml::operator=(const Xml& other) {
    Clear();

    name_ = other.name_;
    text_ = other.text_;
    attrs_ = other.attrs_;
    child_ = other.child_;

    return *this;
}

Xml::~Xml() {}

std::string Xml::GetName() const {
    if (name_ == nullptr)
        return "";
    return *name_;
}

void Xml::SetName(const std::string& name) {
    if (name_ != nullptr) {
        delete name_;
        name_ = nullptr;
    }

    name_ = new std::string(name);
}

std::string Xml::GetText() const {
    if (text_ == nullptr)
        return "";
    return *text_;
}

void Xml::SetText(const std::string& text) {
    if (text_ != nullptr) {
        delete text_;
        text_ = nullptr;
    }

    text_ = new std::string(text);
}

Value& Xml::GetAttr(const std::string& key) {
    if (attrs_ == nullptr)
        attrs_ = new std::map<std::string, Value>();

    return (*attrs_)[key];
}
void Xml::SetAttr(const std::string& key, const Value& value) {
    if (attrs_ == nullptr)
        attrs_ = new std::map<std::string, Value>();

    (*attrs_)[key] = value;
}

std::string Xml::AsString() {
    if (name_ == nullptr)
        throw std::logic_error("element name is empty");

    std::stringstream ss;
    ss << "<" << *name_;

    if (attrs_ != nullptr) {
        for (auto it = attrs_->begin(); it != attrs_->end(); it++) {
            ss << " " << it->first << "=\"" << std::string(it->second) << "\"";
        }
    }
    ss << ">";

    if (child_ != nullptr) {
        for (auto it = child_->begin(); it != child_->end(); it++) {
            ss << it->AsString();
        }
    }

    if (text_ != nullptr) {
        ss << *text_;
    }

    ss << "</" << *name_ << ">";

    return ss.str();
}

void Xml::Clear() {
    if (name_ != nullptr) {
        delete name_;
        name_ = nullptr;
    }

    if (text_ != nullptr) {
        delete text_;
        text_ = nullptr;
    }

    if (attrs_ != nullptr) {
        delete attrs_;
        attrs_ = nullptr;
    }

    if (child_ != nullptr) {
        for (auto it = child_->begin(); it != child_->end(); it++) {
            it->Clear();
        }
        delete child_;
        child_ = nullptr;
    }
}

void Xml::Append(const Xml& child) {
    if (child_ == nullptr) {
        child_ = new std::list<Xml>();
    }

    child_->push_back(child);
}

Xml& Xml::operator[](int index) {
    if (index < 0)
        throw std::logic_error("index less than zero");

    if (child_ == nullptr) {
        child_ = new std::list<Xml>();
    }

    int size = child_->size();
    if (index >= 0 && index < size) {
        auto it = child_->begin();
        for (int i = 0; i < index; i++) {
            it++;
        }
        return *it;
    } else {
        // list 需要扩容
        for (int i = size; i <= index; i++) {
            child_->push_back(Xml());
        }
    }

    return child_->back();
}

Xml& Xml::operator[](const char* name) { return (*this)[std::string(name)]; }
Xml& Xml::operator[](const std::string& name) {
    if (child_ == nullptr)
        child_ = new std::list<Xml>();

    for (auto it = child_->begin(); it != child_->end(); it++) {
        if (it->GetName() == name) {
            return *it;
        }
    }

    child_->push_back(Xml(name));
    return child_->back();
}

void Xml::Remove(int index) {
    if (child_ == nullptr)
        return;

    int size = child_->size();
    if (index < 0 || index >= size)
        return;

    auto it = child_->begin();
    for (int i = 0; i < index; i++) {
        it++;
    }

    it->Clear();
    child_->erase(it);
}

void Xml::Remove(const char* name) { Remove(std::string(name)); }

void Xml::Remove(const std::string& name) {
    if (child_ == nullptr)
        return;

    // 边删除边操作
    for (auto it = child_->begin(); it != child_->end();) {
        if (it->GetName() == name) {
            it->Clear();
            it = child_->erase(it);
        } else {
            it++;
        }
    }
}

std::list<Xml>::iterator Xml::begin() { return child_->begin(); }

std::list<Xml>::iterator Xml::end() { return child_->end(); }

std::list<Xml>::iterator Xml::erase(std::list<Xml>::iterator it) {
    it->Clear();
    return child_->erase(it);
}

int Xml::size() const { return child_->size(); }

} // namespace utility

} // namespace zel