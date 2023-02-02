/// @file ini_file.cpp
/// @author ZEL (zel1362848545@gmail.com)
/// @brief 解析配置文件
/// @version 0.1
/// @date 2023-01-30
/// @copyright Copyright (c) 2023 ZEL

#include "ini_file.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace zel {
namespace utility {

CIniFile::CIniFile() {}

CIniFile::~CIniFile() {}

bool CIniFile::Load(const std::string& filename) {

    std::ifstream fin(filename);

    if (fin.fail())
        return false;

    std::string buffer, section;
    int pos = 0;
    while (std::getline(fin, buffer)) {
        buffer = Trim(buffer);
        if (buffer == "")
            continue;
        if (buffer[0] == '[') {
            pos = buffer.find_first_of(']');
            section = buffer.substr(1, pos - 1);
            section = Trim(section);
            m_sections_[section] = Section();
        } else {
            pos = buffer.find_first_of('=');
            std::string key = buffer.substr(0, pos);
            std::string value = buffer.substr(pos + 1, buffer.length() - pos);
            key = Trim(key);
            value = Trim(value);
            m_sections_[section][key] = value;
        }
    }

    fin.close();

    return true;
}

std::string CIniFile::Str() {
    std::stringstream ss;
    for (auto it = m_sections_.begin(); it != m_sections_.end(); it++) {
        ss << "[" << it->first << "]" << std::endl;
        for (auto iter = it->second.begin(); iter != it->second.end(); iter++) {
            ss << iter->first << " = " << std::string(iter->second) << std::endl;
        }
        ss << std::endl;
    }

    return ss.str();
}

bool CIniFile::Save(const std::string& filename) {
    std::ofstream fout(filename);

    if (fout.fail())
        return false;

    fout << Str();
    fout.close();

    return true;
}

void CIniFile::Show() { std::cout << Str(); }

std::string CIniFile::Trim(std::string s) {
    if (s.empty())
        return s;

    s.erase(0, s.find_first_not_of(" \n\r"));
    s.erase(s.find_last_not_of(" \n\r") + 1);

    return s;
}

CValue& CIniFile::Get(const std::string& section, const std::string& key) {
    return m_sections_[section][key];
}

void CIniFile::Set(const std::string& section, const std::string& key, const CValue& value) {
    m_sections_[section][key] = value;
}

bool CIniFile::Has(const std::string& section) {
    return (m_sections_.find(section) != m_sections_.end());
}

bool CIniFile::Has(const std::string& section, const std::string& key) {
    auto iter = m_sections_.find(section);
    if (iter != m_sections_.end()) {
        return (iter->second.find(key) != iter->second.end());
    }
    return false;
}

void CIniFile::Remove(const std::string& section) { m_sections_.erase(section); }

void CIniFile::Remove(const std::string& section, const std::string& key) {
    auto iter = m_sections_.find(section);
    if (iter != m_sections_.end()) {
        iter->second.erase(key);
    }
}

void CIniFile::Clear() { m_sections_.clear(); }

Section& CIniFile::operator[](const std::string& section) { return m_sections_[section]; }

} // namespace utility
} // namespace zel
