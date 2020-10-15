#pragma once
#include<vector>
#include<cstdio>
#include<cstring>
#include<iostream>
#include<cstdbool>
#include<stack>
#include"graph.h"

using namespace std;

int solve(int i);
int reset(void);

class driver
{
public:
	vector<char> rpn_vector;
	stack<char> rpn_op_stack;
	vector<char> tran_char_vector;
	vector<int> set_search_vector;
	stack<int> set_search_stack;
	vector<int> better_prt_vector;
	vector<int> m_set;
	vector<int> m_set1;
	vector<int> m_set2;
	vector<vector<int> > spt_set_vector;
};