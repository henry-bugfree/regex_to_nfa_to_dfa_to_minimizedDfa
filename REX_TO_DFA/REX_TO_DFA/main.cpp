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

extern stack<graph> temp_nfa_stack;
extern vector<new_node> new_node_vector_spt;
extern vector<new_node> new_node_vector;

int main()
{
	FILE* input;
	FILE* output;
	#if defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
		// do Unix-specific stuff
		input = freopen("input.txt", "r", stdin);
		output = freopen("output.txt", "w", stdout);
	#else
		// do Windows-specific stuff
		freopen_s(&input, "input.txt", "r", stdin);
		freopen_s(&output, "output.txt", "w", stdout);
	#endif
	for (int i = 0; i < TEST_NUM; i++)
	{
		solve(i);
		reset();
		cout << endl;
	}

	fclose(stdin);
	fclose(stdout);
	return 0;
}

int solve(int i)
{	
	//declare graph_nfa
	graph graph_nfa;
	driver driver_prac;

	//input regular expression
	char rex[MAX_REX_SIZE];
	cin >> rex;
	cout << "test" << i << ":" << endl << "regular expression: " << rex << endl;
	int rex_len = (int)strlen(rex);

	//rex to rpn
	if (rpn_check(to_rpn(rex, rex_len, &driver_prac)) == false) return 1;
	print_rnp(&driver_prac);

	//rpn to nfa
	rpn_to_nfa(&driver_prac);
	graph_nfa = temp_nfa_stack.top();
	temp_nfa_stack.pop();

	//print nfa
	print_nfa(graph_nfa);

	//nfa to dfa
	dfa_graph dfa;
	get_tran_char(rex, rex_len, &driver_prac);
	if (nfa_to_dfa(graph_nfa, &dfa, &driver_prac) == -2)
		return 1;
	print_dfa(&dfa, &driver_prac);

	//minimize dfa
	minimize_dfa(&dfa, &driver_prac);

	cout << endl;
	return 0;
}

int reset(void)
{
	while (!temp_nfa_stack.empty())
		temp_nfa_stack.pop();

	new_node_vector.clear();
	new_node_vector_spt.clear();

	return 0;
}
