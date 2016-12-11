#pragma once

#define YYDEBUG 0

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "main.hpp"
#include "node.hpp"
#include "inter.hpp"
#include "error.hpp"
#include "parser.m.hpp"
#include "lexer.hpp"

#define DELETE(ptr) \
{                   \
    delete ptr;     \
    ptr = nullptr;  \
}

using namespace std;
