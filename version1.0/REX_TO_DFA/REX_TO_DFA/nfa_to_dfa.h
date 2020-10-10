#pragma once
#include"graph.h"

class spt_struct
{
	public:
		int node_id;
		int set_id;
};

class new_node
{
	public:
		new_node(void);
		~new_node(void);
	public:
		vector<int> set;
		int id;
		vector<spt_struct> tran_set_vector;
		vector<int> subject;
};

class table_row
{
	public:
		table_row(void);
		~table_row(void);
	public:
		vector<new_node> row_vector;
};

class dfa_graph
{
	public:
		dfa_graph(void);
		~dfa_graph(void);
	public:
		vector<table_row> dfa_table;
		vector<int> end_vector;
		vector<int> not_end_vector;
		vector<int> start_vector;
};

int nfa_to_dfa(graph graph_nfa, dfa_graph* dfa);
int get_tran_char(char* rex, int rex_len);
int build_set(graph graph_nfa, int start, char tran);
int print_dfa(dfa_graph* dfa);
int is_node_exist(void);
int create_new_node(int* new_id);