#include "xml.h"

#include <cstddef>
#include <list>
#include <sstream>

namespace zel {

namespace utility {

CXml::CXml() : name_(nullptr), text_(nullptr), attrs_(nullptr), child_(nullptr) {}

CXml::CXml(const char* name) : name_(nullptr), text_(nullptr), attrs_(nullptr), child_(nullptr) {
    name_ = new std::string(name);
}

CXml::CXml(const std::string& name)
    : name_(nullptr),
      text_(nullptr),
      attrs_(nullptr),
      child_(nullptr) {
    name_ = new std::string(name);
}

CXml::CXml(const CXml& other) {
    name_ = other.name_;
    text_ = other.text_;
    attrs_ = other.attrs_;
    child_ = other.child_;
}

CXml& CXml::operator=(const CXml& other) {
    Clear();

    name_ = other.name_;
    text_ = other.text_;
    attrs_ = other.attrs_;
    child_ = other.child_;

    return *this;
}

CXml::~CXml() {}

std::string CXml::GetName() const {
    if (name_ == nullptr)
        return "";
    return *name_;
}

void CXml::SetName(const std::string& name) {
    if (name_ != nullptr) {
        delete name_;
        name_ = nullptr;
    }

    name_ = new std::string(name);
}

std::string CXml::GetText() const {
    if (text_ == nullptr)
        return "";
    return *text_;
}

void CXml::SetText(const std::string& text) {
    if (text_ != nullptr) {
        delete text_;
        text_ = nullptr;
    }

    text_ = new std::string(text);
}

CValue& CXml::GetAttr(const std::string& key) {
    if (attrs_ == nullptr)
        attrs_ = new std::map<std::string, CValue>();

    return (*attrs_)[key];
}
void CXml::SetAttr(const std::string& key, const CValue& value) {
    if (attrs_ == nullptr)
        attrs_ = new std::map<std::string, CValue>();

    (*attrs_)[key] = value;
}

std::string CXml::AsString() {
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

void CXml::Clear() {
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

void CXml::Append(const CXml& child) {
    if (child_ == nullptr) {
        child_ = new std::list<CXml>();
    }

    child_->push_back(child);
}

CXml& CXml::operator[](int index) {
    if (index < 0)
        throw std::logic_error("index less than zero");

    if (child_ == nullptr) {
        child_ = new std::list<CXml>();
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
            child_->push_back(CXml());
        }
    }

    return child_->back();
}

CXml& CXml::operator[](const char* name) { return (*this)[std::string(name)]; }
CXml& CXml::operator[](const std::string& name) {
    if (child_ == nullptr)
        child_ = new std::list<CXml>();

    for (auto it = child_->begin(); it != child_->end(); it++) {
        if (it->GetName() == name) {
            return *it;
        }
    }

    child_->push_back(CXml(name));
    return child_->back();
}

void CXml::Remove(int index) {
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

void CXml::Remove(const char* name) { Remove(std::string(name)); }

void CXml::Remove(const std::string& name) {
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

std::list<CXml>::iterator CXml::begin() { return child_->begin(); }

std::list<CXml>::iterator CXml::end() { return child_->end(); }

std::list<CXml>::iterator CXml::erase(std::list<CXml>::iterator it) {
    it->Clear();
    return child_->erase(it);
}

int CXml::size() const { return child_->size(); }

} // namespace utility

} // namespace zel