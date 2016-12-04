#pragma once

#define XC_DEBUG 1

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

#include "main.hpp"
#include "node.hpp"
#include "error.hpp"
// #include "parser.hpp"
#include "parser.m.hpp"
#include "lexer.hpp"

#define DELETE(ptr) \
{                   \
    delete ptr;     \
    ptr = nullptr;  \
}


using namespace std;

