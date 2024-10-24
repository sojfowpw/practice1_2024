#pragma once
#include <iostream>
#include "insert.h"
#include "delete.h"

using namespace std;

void splitDot(const string& word, string& table, string& column, tableJson& tjs);
void select(const string& command, tableJson& tjs);