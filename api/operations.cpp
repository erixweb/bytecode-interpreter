#include <vector>
#include <iostream>
#include <variant>
#include <string>
#include "../utils/check_type.hpp" 
#include "operations.hpp"

void add(std::vector<std::variant<int, std::string>>& stack) 
{
    if (stack.size() < 2) {
        std::cerr << "Error: ADD requires two operands." << std::endl;
        return;
    }

    auto& op1 = stack[stack.size() - 2];
    auto& op2 = stack[stack.size() - 1];

    if (checkInt(op1) && checkInt(op2)) {
        int a = std::get<int>(op1);
        int b = std::get<int>(op2);

        stack.pop_back();
        stack.back() = a + b; 
    } else {
        std::cerr << "Error: ADD expects integer operands." << std::endl;
    }
}
void sub(std::vector<std::variant<int, std::string>>& stack) 
{
    if (stack.size() < 2) {
        std::cerr << "Error: SUB requires two operands." << std::endl;
        return;
    }

    auto& op1 = stack[stack.size() - 2];
    auto& op2 = stack[stack.size() - 1];

    if (checkInt(op1) && checkInt(op2)) {
        int a = std::get<int>(op1);
        int b = std::get<int>(op2);

        stack.pop_back();
        stack.back() = a - b;
    } else {
        std::cerr << "Error: SUB expects integer operands." << std::endl;
    }

}
void mul(std::vector<std::variant<int, std::string>>& stack) 
{
    if (stack.size() < 2) {
        std::cerr << "Error: MUL requires two operands." << std::endl;
        return;
    }
    auto& op1 = stack[stack.size() - 2];
    auto& op2 = stack[stack.size() - 1];
    if (checkInt(op1) && checkInt(op2)) {
        int a = std::get<int>(op1);
        int b = std::get<int>(op2);
        stack.pop_back();
        stack.back() = a * b;
    } else {
        std::cerr << "Error: MUL expects integer operands." << std::endl;
    }
}
void div(std::vector<std::variant<int, std::string>>& stack) 
{
    if (stack.size() < 2) {
        std::cerr << "Error: DIV requires two operands." << std::endl;
        return;
    }
    auto& op1 = stack[stack.size() - 2];
    auto& op2 = stack[stack.size() - 1];
    if (checkInt(op1) && checkInt(op2)) {
        int a = std::get<int>(op1);
        int b = std::get<int>(op2);
        if (b != 0) {
            stack.pop_back();
            stack.back() = a / b;
        } else {
            std::cerr << "Error: DIV by zero." << std::endl;
        }
    } else {
        std::cerr << "Error: DIV expects integer operands." << std::endl;
    }
}