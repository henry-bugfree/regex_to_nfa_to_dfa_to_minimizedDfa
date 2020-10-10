#include"nfa_to_dfa.h"
#include"main.h"
#include"rex_to_rpn.h"
#include<algorithm>
using namespace std;

vector<char> tran_char_vector;
vector<int> set_search_vector;
stack<int> set_search_stack;
vector<new_node> new_node_vector_spt;
vector<new_node> new_node_vector;
vector<int> better_prt_vector;

new_node::new_node(void)
{
}
new_node::~new_node(void)
{
}
table_row::table_row(void)
{
}
table_row::~table_row(void)
{
}
dfa_graph::dfa_graph(void)
{
}
dfa_graph::~dfa_graph(void)
{
}

int nfa_to_dfa(graph graph_nfa, dfa_graph* dfa)
{
	int new_id = 0;
	int start = graph_nfa.start;
	build_set(graph_nfa, start, 'E');
	set_search_vector.push_back(start);//手动增加初始点
	
	vector<int>::iterator result1 = find(set_search_vector.begin(), set_search_vector.end(), graph_nfa.start);
	if (result1 != set_search_vector.end())//is start node?
		dfa->start_vector.push_back(new_id);

	vector<int>::iterator result2 = find(set_search_vector.begin(), set_search_vector.end(), graph_nfa.end);
	if (result2 != set_search_vector.end())//is end node?
		dfa->end_vector.push_back(new_id);
	else dfa->not_end_vector.push_back(new_id);

	create_new_node(&new_id);

	while (!new_node_vector_spt.empty())
	{
		table_row new_row;

		new_node temp_node = new_node_vector_spt.front();
		new_node_vector_spt.erase(new_node_vector_spt.begin());

		for (int i = 0; i < tran_char_vector.size(); i++)
		{
			set_search_vector.clear();
			while (!set_search_stack.empty())
				set_search_stack.pop();

			for (int j = 0; j < temp_node.set.size(); j++)
			{
				int temp_arc = graph_nfa.node_list[temp_node.set[j]].first_arc;
				while (temp_arc != -1)
				{
					if (graph_nfa.arc_list[temp_arc].value == tran_char_vector[i])
					{
						int can_move = graph_nfa.arc_list[temp_arc].adjvex;

						vector<int>::iterator result = find(set_search_vector.begin(), set_search_vector.end(), can_move);
						if (result == set_search_vector.end())
							set_search_vector.push_back(can_move);
					}
					temp_arc = graph_nfa.arc_list[temp_arc].next_arc;
				}
			}

			for (int k = 0; k < set_search_vector.size(); k++)
			{
				start = set_search_vector[k];
				build_set(graph_nfa, start, 'E');
			}

			int temp_id = is_node_exist();
			if (temp_id == -2) return -2;
			if (temp_id == -1)
			{
				vector<int>::iterator result1 = find(set_search_vector.begin(), set_search_vector.end(), graph_nfa.start);
				if (result1 != set_search_vector.end())//is start node?
					dfa->start_vector.push_back(new_id);

				vector<int>::iterator result2 = find(set_search_vector.begin(), set_search_vector.end(), graph_nfa.end);
				if (result2 != set_search_vector.end())//is end node?
					dfa->end_vector.push_back(new_id);
				else dfa->not_end_vector.push_back(new_id);

				create_new_node(&new_id);
				new_row.row_vector.push_back(new_node_vector.back());
			}
			else
			{
				new_row.row_vector.push_back(new_node_vector[temp_id]);
			}
		}
		dfa->dfa_table.push_back(new_row);
	}

	return 0;
}

int get_tran_char(char* rex, int rex_len)
{
	for (int i = 0; i < rex_len; i++)
	{
		if (!is_operator(rex[i]))
		{
			vector<char>::iterator result = find(tran_char_vector.begin(), tran_char_vector.end(), rex[i]);
			if (result == tran_char_vector.end()) tran_char_vector.push_back(rex[i]);
		}
	}
	return 0;
}

int build_set(graph graph_nfa, int start, char tran)//递归实现广度优先搜索
{
	int temp_arc = graph_nfa.node_list[start].first_arc;
	while (temp_arc != -1)
	{
		if (graph_nfa.arc_list[temp_arc].value == tran)
		{
			int can_move = graph_nfa.arc_list[temp_arc].adjvex;

			vector<int>::iterator result = find(set_search_vector.begin(), set_search_vector.end(), can_move);
			if (result == set_search_vector.end())
			{
				set_search_stack.push(can_move);
				set_search_vector.push_back(can_move);
			}
		}
		temp_arc = graph_nfa.arc_list[temp_arc].next_arc;
	}
	if (!set_search_stack.empty())
	{
		start = set_search_stack.top();
		set_search_stack.pop();
		build_set(graph_nfa, start, tran);
	}
	return 0;
}

int print_dfa(dfa_graph* dfa)
{
	cout << "dfa table: ";
	cout << endl << "E" << " ";
	for (int i = 0; i < tran_char_vector.size(); i++)
	{
		cout << tran_char_vector[i] << " ";
	}
	cout << endl;

	for (int i = 0; i < new_node_vector.size(); i++)
	{
		cout << i << " ";
		cout << "{";

		better_prt_vector = new_node_vector[i].set;
		sort(better_prt_vector.begin(), better_prt_vector.end());

		for (int k = 0; k < better_prt_vector.size(); k++)
			cout << better_prt_vector[k] << ",";
		cout << "}" << "; ";
		for (int j = 0;j < dfa->dfa_table[i].row_vector.size();j++)
		{
			cout << dfa->dfa_table[i].row_vector[j].id << " ";
			cout << "{";

			better_prt_vector = dfa->dfa_table[i].row_vector[j].set;
			sort(better_prt_vector.begin(), better_prt_vector.end());

			for(int k=0;k< better_prt_vector.size();k++)
				cout << better_prt_vector[k] << ",";
			cout << "}" << "; ";
		}
		cout << endl;
	}

	cout << "start node id: ";
	for (int i = 0; i < dfa->start_vector.size(); i++)
		cout << dfa->start_vector[i] << " ";
	cout << endl;

	cout << "end node id: ";
	for (int i = 0; i < dfa->end_vector.size(); i++)
		cout << dfa->end_vector[i] << " ";

	return 0;
}

int is_node_exist(void)
{
	//bug warning(??): 如果set_search_vector.size()==0时，会return 0;
	if (set_search_vector.size() == 0)
	{
		cout << "there is a node which can't reach any node!";
		cout << endl << "this may refer to that it can't be converted to a dfa!" << endl;
		return -2;
	}
		
	int j;
	for (int i = 0; i < new_node_vector.size(); i++)
	{
		for (j = 0; j < set_search_vector.size(); j++)
		{
			if (new_node_vector[i].set.size() != set_search_vector.size()) break;
			if (new_node_vector[i].set[j] != set_search_vector[j]) break;
		}
		if (j == set_search_vector.size()) return i;
	}
	return -1;
}

int create_new_node(int *new_id)
{
	new_node node;
	node.id = *new_id;
	*new_id = *new_id+1;
	node.set = set_search_vector;
	new_node_vector_spt.push_back(node);
	new_node_vector.push_back(node);
	return 0;
}
