#include<algorithm>
#include"main.h"
#include"nfa_to_dfa.h"
#include"minimize_dfa.h"
using namespace std;

extern vector<new_node> new_node_vector_spt;
extern vector<new_node> new_node_vector;

m_dfa::m_dfa(void)
{
}
m_dfa::~m_dfa(void)
{
}

int minimize_dfa(dfa_graph* dfa, driver* driver_prac)
{
	m_dfa m_dfa_practice;

	int row_size = driver_prac->tran_char_vector.size();
	initialize_tran_set_vector(dfa, row_size, driver_prac);
	int set_num = 2;

	if (dfa->end_vector.size() == 0 || dfa->not_end_vector.size() == 0)
	{
		cout << endl << endl << "this dfa is natually minimized!" << endl;
		return 1;
	}

	driver_prac->spt_set_vector.push_back(dfa->end_vector);
	driver_prac->spt_set_vector.push_back(dfa->not_end_vector);

	while (!driver_prac->spt_set_vector.empty())
	{
		driver_prac->m_set = driver_prac->spt_set_vector.back();
		driver_prac->spt_set_vector.pop_back();
		minimize_step(row_size, &set_num, &m_dfa_practice, driver_prac);
	}

	//更新表
	for (int i = 0; i < m_dfa_practice.row.size(); i++)
	{
		for (int j = 1; j < row_size + 1; j++)
		{
			int result = is_subject(m_dfa_practice.row[i][j], &m_dfa_practice, driver_prac);
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

	print_m_dfa(&m_dfa_practice, driver_prac);
	return 0;
}

int initialize_tran_set_vector(dfa_graph* dfa, int row_size, driver* driver_prac)
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

int minimize_step(int row_size, int* set_num, m_dfa* m_dfa_practice, driver* driver_prac)
{
	int flag = 0;
	driver_prac->m_set2.clear();

	for (int j = 1; j < driver_prac->m_set.size(); j++)//首位与其他比较
	{
		for (int k = 0; k < row_size; k++)
		{
			if (new_node_vector[driver_prac->m_set[0]].tran_set_vector[k].set_id !=
				new_node_vector[driver_prac->m_set[j]].tran_set_vector[k].set_id)
			{
				flag = 1;

				for (int p = 0; p < new_node_vector.size(); p++)
				{
					for (int q = 0; q < row_size; q++)
					{
						if (new_node_vector[p].tran_set_vector[q].node_id == new_node_vector[driver_prac->m_set[j]].id)
							new_node_vector[p].tran_set_vector[q].set_id = *set_num;
					}
				}

				driver_prac->m_set2.push_back(driver_prac->m_set[j]);
				driver_prac->m_set.erase(driver_prac->m_set.begin() + j);
				j--;
				break;
			}
		}
	}

	if (flag == 0)//此集合元素都等价
	{
		for (int j = 1; j < driver_prac->m_set.size(); j++)
		{
			new_node_vector[driver_prac->m_set[0]].subject.push_back(driver_prac->m_set[j]);
			driver_prac->m_set.erase(driver_prac->m_set.begin() + j);
			j--;
		}

		driver_prac->m_set1.clear();
		driver_prac->m_set1.push_back(driver_prac->m_set[0]);
		for (int j = 0; j < row_size; j++)
		{
			driver_prac->m_set1.push_back(new_node_vector[driver_prac->m_set[0]].tran_set_vector[j].node_id);
		}
		m_dfa_practice->row.push_back(driver_prac->m_set1);
		return 0;
	}

	else if (flag == 1)//此集合至少有一个元素不等价
	{
		driver_prac->spt_set_vector.push_back(driver_prac->m_set2);
		driver_prac->spt_set_vector.push_back(driver_prac->m_set);
		(*set_num)++;
		return 0;
	}
}

int print_m_dfa(m_dfa* m_dfa_practice, driver* driver_prac)
{
	cout << endl << endl << "minimized dfa table: ";
	cout << endl << "new_id" << " " << "original_id" << " ";
	for (int i = 0; i < driver_prac->tran_char_vector.size(); i++)
	{
		cout << driver_prac->tran_char_vector[i] << " ";
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

int is_subject(int tran_to, m_dfa* m_dfa_practice, driver* driver_prac)
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