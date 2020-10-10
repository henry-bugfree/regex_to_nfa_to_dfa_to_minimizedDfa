#pragma once
int to_rpn(char* rex, int rex_len);
void print_rnp(void);//for the use of debugging
bool is_operator(char ch);
bool is_and(char* rex, int rex_len, int i);
bool rpn_check(int check_num);

using namespace std;