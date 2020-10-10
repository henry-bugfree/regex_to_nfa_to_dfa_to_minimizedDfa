#include"graph.h"
#include"main.h"
using namespace std;

graph_arc::graph_arc(void)
{
}
graph_arc::~graph_arc(void)
{
}

graph_node::graph_node(void)
{
}
graph_node::~graph_node(void)
{
}

graph::graph(void)
{
	start = 0;
	end = 0;
}
graph::~graph(void)
{
}

int graph::create_basic_model(graph* temp_graph, char value)
{
	graph_node new_node1;
	graph_node new_node2;
	graph_arc new_arc;

	temp_graph->arc_list.push_back(new_arc);
	temp_graph->arc_list.back().next_arc = -1;
	temp_graph->arc_list.back().value = value;

	temp_graph->node_list.push_back(new_node1);
	temp_graph->start = 0;
	temp_graph->node_list.back().first_arc = temp_graph->arc_list.size() - 1;

	temp_graph->node_list.push_back(new_node2);
	temp_graph->end = 1;
	temp_graph->node_list.back().first_arc = -1;

	temp_graph->arc_list.back().adjvex = 1;
	
	return 0;
}

int graph::repeat_model(graph* target)
{
	graph_node new_node1;
	graph_node new_node2;
	graph_arc new_arc1;
	graph_arc new_arc2;
	graph_arc new_arc3;
	graph_arc new_arc4;

	new_arc1.next_arc = target->arc_list.size() + 1;
	new_arc1.value = 'E';
	new_arc1.adjvex = 0;
	target->arc_list.push_back(new_arc1);

	target->node_list.back().first_arc = target->arc_list.size() - 1;
	new_node1.first_arc = target->arc_list.size() + 1;
	target->node_list.insert(target->node_list.begin(), new_node1);
	for (int i = 0; i < target->arc_list.size(); i++)
		target->arc_list[i].adjvex++;

	new_node2.first_arc = -1;
	target->node_list.push_back(new_node2);

	new_arc2.next_arc = -1;
	new_arc2.value = 'E';
	new_arc2.adjvex = target->node_list.size() - 1;
	target->arc_list.push_back(new_arc2);

	new_arc3.next_arc = target->arc_list.size() + 1;
	new_arc3.value = 'E';
	new_arc3.adjvex = 1;
	target->arc_list.push_back(new_arc3);

	new_arc4.next_arc = -1;
	new_arc4.value = 'E';
	new_arc4.adjvex = target->node_list.size() - 1;
	target->arc_list.push_back(new_arc4);

	target->start = 0;
	target->end = target->node_list.size() - 1;

	return 0;
}

int graph::connect_model(graph temp_graph, graph* target)
{	
	//合并时所需的偏移量
	int arc_size = target->arc_list.size();
	int node_size = target->node_list.size();

	for (int i = 1; i < temp_graph.node_list.size(); i++)
	{
		if (temp_graph.node_list[i].first_arc != -1)
		{
			temp_graph.node_list[i].first_arc = temp_graph.node_list[i].first_arc + arc_size;
		}
		target->node_list.push_back(temp_graph.node_list[i]);
	}

	for (int i = 0; i < temp_graph.arc_list.size(); i++)
	{
		if (temp_graph.arc_list[i].next_arc != -1)
		{
			temp_graph.arc_list[i].next_arc = temp_graph.arc_list[i].next_arc + arc_size;
		}
		temp_graph.arc_list[i].adjvex = temp_graph.arc_list[i].adjvex + node_size - 1;
		target->arc_list.push_back(temp_graph.arc_list[i]);
	}

	if (target->node_list[target->end].first_arc != -1)
	{
		int temp1 = target->node_list[target->end].first_arc;
		while (target->arc_list[temp1].next_arc != -1)
		{
			temp1 = target->arc_list[temp1].next_arc;
		}
		int temp = temp_graph.node_list[0].first_arc + arc_size;
		target->arc_list[temp1].next_arc = temp;
	}

	else if (target->node_list[target->end].first_arc == -1)
		target->node_list[target->end].first_arc = temp_graph.node_list[0].first_arc + arc_size;

	target->start = 0;
	target->end = target->node_list.size() - 1;

	return 0;
}

int graph::branch_model(graph temp_graph, graph* target)
{
	//合并时所需的偏移量
	int arc_size = target->arc_list.size();
	int node_size = target->node_list.size();

	for (int i = 0; i < temp_graph.node_list.size(); i++)
	{
		if (temp_graph.node_list[i].first_arc != -1)
		{
			temp_graph.node_list[i].first_arc = temp_graph.node_list[i].first_arc + arc_size;
		}
		target->node_list.push_back(temp_graph.node_list[i]);
	}

	for (int i = 0; i < temp_graph.arc_list.size(); i++)
	{
		if (temp_graph.arc_list[i].next_arc != -1)
		{
			temp_graph.arc_list[i].next_arc = temp_graph.arc_list[i].next_arc + arc_size;
		}
		temp_graph.arc_list[i].adjvex = temp_graph.arc_list[i].adjvex + node_size;
		target->arc_list.push_back(temp_graph.arc_list[i]);
	}

	graph_node new_node1;
	graph_node new_node2;
	graph_arc new_arc1;
	graph_arc new_arc2;
	graph_arc new_arc3;
	graph_arc new_arc4;

	new_arc1.adjvex = 0;
	new_arc1.next_arc = target->arc_list.size() + 1;
	new_arc1.value = 'E';
	target->arc_list.push_back(new_arc1);

	new_arc2.adjvex = node_size;
	new_arc2.next_arc = -1;
	new_arc2.value = 'E';
	target->arc_list.push_back(new_arc2);

	new_node1.first_arc = target->arc_list.size() - 2;
	target->node_list.insert(target->node_list.begin(), new_node1);
	for (int i = 0; i < target->arc_list.size(); i++)
		target->arc_list[i].adjvex++;

	new_arc3.adjvex = target->node_list.size();
	new_arc3.next_arc = -1;
	new_arc3.value = 'E';
	target->arc_list.push_back(new_arc3);

	new_arc4.adjvex = target->node_list.size();
	new_arc4.next_arc = -1;
	new_arc4.value = 'E';
	target->arc_list.push_back(new_arc4);

	target->node_list[target->end + 1].first_arc = target->arc_list.size() - 2;
	target->node_list[temp_graph.end + node_size + 1].first_arc = target->arc_list.size() - 1;

	new_node2.first_arc = -1;
	target->node_list.push_back(new_node2);

	target->start = 0;
	target->end = target->node_list.size() - 1;

	return 0;
}