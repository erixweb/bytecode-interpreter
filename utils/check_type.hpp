#pragma once // Prevents the file from being included twice
#include <variant>
#include <string>

// Just the declaration (the "signature")
bool checkInt(const std::variant<int, std::string>& v);
bool checkStr(const std::variant<int, std::string>& v);