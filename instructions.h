#pragma once

enum Instructions
{
    PUSH_I32 = 1,
    PUSH_STR = 2,
    ADD = 3,
    PRINT = 4,
    CONCAT = 5,
    SUB = 6,
    MUL = 7,
    DIV = 8,
    STORE = 9,
    GET = 10,
    EQUAL = 11,
    NOT_EQUAL = 12,
    JZ = 13,
    JNZ = 14,
    JMP = 15,
    EXIT = 16,
    IS_GT = 17,
};