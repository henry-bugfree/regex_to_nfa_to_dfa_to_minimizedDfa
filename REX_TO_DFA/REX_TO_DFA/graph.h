#pragma once
#include"main.h"
using namespace std;

class graph_arc
{	
	public:
		//Ĭ�Ϲ��캯��
		graph_arc(void);
		//��������
		~graph_arc(void);
	public:
		int adjvex;
		int next_arc;
		char value;
};

class graph_node
{
	public:
		//Ĭ�Ϲ��캯��
		graph_node(void);
		//��������
		~graph_node(void);
	public:
		int first_arc;
};

class graph
{
	public:
		//Ĭ�Ϲ��캯��
		graph(void);
		//��������
		~graph(void);
	public:
		vector<graph_node> node_list;
		vector<graph_arc> arc_list;
		int start;
		int end;
	public:
		//reform : aim to make graph->start always be 0 and graph->end always be size()-1
		int create_basic_model(graph* temp_graph, char value);
		static int repeat_model(graph* target);
		static int connect_model(graph temp_graph, graph* target);
		static int branch_model(graph temp_graph, graph* target);
};