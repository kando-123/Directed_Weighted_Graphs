#include "graph_template.cpp"
#include <iostream>

int main()
{
	std::cout << "Hello, world!" << std::endl;
	my_graph<int, int> G;
	G.insert_vertex(1);
	return 0;
}