#pragma once
#include <list>
#include <map>
#include <iostream>

/*
 Implements a directed, weighted graph that enables storing additional data for vertices.
 @param key_t - type of the keys of the vertices, unique in all the graph; ket_t() should not be used
 @param data_t - type of the data assigned to the vertices
 @param weight_t - type of the weights assigned to the edges, should be of a floating-point type
 */
template <class key_t, class data_t, class weight_t = double>
class my_graph
{
	struct edge_t
	{
		key_t head;
		weight_t weight;
		
		edge_t(key_t _head, weight_t _weight = 0.0) : head(_head), weight(_weight) {}
	};
	struct vertex_t
	{
		data_t data;
		std::list<edge_t> outedges;

		vertex_t(data_t _data = data_t()) : data(_data), outedges() {}
		// vertex_t(data_t _data, std::list<edge_t> _outedges); // ?
	};
	std::map<key_t, vertex_t> incidences;
	size_t graph_order, graph_size;

public:
	void print_graph();

	my_graph() = default;
	
	void insert_vertex(key_t key, data_t data = data_t());
	void remove_vertex(key_t key);

	void reset_key(key_t key, key_t new_key);
	void reset_data(key_t key, data_t new_data);
	data_t& vertex_data(key_t key);

	size_t indegree(key_t key);
	size_t outdegree(key_t key);
	size_t degree(key_t key);

	void insert_edge(key_t tail, key_t head, weight_t weight = 0.0);
	void insert_undirected_edge(key_t tail, key_t head, weight_t weight = 0.0);
	void remove_edge(key_t tail, key_t head);

	void reset_weight(key_t tail, key_t head, weight_t weight);
	weight_t& edge_weight(key_t tail, key_t head);
};

/* Prints the graph to the console. */
template<class key_t, class data_t, class weight_t>
void my_graph<key_t, data_t, weight_t>::print_graph()
{
	if (incidences.empty())
	{
		std::cout << "empty graph" << std::endl;
		return;
	}
	std::cout << "order = " << graph_order << ", size = " << graph_size << std::endl
		<< "(key)\t{data}\toutgoing edges: [head:weight]" << std::endl;
	for (const auto& i : incidences)
	{
		std::cout << "(" << i.first << ")\t{" << i.second.data << "}\t";
		if (i.second.outedges.empty())
			std::cout << "<none>" << std::endl;
		else
		{
			for (const auto& o : i.second.outedges)
				std::cout << "[" << o.head << ":" << o.weight << "] ";
			std::cout << std::endl;
		}
	}
}

/* Inserts the vertex with key 'key', provided such key is not yet present in the graph,
 and assigns its data with 'data', which is equal to data_t() by default.
 */
template<class key_t, class data_t, class weight_t>
void my_graph<key_t, data_t, weight_t>::insert_vertex(key_t key, data_t data)
{
	incidences.insert(std::pair<key_t, vertex_t>(key, vertex_t(data)));
	++graph_order;
}

template<class key_t, class data_t, class weight_t>
void my_graph<key_t, data_t, weight_t>::insert_edge(key_t tail, key_t head, weight_t weight)
{
	if (incidences.find(tail) == incidences.end() or incidences.find(head) == incidences.end())
		return;
	incidences[tail].outedges.emplace_back(edge_t(head, weight));
	++graph_size;
}

template<class key_t, class data_t, class weight_t>
void my_graph<key_t, data_t, weight_t>::insert_undirected_edge(key_t tail, key_t head, weight_t weight)
{
	if (incidences.find(tail) == incidences.end() or incidences.find(head) == incidences.end())
		return;
	incidences[tail].outedges.emplace_back(edge_t(head, weight));
	incidences[head].outedges.emplace_back(edge_t(tail, weight));
	graph_size += 2;
}
