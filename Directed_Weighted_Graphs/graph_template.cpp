#pragma once
#include <list>
#include <map>

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
	};
	struct vertex_t
	{
		data_t data;
		std::list<edge_t> outedges;
	};
	std::map<key_type, vertex_t> incidences;
	size_t order, size;
public:
	my_graph() = default;
	
	void insert_vertex(key_t key, data_t data = data());
	void remove_vertex(key_t key);

	void reset_key(key_t key, key_t new_key);
	void reset_data(key_t key, data_t new_data);
	data_t& vertex_data(key_t key);

	size_t indegree(key_t key);
	size_t outdegree(key_t key);
	size_t degree(key_t key);

	void insert_edge(key_t tail, key_t head, weight_t weight = 0.0);
	void remove_edge(key_t tail, key_t head);

	void reset_weight(key_t tail, key_t head, weight_t weight);
	weight_t& edge_weight(key_t tail, key_t head);
};

/* Inserts the vertex with key 'key', provided such key is not yet present in the graph,
 and assigns its data with 'data', which is equal to data_t() by default.
 */
template<class key_t, class data_t, class weight_t>
void my_graph<key_t, data_t, weight_t>::insert_vertex(key_t key, data_t data)
{
	
}