#pragma once

#include <unordered_map>
#include <string>

extern std::unordered_map<std::string, uint32_t> ConstantsTable;
extern std::unordered_map<std::string, uint32_t> IdentifiersTable;
extern std::unordered_map<std::string, uint32_t> KeyWordsTable;

extern std::unordered_map<uint32_t, std::string> Reverse_ConstantsTable;
extern std::unordered_map<uint32_t, std::string> Reverse_IdentifiersTable;
extern std::unordered_map<uint32_t, std::string> Reverse_KeyWordsTable;

