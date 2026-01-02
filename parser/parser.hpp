#pragma once
#include <variant>
#include <string>
#include <vector>


std::vector<std::variant<int, std::string>> fileToVector(const std::string& line);
std::variant<int, std::string> parse_token(std::string token);
std::string trim(std::string s);
std::string loadFile(std::string fileName);

