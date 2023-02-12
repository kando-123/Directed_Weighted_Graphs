#pragma once
#include <exception>
#include <iostream>
#include <list>
#include <map>

/** Implements a directed, weighted graph that enables storing additional data for vertices.
 * @param key_t - type of the keys of the vertices, unique in all the graph; ket_t() should not be used
 * @param data_t - type of the data assigned to the vertices
 * @param weight_t - type of the weights assigned to the edges, should be of a floating-point type
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

	my_graph();
	
	void insert_vertex(key_t key, data_t data = data_t());
	void erase_vertex(key_t key);

	void reset_key(key_t key, key_t new_key);
	void reset_data(key_t key, data_t new_data);
	data_t& vertex_data(key_t key);

	size_t indegree(key_t key);
	size_t outdegree(key_t key);
	size_t degree(key_t key);
	size_t degree();

	void insert_edge(key_t tail, key_t head, weight_t weight = 0.0);
	void insert_undirected_edge(key_t tail, key_t head, weight_t weight = 0.0);
	void erase_edge(key_t tail, key_t head);

	void reset_weight(key_t tail, key_t head, weight_t weight);
	weight_t& edge_weight(key_t tail, key_t head);
};

/// Enumeration class for errors, used as argument to class 'error_t' constructor.
enum class problem_t { out_of_range = 1 };

/// Class used for throwing errors.
class error_t : public std::exception
{
	problem_t problem;
public:
	error_t(problem_t _problem);
	const char* what();
};

/** Constructor of class 'error_t'.
 * @param problem_t _problem - the information about the problem that has occurred
 */
error_t::error_t(problem_t _problem) : problem(_problem)
{
	// done :D
}

/// Specialization of method what().
const char* error_t::what()
{
	switch (problem)
	{
	case problem_t::out_of_range:
		return "Out of range.";
	default:
		return "Unknown problem.";
	}
}

/// Prints the graph to the console.
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
			std::cout << "<none>";
		else
			for (const auto& o : i.second.outedges)
				std::cout << "[" << o.head << ":" << o.weight << "] ";
		std::cout << std::endl;
	}
}

/// Constructor of class 'my_graph'.
template<class key_t, class data_t, class weight_t>
my_graph<key_t, data_t, weight_t>::my_graph() : graph_order(0), graph_size(0), incidences()
{
	// done :D
}

/** Inserts vertex 'key', provided such key is not yet present in the graph,
 * and assigns it with 'data', that is equal to data_t() by default.
 * @param key_t key - the key that should be inserted
 * @param data_t data - the data that should be assigned to the vertex
 * @return true if the replacement succeeded, false otherwise
 */
template<class key_t, class data_t, class weight_t>
void my_graph<key_t, data_t, weight_t>::insert_vertex(key_t key, data_t data)
{
	incidences.insert(std::pair<key_t, vertex_t>(key, vertex_t(data)));
	++graph_order;
}

/** Erases vertex 'key' and all edges incident on it.
 * @param key_t key - the key of the vertex that should be erased
 */
template<class key_t, class data_t, class weight_t>
void my_graph<key_t, data_t, weight_t>::erase_vertex(key_t key)
{
	if (incidences.find(key) == incidences.end())
		return;
	graph_size -= incidences[key].outedges.size();
	incidences.erase(key);
	--graph_order;
	for (auto& i : incidences)
	{
		auto it = i.second.outedges.begin();
		while (it != i.second.outedges.end())
		{
			if (it->head == key)
			{
				it = i.second.outedges.erase(it);
				--graph_size;
			}
			else
				++it;
		}
	}
}

/* Moves the data associated with vertex 'key' to a newly created vertex 'new_key'.
 * The old vertex is erased; 'key' is replaced by 'new_key' in all the edges.
 * Throws error_t(problem_t::out_of_range) if 'key' is not present or 'new_key' is already present,
 * or 'key' and 'new_key' are equal.
 * @param key_t key - the key to be replaced
 * @param key_t new_key - the key to replace with
 */
template<class key_t, class data_t, class weight_t>
void my_graph<key_t, data_t, weight_t>::reset_key(key_t key, key_t new_key)
{
	if (incidences.find(key) == incidences.end() or incidences.find(new_key) != incidences.end() or key == new_key)
		throw error_t(problem_t::out_of_range);
	incidences[new_key] = std::move(incidences[key]);
	incidences.erase(key);
	for (auto& i : incidences)
	{
		auto it = i.second.outedges.begin();
		while (it != i.second.outedges.end())
		{
			if (it->head == key)
				it->head = new_key;
			++it;
		}
	}
	return true;
}

/** Resets the data assigned to vertex 'key'.
 * Throws error_t(problem_t::out_of_range) if no such vertex is present in the graph.
 * @param key_t key - the key of the vertex whose data should be reset
 * @param data_t new_data - the new data that should be assigned to the vertex
 */
template<class key_t, class data_t, class weight_t>
void my_graph<key_t, data_t, weight_t>::reset_data(key_t key, data_t new_data)
{
	if (incidences.find(key) == incidences.end())
		throw error_t(problem_t::out_of_range);
	incidences[key].data = new_data;
}

 /** Returns reference to the data assigned to vertex 'key'.
  * Throws error_t(problem_t::out_of_range) if no such vertex is present in the graph.
  * @param key_t key - the vertex whose data should be accessed
  * @return reference to the data, type: data_t&
  */
template<class key_t, class data_t, class weight_t>
data_t& my_graph<key_t, data_t, weight_t>::vertex_data(key_t key)
{
	if (incidences.find(key) == incidences.end())
		throw error_t(problem_t::out_of_range);
	return incidences[key].data;
}

template<class key_t, class data_t, class weight_t>
size_t my_graph<key_t, data_t, weight_t>::indegree(key_t key)
{
	if (incidences.find(key) == incidences.end())
		throw error_t(problem_t::out_of_range);
	size_t count = 0;
	for (const auto& i : incidences)
	{
		for (const auto& o : i.second.outedges)
		{
			if (o.head == key)
				++count;
		}
	}
	return count;
}

template<class key_t, class data_t, class weight_t>
size_t my_graph<key_t, data_t, weight_t>::outdegree(key_t key)
{
	if (incidences.find(key) == incidences.end())
		throw error_t(problem_t::out_of_range);
	return incidences[key].outedges.size();
}

template<class key_t, class data_t, class weight_t>
size_t my_graph<key_t, data_t, weight_t>::degree(key_t key)
{
	if (incidences.find(key) == incidences.end())
		throw error_t(problem_t::out_of_range);
	size_t count = incidences[key].outedges.size();
	for (const auto& i : incidences)
	{
		for (const auto& o : i.second.outedges)
		{
			if (o.head == key)
				++count;
		}
	}
	return count;
}

template<class key_t, class data_t, class weight_t>
size_t my_graph<key_t, data_t, weight_t>::degree()
{
	std::map<key_t, size_t> counts;
	for (const auto& i : incidences)
		counts.insert(std::make_pair<key_t, size_t>(i.first, i.second.outedges.size()));
	for (const auto& i : incidences)
		for (const auto& o : i.second.outedges)
			++counts[o.first];
	size_t max_count = 0;
	for (const auto& c : counts)
		if (c.second > max_count)
			max_count = c.second;
	return max_count;
	// ?
}

/** Inserts an edge from 'tail' to 'head', provided such vertices are present in the graph,
 * and assigns it with 'weight', that is equal to 0.0 by default.
 * @param key_t tail - the initial vertex of the edge
 * @param key_t head - the terminal vertex of the edge
 * @param weight_t weight - the weight that should be assigned to the edge
 */
template<class key_t, class data_t, class weight_t>
void my_graph<key_t, data_t, weight_t>::insert_edge(key_t tail, key_t head, weight_t weight)
{
	if (incidences.find(tail) == incidences.end() or incidences.find(head) == incidences.end())
		return;
	incidences[tail].outedges.emplace_back(edge_t(head, weight));
	++graph_size;
}

 /** Inserts two edges: from 'tail' to 'head' and from 'head' to 'tail',
  * and assigns them with 'weight', that is equal to 0.0 by default.
  * @param key_t tail - an endpoint of the edges
  * @param key_t head - the other endpoint of the edges
  * @param weight_t weight - the weight that should be assigned to the edges
  */
template<class key_t, class data_t, class weight_t>
void my_graph<key_t, data_t, weight_t>::insert_undirected_edge(key_t tail, key_t head, weight_t weight)
{
	if (incidences.find(tail) == incidences.end() or incidences.find(head) == incidences.end())
		return;
	incidences[tail].outedges.emplace_back(edge_t(head, weight));
	incidences[head].outedges.emplace_back(edge_t(tail, weight));
	graph_size += 2;
}
