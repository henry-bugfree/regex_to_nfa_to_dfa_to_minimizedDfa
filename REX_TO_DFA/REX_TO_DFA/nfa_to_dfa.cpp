#include"nfa_to_dfa.h"
#include"main.h"
#include"rex_to_rpn.h"
#include<algorithm>
using namespace std;

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
vector<new_node> new_node_vector_spt;
vector<new_node> new_node_vector;
int nfa_to_dfa(graph graph_nfa, dfa_graph* dfa, driver* driver_prac)
{
	int new_id = 0;
	int start = graph_nfa.start;
	build_set(graph_nfa, start, 'E', driver_prac);
	driver_prac->set_search_vector.push_back(start);//手动增加初始点
	
	vector<int>::iterator result1 = find(driver_prac->set_search_vector.begin(), driver_prac->set_search_vector.end(), graph_nfa.start);
	if (result1 != driver_prac->set_search_vector.end())//is start node?
		dfa->start_vector.push_back(new_id);

	vector<int>::iterator result2 = find(driver_prac->set_search_vector.begin(), driver_prac->set_search_vector.end(), graph_nfa.end);
	if (result2 != driver_prac->set_search_vector.end())//is end node?
		dfa->end_vector.push_back(new_id);
	else dfa->not_end_vector.push_back(new_id);

	create_new_node(&new_id, driver_prac);

	while (!new_node_vector_spt.empty())
	{
		table_row new_row;

		new_node temp_node = new_node_vector_spt.front();
		new_node_vector_spt.erase(new_node_vector_spt.begin());

		for (int i = 0; i < driver_prac->tran_char_vector.size(); i++)
		{
			driver_prac->set_search_vector.clear();
			while (!driver_prac->set_search_stack.empty())
				driver_prac->set_search_stack.pop();

			for (int j = 0; j < temp_node.set.size(); j++)
			{
				int temp_arc = graph_nfa.node_list[temp_node.set[j]].first_arc;
				while (temp_arc != -1)
				{
					if (graph_nfa.arc_list[temp_arc].value == driver_prac->tran_char_vector[i])
					{
						int can_move = graph_nfa.arc_list[temp_arc].adjvex;

						vector<int>::iterator result = find(driver_prac->set_search_vector.begin(), driver_prac->set_search_vector.end(), can_move);
						if (result == driver_prac->set_search_vector.end())
							driver_prac->set_search_vector.push_back(can_move);
					}
					temp_arc = graph_nfa.arc_list[temp_arc].next_arc;
				}
			}

			for (int k = 0; k < driver_prac->set_search_vector.size(); k++)
			{
				start = driver_prac->set_search_vector[k];
				build_set(graph_nfa, start, 'E', driver_prac);
			}

			int temp_id = is_node_exist(driver_prac);
			if (temp_id == -2) return -2;
			if (temp_id == -1)
			{
				vector<int>::iterator result1 = find(driver_prac->set_search_vector.begin(), driver_prac->set_search_vector.end(), graph_nfa.start);
				if (result1 != driver_prac->set_search_vector.end())//is start node?
					dfa->start_vector.push_back(new_id);

				vector<int>::iterator result2 = find(driver_prac->set_search_vector.begin(), driver_prac->set_search_vector.end(), graph_nfa.end);
				if (result2 != driver_prac->set_search_vector.end())//is end node?
					dfa->end_vector.push_back(new_id);
				else dfa->not_end_vector.push_back(new_id);

				create_new_node(&new_id, driver_prac);
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

int get_tran_char(char* rex, int rex_len, driver* driver_prac)
{
	for (int i = 0; i < rex_len; i++)
	{
		if (!is_operator(rex[i]))
		{
			vector<char>::iterator result = find(driver_prac->tran_char_vector.begin(), driver_prac->tran_char_vector.end(), rex[i]);
			if (result == driver_prac->tran_char_vector.end()) driver_prac->tran_char_vector.push_back(rex[i]);
		}
	}
	return 0;
}

int build_set(graph graph_nfa, int start, char tran, driver* driver_prac)//递归实现广度优先搜索
{
	int temp_arc = graph_nfa.node_list[start].first_arc;
	while (temp_arc != -1)
	{
		if (graph_nfa.arc_list[temp_arc].value == tran)
		{
			int can_move = graph_nfa.arc_list[temp_arc].adjvex;

			vector<int>::iterator result = find(driver_prac->set_search_vector.begin(), driver_prac->set_search_vector.end(), can_move);
			if (result == driver_prac->set_search_vector.end())
			{
				driver_prac->set_search_stack.push(can_move);
				driver_prac->set_search_vector.push_back(can_move);
			}
		}
		temp_arc = graph_nfa.arc_list[temp_arc].next_arc;
	}
	if (!driver_prac->set_search_stack.empty())
	{
		start = driver_prac->set_search_stack.top();
		driver_prac->set_search_stack.pop();
		build_set(graph_nfa, start, tran, driver_prac);
	}
	return 0;
}

int print_dfa(dfa_graph* dfa, driver* driver_prac)
{
	cout << "dfa table: ";
	cout << endl << "E" << " ";
	for (int i = 0; i < driver_prac->tran_char_vector.size(); i++)
	{
		cout << driver_prac->tran_char_vector[i] << " ";
	}
	cout << endl;

	for (int i = 0; i < new_node_vector.size(); i++)
	{
		cout << i << " ";
		cout << "{";

		driver_prac->better_prt_vector = new_node_vector[i].set;
		sort(driver_prac->better_prt_vector.begin(), driver_prac->better_prt_vector.end());

		for (int k = 0; k < driver_prac->better_prt_vector.size(); k++)
			cout << driver_prac->better_prt_vector[k] << ",";
		cout << "}" << "; ";
		for (int j = 0;j < dfa->dfa_table[i].row_vector.size();j++)
		{
			cout << dfa->dfa_table[i].row_vector[j].id << " ";
			cout << "{";

			driver_prac->better_prt_vector = dfa->dfa_table[i].row_vector[j].set;
			sort(driver_prac->better_prt_vector.begin(), driver_prac->better_prt_vector.end());

			for(int k=0;k< driver_prac->better_prt_vector.size();k++)
				cout << driver_prac->better_prt_vector[k] << ",";
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

int is_node_exist(driver* driver_prac)
{
	//bug warning(??): 如果set_search_vector.size()==0时，会return 0;
	if (driver_prac->set_search_vector.size() == 0)
	{
		cout << "there is a node which can't reach any node!";
		cout << endl << "this may refer to that it can't be converted to a dfa!" << endl;
		return -2;
	}
		
	int j;
	for (int i = 0; i < new_node_vector.size(); i++)
	{
		for (j = 0; j < driver_prac->set_search_vector.size(); j++)
		{
			if (new_node_vector[i].set.size() != driver_prac->set_search_vector.size()) break;
			if (new_node_vector[i].set[j] != driver_prac->set_search_vector[j]) break;
		}
		if (j == driver_prac->set_search_vector.size()) return i;
	}
	return -1;
}

int create_new_node(int *new_id, driver* driver_prac)
{
	new_node node;
	node.id = *new_id;
	*new_id = *new_id+1;
	node.set = driver_prac->set_search_vector;
	new_node_vector_spt.push_back(node);
	new_node_vector.push_back(node);
	return 0;
}
