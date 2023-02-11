#include "graph_template.cpp"
#include <iostream>

int main()
{
	my_graph<std::string, unsigned int> towns;
	towns.print_graph();
	
	towns.insert_vertex("Piri", 8'500);
	towns.insert_vertex("Tumu", 5'000);
	towns.insert_edge("Piri", "Tumu", 1.0);
	towns.insert_edge("Tumu", "Piri", 1.3);
	towns.insert_undirected_edge("Piri", "Tumu", 4.5);
	towns.print_graph();
	return 0;
}