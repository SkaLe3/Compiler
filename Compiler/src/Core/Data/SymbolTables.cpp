#include "SymbolTables.h"

std::unordered_map<std::string, uint32_t> ConstantsTable;
std::unordered_map<std::string, uint32_t> IdentifiersTable;
std::unordered_map<std::string, uint32_t> KeyWordsTable;

std::unordered_map<uint32_t, std::string> Reverse_ConstantsTable;
std::unordered_map<uint32_t, std::string> Reverse_IdentifiersTable;
std::unordered_map<uint32_t, std::string> Reverse_KeyWordsTable;