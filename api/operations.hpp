#pragma once // Prevents the file from being included twice
#include <variant>
#include <string>
#include <vector>

// Just the declaration (the "signature")
void add(std::vector<std::variant<int, std::string>> &stack);
void sub(std::vector<std::variant<int, std::string>> &stack);
void mul(std::vector<std::variant<int, std::string>> &stack);
void div(std::vector<std::variant<int, std::string>> &stack);