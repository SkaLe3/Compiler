#ifndef SYMBOL_TABLES_H_
#define SYMBOL_TABLES_H_

#include <unordered_map>
#include <string>

extern std::unordered_map<std::string, uint32_t> ConstantsTable;
extern std::unordered_map<std::string, uint32_t> IdentifiersTable;
extern std::unordered_map<std::string, uint32_t> KeyWordsTable;

#endif // SYMBOL_TABLES_H_
