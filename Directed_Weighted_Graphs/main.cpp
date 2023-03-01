#include "graph_template.cpp"
#include <iostream>
#include <string>

inline void header(std::string text)
{
	std::cout << std::endl << " === === " << text << " === ===" << std::endl;
}

int main()
{
	my_graph<std::string, unsigned int> towns;
	towns.insert_vertex("Piri", 8'500);
	towns.insert_vertex("Kafa", 7'400);
	towns.insert_vertex("Sele", 3'700);
	towns.insert_vertex("Dimi", 1'100);
	towns.insert_vertex("Tumu", 5'000);
	towns.insert_vertex("Bono", 2'900);
	towns.insert_vertex("Fiki", 1'200);
	towns.insert_vertex("Lutu", 1'100);
	towns.insert_vertex("Govo", 1'200);
	towns.insert_vertex("Nada", 1'000);
	towns.insert_vertex("Hubu", 200);
	towns.insert_vertex("Rava", 700);
	towns.insert_vertex("Waya", 500);
	towns.insert_edge("Piri", "Kafa", 0.7);
	towns.insert_edge("Piri", "Kafa", 0.5);
	towns.insert_edge("Kafa", "Piri", 0.4);
	towns.insert_edge("Kafa", "Piri", 0.3);
	towns.insert_edge("Kafa", "Dimi", 0.7);
	towns.insert_edge("Dimi", "Kafa", 0.6);
	towns.insert_edge("Dimi", "Dimi", 0.2);
	towns.insert_edge("Piri", "Sele", 1.2);
	towns.insert_edge("Sele", "Piri", 0.8);
	towns.insert_edge("Kafa", "Sele", 2.8);
	towns.insert_edge("Kafa", "Sele", 1.1);
	towns.insert_edge("Sele", "Kafa", 2.0);
	towns.insert_edge("Sele", "Kafa", 0.7);
	towns.insert_edge("Piri", "Tumu", 1.1);
	towns.insert_edge("Piri", "Tumu", 1.0);
	towns.insert_edge("Tumu", "Piri", 1.3);
	towns.insert_edge("Tumu", "Piri", 1.2);
	towns.insert_edge("Tumu", "Tumu", 0.4);
	towns.insert_edge("Piri", "Bono", 0.8);
	towns.insert_edge("Bono", "Piri", 0.9);
	towns.insert_edge("Tumu", "Bono", 0.7);
	towns.insert_edge("Tumu", "Bono", 0.6);
	towns.insert_edge("Bono", "Tumu", 0.6);
	towns.insert_edge("Bono", "Tumu", 0.4);
	towns.insert_edge("Bono", "Fiki", 3.0);
	towns.insert_edge("Bono", "Fiki", 2.3);
	towns.insert_edge("Fiki", "Bono", 3.1);
	towns.insert_edge("Fiki", "Bono", 2.4);
	towns.insert_edge("Fiki", "Fiki", 1.6);
	towns.insert_edge("Rava", "Rava", 0.3);
	towns.insert_edge("Rava", "Rava", 0.9);
	towns.insert_edge("Hubu", "Piri", 4.1);
	towns.insert_edge("Piri", "Hubu", 4.0);
	towns.insert_edge("Hubu", "Govo", 3.0);
	towns.insert_edge("Govo", "Hubu", 3.1);
	towns.insert_edge("Lutu", "Nada", 0.3);
	towns.insert_edge("Nada", "Lutu", 0.2);
	towns.insert_edge("Govo", "Nada", 0.4);
	towns.insert_edge("Govo", "Nada", 0.2);
	towns.insert_edge("Nada", "Govo", 0.3);
	towns.insert_edge("Nada", "Govo", 0.1);
	towns.insert_undirected_edge("Lutu", "Govo", 0.1);
	towns.insert_undirected_edge("Lutu", "Govo", 0.2);
	towns.print_graph();

	header("reset_key(\"Jubu\", \"Piri\")");
	try
	{
		towns.reset_key("Jubu", "Piri");
	}
	catch (error_t e)
	{
		std::cout << e.what() << std::endl;
	}

	header("reset_key(\"Hubu\", \"Piri\")");
	try
	{
		towns.reset_key("Hubu", "Piri");
	}
	catch (error_t e)
	{
		std::cout << e.what() << std::endl;
	}

	header("reset_key(\"Hubu\", \"Jubu\")");
	towns.reset_key("Hubu", "Jubu");
	towns.print_graph();

	header("reset_data(\"Zubu\", 10'000)");
	try
	{
		towns.reset_data("Zubu", 10'000);
	}
	catch (error_t e)
	{
		std::cout << e.what() << std::endl;
	}

	header("reset_data(\"Jubu\", 10'000)");
	towns.reset_data("Jubu", 10'000);
	towns.print_graph();

	header("vertex_data(\"Zubu\")");
	try
	{
		towns.vertex_data("Zubu") = 15'000;
	}
	catch (error_t e)
	{
		std::cout << e.what() << std::endl;
	}

	header("vertex_data(\"Jubu\") = 15'000");
	towns.vertex_data("Jubu") = 15'000;
	std::cout << towns.vertex_data("Jubu") << std::endl;

	header("erase_vertex(\"Jubu\")");
	towns.erase_vertex("Jubu");
	towns.print_graph();

	header("indegree(\"Piri\")");
	std::cout << towns.indegree("Piri") << std::endl;

	header("outdegree(\"Tumu\")");
	std::cout << towns.outdegree("Tumu") << std::endl;

	header("degree(\"Kafa\")");
	std::cout << towns.degree("Kafa") << std::endl;

	header("degree()");
	std::cout << towns.degree() << std::endl;

	header("edges(\"Fiki\", \"Fiki\")");
	auto v = towns.edges("Fiki", "Fiki");
	if (v.empty())
		std::cout << "No edges." << std::endl;
	else
	{
		for (int i = 0; i < v.size(); ++i)
			std::cout << "[" << i << "] " << v[i] << "; ";
		std::cout << std::endl;
	}

	header("outedges(\"Fiki\")");
	v = towns.outedges("Fiki");
	if (v.empty())
		std::cout << "No edges." << std::endl;
	else
	{
		for (int i = 0; i < v.size(); ++i)
			std::cout << "[" << i << "] " << v[i] << "; ";
		std::cout << std::endl;
	}

	header("inedges(\"Fiki\")");
	v = towns.inedges("Fiki");
	if (v.empty())
		std::cout << "No edges." << std::endl;
	else
	{
		for (int i = 0; i < v.size(); ++i)
			std::cout << "[" << i << "] " << v[i] << "; ";
		std::cout << std::endl;
	}

	header("BFS, print the number of inhabitants");
	towns.breadth_first_search(
		[](std::string k, unsigned int d)
		{
			std::cout << k << " has " << d << " inhabitants." << std::endl;
		},
		"Piri");

	header("DFS, print the number of inhabitants");
	towns.depth_first_search(
		[](std::string k, unsigned int d)
		{
			std::cout << k << " has " << d << " inhabitants." << std::endl;
		},
		"Piri");

	header("undefined");
	std::cout << "\"" << my_graph<std::string, unsigned int>::undefined << "\"" << std::endl;

	header("undefined = \"none\"");
	my_graph<std::string, unsigned int>::undefined = "none";
	std::cout << "\"" << my_graph<std::string, unsigned int>::undefined << "\"" << std::endl;

	header("Dijkstra");
	auto Dijkstra = towns.Dijkstra("Tumu");

	return 0;
}