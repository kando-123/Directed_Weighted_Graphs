#include "graph_template.cpp"
#include <iostream>
#include <string>

int main()
{
	my_graph<std::string, unsigned int> towns;
	towns.insert_vertex("Piri", 8'500);
	towns.insert_vertex("Kafa", 7'400);
	towns.insert_vertex("Sele", 3'700);
	towns.insert_vertex("Tumu", 5'000);
	towns.insert_vertex("Bono", 2'900);
	towns.insert_vertex("Fiki", 1'200);
	towns.insert_vertex("Lutu", 1'100);
	towns.insert_vertex("Govo", 1'200);
	towns.insert_vertex("Nada", 1'000);
	towns.insert_vertex("Waya", 500);
	towns.insert_edge("Piri", "Kafa", 0.5);
	towns.insert_edge("Kafa", "Piri", 0.4);
	towns.insert_edge("Piri", "Sele", 1.2);
	towns.insert_edge("Sele", "Piri", 0.8);
	//towns.insert_edge("Kafa", "Nada", 0.1);
	towns.insert_edge("Kafa", "Sele", 2.8);
	towns.insert_edge("Kafa", "Sele", 1.1);
	towns.insert_edge("Sele", "Kafa", 0.7);
	towns.insert_edge("Piri", "Tumu", 1.0);
	towns.insert_edge("Tumu", "Piri", 1.3);
	towns.insert_edge("Piri", "Bono", 0.8);
	towns.insert_edge("Bono", "Piri", 0.9);
	towns.insert_edge("Tumu", "Bono", 0.6);
	towns.insert_edge("Bono", "Tumu", 0.4);
	towns.insert_edge("Bono", "Fiki", 2.3);
	towns.insert_edge("Fiki", "Bono", 2.4);
	towns.insert_edge("Fiki", "Fiki", 1.6);
	towns.insert_edge("Lutu", "Nada", 0.3);
	towns.insert_edge("Nada", "Lutu", 0.2);
	towns.insert_edge("Govo", "Nada", 0.2);
	towns.insert_edge("Nada", "Govo", 0.1);
	towns.insert_undirected_edge("Lutu", "Govo", 0.1);
	towns.print_graph();
	
	towns.reset_data("Piri", 9'000);
	//towns.erase_edge(8);
	towns.erase_edge(3, "Piri");
	towns.erase_edges("Kafa", "Sele");
	towns.erase_outedges("Bono");
	towns.erase_inedges("Tumu");
	towns.print_graph();
	std::cout << towns.vertex_data("Piri") << std::endl;
	std::cout << towns.indegree("Piri") << ' ' << towns.outdegree("Piri")
		<< ' ' << towns.degree("Piri") << ' ' << towns.degree() << std::endl;
	std::cout << towns.indegree("Tumu") << ' ' << towns.outdegree("Bono") << std::endl;
	return 0;
}