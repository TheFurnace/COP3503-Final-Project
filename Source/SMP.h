#pragma once
#include "Index.h"
#include <iostream>
#include <string>

using namespace std;

char* getCmdOption(char ** begin, char ** end, const std::string & option);
bool cmdOptionExists(char** begin, char** end, const std::string& option);
int startTBI();
bool isInteger(const std::string & s);
void BadInput();