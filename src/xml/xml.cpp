#include "xml.h"

#include "parser.h"
#include "xml/parser.h"

#include <cstddef>
#include <fstream>
#include <list>
#include <sstream>

namespace zel {
namespace xml {

Xml::Xml() : name_(nullptr), text_(nullptr), attrs_(nullptr), child_(nullptr) {}

Xml::Xml(const char* name)
    : name_(nullptr),
      text_(nullptr),
      attrs_(nullptr),
      child_(nullptr) {
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

std::string Xml::name() const {
    if (name_ == nullptr)
        return "";
    return *name_;
}

void Xml::name(const std::string& name) {
    if (name_ != nullptr) {
        delete name_;
        name_ = nullptr;
    }

    name_ = new std::string(name);
}

std::string Xml::text() const {
    if (text_ == nullptr)
        return "";
    return *text_;
}

void Xml::text(const std::string& text) {
    if (text_ != nullptr) {
        delete text_;
        text_ = nullptr;
    }

    text_ = new std::string(text);
}

zel::utility::Value& Xml::attr(const std::string& key) {
    if (attrs_ == nullptr)
        attrs_ = new std::map<std::string, zel::utility::Value>();

    return (*attrs_)[key];
}
void Xml::attr(const std::string& key, const zel::utility::Value& value) {
    if (attrs_ == nullptr)
        attrs_ = new std::map<std::string, zel::utility::Value>();

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
        if (it->name() == name) {
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
        if (it->name() == name) {
            it->Clear();
            it = child_->erase(it);
        } else {
            it++;
        }
    }
}

bool Xml::Load(const std::string& filename) {
    Parser parser;
    if (!parser.LoadFile(filename))
        return false;

    *this = parser.Parse();
    return true;
}

bool Xml::Save(const std::string& filename) {
    std::ofstream fout(filename);

    if (fout.fail())
        return false;

    fout << AsString();

    fout.close();

    return true;
}

bool Xml::Parse(const std::string& str) {
    Parser parser;
    if (!parser.LoadString(str))
        return false;

    *this = parser.Parse();

    return true;
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