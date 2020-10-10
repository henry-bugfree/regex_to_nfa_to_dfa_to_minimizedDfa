#pragma once
#include<algorithm>
#include"main.h"
#include"nfa_to_dfa.h"
using namespace std;

class m_dfa
{
	public:
		m_dfa(void);
		~m_dfa(void);
	public:
		vector<vector<int> > row;//orignial node id will be stored in m_dfa_row[i][0]
		vector<int> start;
		vector<int> end;
};

int minimize_dfa(dfa_graph* dfa);
int initialize_tran_set_vector(dfa_graph* dfa, int row_size);
int minimize_step(int row_size, int* set_num, m_dfa* m_dfa_practice);
int print_m_dfa(m_dfa* m_dfa_practice);
int is_subject(int tran_to, m_dfa* m_dfa_practice);

