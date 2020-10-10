#include"rex_to_rpn.h"
#include"main.h"
using namespace std;

vector<char> rpn_vector;
stack<char> rpn_op_stack;

void print_rnp(void)
{
	cout << "rnp: ";
	for (unsigned int i = 0; i < rpn_vector.size(); i++)
		cout << rpn_vector[i] << " ";
	cout << endl;
}

int to_rpn(char* rex,int rex_len)
{
	for (int i = 0; i < rex_len; i++)
	{
		if (!is_operator(rex[i]) || rex[i]=='*')
		{
			rpn_vector.push_back(rex[i]);
			if (is_and(rex, rex_len, i) == true)
			{
				while (!rpn_op_stack.empty() && (rpn_op_stack.top() == '*' || rpn_op_stack.top() == '+'))
				{
					rpn_vector.push_back(rpn_op_stack.top());
					rpn_op_stack.pop();
				}
				rpn_op_stack.push('+');
			}
		}

		else
		{
			switch (rex[i])
			{
			case '*':
				rpn_vector.push_back('*');
				break;
			case '|':
				while (!rpn_op_stack.empty() && (rpn_op_stack.top() == '*' || rpn_op_stack.top() == '+' || rpn_op_stack.top() == '|'))
				{
					rpn_vector.push_back(rpn_op_stack.top());
					rpn_op_stack.pop();
				}
				rpn_op_stack.push('|');
				break;
			case '(':
				rpn_op_stack.push('(');
				break;
			case ')':
				while (!rpn_op_stack.empty() && rpn_op_stack.top() != '(')
				{
					rpn_vector.push_back(rpn_op_stack.top());
					rpn_op_stack.pop();
				}
				if (rpn_op_stack.empty()) return 1;
				else {
					rpn_op_stack.pop();//pop '('
				}
				if (is_and(rex, rex_len, i) == true)
				{
					while (!rpn_op_stack.empty() && (rpn_op_stack.top() == '*' || rpn_op_stack.top() == '+'))
					{
						rpn_vector.push_back(rpn_op_stack.top());
						rpn_op_stack.pop();
					}
					rpn_op_stack.push('+');
				}
				break;
			default:
				break;
			}
		}
	}
	while(!rpn_op_stack.empty())
	{
		if (rpn_op_stack.top() == '(') return 2;
		else
		{
			rpn_vector.push_back(rpn_op_stack.top());
			rpn_op_stack.pop();
		}
	}
	return 0;
}

bool is_operator(char ch)
{
	if (ch == '|' || ch == '*' || ch == '(' || ch == ')' || ch == '+')
		return true;
	return false;
}

bool is_and(char* rex, int rex_len, int i)
{
	if (i + 1 >= rex_len) return false;
	else if ((!is_operator(rex[i+1])) || rex[i + 1] == '(') return true;
	else return false;
	//"aaa" == 'a+a+a'
}

bool rpn_check(int check_num)
{
	if (check_num == 1)
	{
		cout << "正则表达式括号不匹配，缺少左括号" << endl;
		return false;
	}
	else if (check_num == 2)
	{
		cout << "正则表达式括号不匹配，缺少右括号" << endl;
		return false;
	}
	else return true;
}