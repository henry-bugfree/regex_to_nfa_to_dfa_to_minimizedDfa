#pragma once
#include"main.h"

int to_rpn(char* rex, int rex_len, driver* driver_prac);
void print_rnp(driver* driver_prac);//for the use of debugging
bool is_operator(char ch);
bool is_and(char* rex, int rex_len, int i);
bool rpn_check(int check_num);

using namespace std;