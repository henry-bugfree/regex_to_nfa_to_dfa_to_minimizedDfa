#include"graph.h"
#include"rex_to_rpn.h"
#include"nfa_to_dfa.h"
#include"main.h"
#include"rpn_to_nfa.h"
#include"nfa_to_dfa.h"
#include"minimize_dfa.h"
using namespace std;

#define MAX_REX_SIZE 100
#define TEST_NUM 5
stack<graph> temp_nfa_stack;

extern vector<char> rpn_vector;
extern stack<char> rpn_op_stack;
extern vector<char> tran_char_vector;
extern vector<int> set_search_vector;
extern stack<int> set_search_stack;
extern vector<new_node> new_node_vector_spt;
extern vector<new_node> new_node_vector;
extern vector<int> better_prt_vector;
extern vector<int> m_set;
extern vector<int> m_set1;
extern vector<int> m_set2;
extern vector<vector<int> > spt_set_vector;

int main()
{
	FILE* input;
	FILE* output;
	freopen_s(&input, "input.txt", "r", stdin);
	freopen_s(&output, "output.txt", "w", stdout);

	for (int i = 0; i < TEST_NUM; i++)
	{
		solve(i);
		reset();
		cout << endl;
	}

	fclose(stdin);
	return 0;
}

int solve(int i)
{	
	//declare graph_nfa
	graph graph_nfa;

	//input regular expression
	char rex[MAX_REX_SIZE];
	cin >> rex;
	cout << "test" << i << ":" << endl << "regular expression: " << rex << endl;
	int rex_len = (int)strlen(rex);

	//rex to rpn
	if (rpn_check(to_rpn(rex, rex_len)) == false) return 1;
	print_rnp();

	//rpn to nfa
	rpn_to_nfa();
	graph_nfa = temp_nfa_stack.top();
	temp_nfa_stack.pop();

	//print nfa
	print_nfa(graph_nfa);

	//nfa to dfa
	dfa_graph dfa;
	get_tran_char(rex, rex_len);
	if (nfa_to_dfa(graph_nfa, &dfa) == -2)
		return 1;
	print_dfa(&dfa);

	//minimize dfa
	minimize_dfa(&dfa);

	cout << endl;
	return 0;
}

int reset(void)
{
	while(!temp_nfa_stack.empty())
		temp_nfa_stack.pop();
	while (!rpn_op_stack.empty())
		rpn_op_stack.pop();
	while (!set_search_stack.empty())
		set_search_stack.pop();

	rpn_vector.clear();
	tran_char_vector.clear();
	set_search_vector.clear();
	new_node_vector_spt.clear();
	new_node_vector.clear();
	better_prt_vector.clear();
	m_set.clear();
	m_set1.clear();
	m_set2.clear();
	spt_set_vector.clear();

	return 0;
}