#include"graph.h"
#include"rex_to_rpn.h"
#include"main.h"
using namespace std;

extern vector<char> rpn_vector;
extern stack<graph> temp_nfa_stack;

int rpn_to_nfa(void)
{
	for(int i=0; i < rpn_vector.size();i++)
	{
		if (!is_operator(rpn_vector[i]))
		{
			graph temp_nfa;
			temp_nfa.create_basic_model(&temp_nfa, rpn_vector[i]);
			temp_nfa_stack.push(temp_nfa);
		}
		else
		{
			switch(rpn_vector[i])
			{
			case '*':
				graph::repeat_model(&temp_nfa_stack.top());
				break;
			case '|':
			{
				graph temp_nfa;
				temp_nfa = temp_nfa_stack.top();
				temp_nfa_stack.pop();
				graph::branch_model(temp_nfa, &temp_nfa_stack.top());
				break;
			}
			case'+':
			{
				graph temp_nfa;
				temp_nfa = temp_nfa_stack.top();
				temp_nfa_stack.pop();
				graph::connect_model(temp_nfa, &temp_nfa_stack.top());
				break;
			}
			default:
				break;
			}
		}
	}

	return 0;
}

int print_nfa(graph target)
{
	cout << endl << "nfa: " << endl;

	int next_arc;
	int next_node;
	for (int i = 0; i < target.node_list.size(); i++)
	{
		cout << "node" << i;
		if (target.node_list[i].first_arc == -1)
		{
			cout << endl;
			continue;
		}
		else {
			next_arc = target.node_list[i].first_arc;
			next_node = target.arc_list[next_arc].adjvex;
			cout << "--" << target.arc_list[next_arc].value << "-->node" << next_node;
			while (target.arc_list[next_arc].next_arc != -1)
			{
				next_arc = target.arc_list[next_arc].next_arc;
				next_node = target.arc_list[next_arc].adjvex;
				cout << "--" << target.arc_list[next_arc].value << "-->node" << next_node;
			}
			cout << endl;
		}
	}

	cout << "start: " << target.start << endl;
	cout << "end: " << target.end << endl;
	cout << endl;

	return 0;
}