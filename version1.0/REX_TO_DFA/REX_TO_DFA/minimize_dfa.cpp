#include<algorithm>
#include"main.h"
#include"nfa_to_dfa.h"
#include"minimize_dfa.h"
using namespace std;

vector<vector<int> > spt_set_vector;
vector<int> m_set;
vector<int> m_set1;
vector<int> m_set2;
extern vector<new_node> new_node_vector;
extern vector<char> tran_char_vector;

m_dfa::m_dfa(void)
{
}
m_dfa::~m_dfa(void)
{
}

int minimize_dfa(dfa_graph* dfa)
{
	m_dfa m_dfa_practice;

	int row_size = tran_char_vector.size();
	initialize_tran_set_vector(dfa, row_size);
	int set_num = 2;

	if (dfa->end_vector.size() == 0 || dfa->not_end_vector.size() == 0)
	{
		cout << endl << endl << "this dfa is natually minimized!" << endl;
		return 1;
	}

	spt_set_vector.push_back(dfa->end_vector);
	spt_set_vector.push_back(dfa->not_end_vector);

	while (!spt_set_vector.empty())
	{
		m_set = spt_set_vector.back();
		spt_set_vector.pop_back();
		minimize_step(row_size, &set_num, &m_dfa_practice);
	}

	//更新表
	for (int i = 0; i < m_dfa_practice.row.size(); i++)
	{
		for (int j = 1; j < row_size + 1; j++)
		{
			int result = is_subject(m_dfa_practice.row[i][j], &m_dfa_practice);
			if (result != -1)
			{
				m_dfa_practice.row[i][j] = result;
			}
		}
	}

	for (int i = 0; i < m_dfa_practice.row.size(); i++)
	{
		vector<int>::iterator result1 = find(dfa->start_vector.begin(), dfa->start_vector.end(), m_dfa_practice.row[i][0]);
		if (result1 != dfa->start_vector.end())
			m_dfa_practice.start.push_back(i);

		vector<int>::iterator result2 = find(dfa->end_vector.begin(), dfa->end_vector.end(), m_dfa_practice.row[i][0]);
		if (result2 != dfa->end_vector.end())
			m_dfa_practice.end.push_back(i);
	}

	print_m_dfa(&m_dfa_practice);
	return 0;
}

int initialize_tran_set_vector(dfa_graph* dfa, int row_size)
{
	for (int i = 0; i < new_node_vector.size(); i++)
	{
		for (int j = 0; j < row_size; j++)
		{
			spt_struct new_struct;
			new_struct.node_id = dfa->dfa_table[i].row_vector[j].id;

			vector<int>::iterator result1 = find(dfa->not_end_vector.begin(), dfa->not_end_vector.end(), new_struct.node_id);
			if (result1 != dfa->not_end_vector.end())
				new_struct.set_id = 0;

			vector<int>::iterator result2 = find(dfa->end_vector.begin(), dfa->end_vector.end(), new_struct.node_id);
			if (result2 != dfa->end_vector.end())
				new_struct.set_id = 1;

			new_node_vector[i].tran_set_vector.push_back(new_struct);
		}
	}
	return 0;
}

int minimize_step(int row_size, int* set_num, m_dfa* m_dfa_practice)
{
	int flag = 0;
	m_set2.clear();

	for (int j = 1; j < m_set.size(); j++)//首位与其他比较
	{
		for (int k = 0; k < row_size; k++)
		{
			if (new_node_vector[m_set[0]].tran_set_vector[k].set_id !=
				new_node_vector[m_set[j]].tran_set_vector[k].set_id)
			{
				flag = 1;

				for (int p = 0; p < new_node_vector.size(); p++)
				{
					for (int q = 0; q < row_size; q++)
					{
						if (new_node_vector[p].tran_set_vector[q].node_id == new_node_vector[m_set[j]].id)
							new_node_vector[p].tran_set_vector[q].set_id = *set_num;
					}
				}

				m_set2.push_back(m_set[j]);
				m_set.erase(m_set.begin() + j);
				j--;
				break;
			}
		}
	}

	if (flag == 0)//此集合元素都等价
	{
		for (int j = 1; j < m_set.size(); j++)
		{
			new_node_vector[m_set[0]].subject.push_back(m_set[j]);
			m_set.erase(m_set.begin() + j);
			j--;
		}

		m_set1.clear();
		m_set1.push_back(m_set[0]);
		for (int j = 0; j < row_size; j++)
		{
			m_set1.push_back(new_node_vector[m_set[0]].tran_set_vector[j].node_id);
		}
		m_dfa_practice->row.push_back(m_set1);
		return 0;
	}

	else if (flag == 1)//此集合至少有一个元素不等价
	{
		spt_set_vector.push_back(m_set2);
		spt_set_vector.push_back(m_set);
		(*set_num)++;
		return 0;
	}
}

int print_m_dfa(m_dfa* m_dfa_practice)
{
	cout << endl << endl << "minimized dfa table: ";
	cout << endl << "new_id" << " " << "original_id" << " ";
	for (int i = 0; i < tran_char_vector.size(); i++)
	{
		cout << tran_char_vector[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < m_dfa_practice->row.size(); i++)
	{
		cout << i << " ";
		for (int j = 0; j < m_dfa_practice->row[i].size(); j++)
		{
			cout << m_dfa_practice->row[i][j] << " ";
		}
		cout << endl;
	}
	cout << "minimized start node: ";
	for (int i = 0; i < m_dfa_practice->start.size(); i++)
	{
		cout << m_dfa_practice->start[i] << " ";
	}
	cout << endl << "minimized end node: ";
	for (int i = 0; i < m_dfa_practice->end.size(); i++)
	{
		cout << m_dfa_practice->end[i] << " ";
	}
	cout << endl;
	return 0;
}

int is_subject(int tran_to, m_dfa* m_dfa_practice)
{
	for (int i = 0; i < m_dfa_practice->row.size(); i++)
	{
		if (new_node_vector[m_dfa_practice->row[i][0]].subject.empty()) continue;
		else
		{
			vector<int>::iterator result = find(new_node_vector[m_dfa_practice->row[i][0]].subject.begin(), new_node_vector[m_dfa_practice->row[i][0]].subject.end(), tran_to);
			if (result != new_node_vector[m_dfa_practice->row[i][0]].subject.end())
				return m_dfa_practice->row[i][0];
		}
	}
	return -1;
}