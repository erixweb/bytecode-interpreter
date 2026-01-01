#include <variant>
#include <string>
#include "./check_type.hpp"

bool checkInt(const std::variant<int, std::string> &a)
{
    return std::holds_alternative<int>(a);
}
bool checkStr(const std::variant<int, std::string> &a)
{
    return std::holds_alternative<std::string>(a);
}