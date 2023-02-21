#pragma once
#include <algorithm>
#include <exception>
#include <iostream>
#include <list>
#include <map>
#include <vector>

/** Implements a directed, weighted graph that enables storing additional data for vertices.
* Vertices are recognised by unique keys; key_t() should not be used as a key since it denotes
* a special value 'undefined'. The weight type should be of a floating-point type because such
* provides value infinity.
* @param key_t - type of the keys of the vertices
* @param data_t - type of the data assigned to the vertices
* @param weight_t - type of the weights assigned to the edges
*/
template <class key_t, class data_t, class weight_t = double>
class my_graph
{
	struct edge_t
	{
		key_t head;
		weight_t weight;
		size_t ordinal;
		edge_t(size_t _ordinal, key_t _head, weight_t _weight = 0.0)
			: ordinal(_ordinal), head(_head), weight(_weight) {}
	};
	struct vertex_t
	{
		data_t data;
		std::list<edge_t> outedges;
		vertex_t(data_t _data = data_t()) : data(_data), outedges() {}
	};

	std::map<key_t, vertex_t> incidences;
	size_t graph_order, graph_size, edges_count;

public:
	my_graph();

	void print_graph();

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
	std::vector<size_t> edges(key_t tail, key_t head);
	std::vector<size_t> outedges(key_t tail);
	std::vector<size_t> inedges(key_t head);
	void erase_edge(size_t number);
	void erase_edge(size_t number, key_t tail);
	void erase_edges(key_t tail, key_t head);
	void erase_inedges(key_t head);
	void erase_outedges(key_t tail);
	void reset_weight(size_t number, weight_t new_weight);
	void reset_weight(size_t number, weight_t new_weight, key_t tail);
	void reset_weights(key_t tail, key_t head, weight_t new_weight);
	weight_t& edge_weight(size_t number);
	weight_t& edge_weight(size_t number, key_t tail);

	// bfs
	// dfs
	// Dijkstra
	// Bellman_Ford
	// Floyd_Warshall
	// 

	bool empty();
};

/// Enumeration class for errors, used as argument to class 'error_t' constructor.
enum class problem_t { out_of_range = 1, empty_graph, invalid_value };

/// Class used for throwing exceptions.
class error_t : public std::exception
{
	problem_t problem;
public:
	error_t(problem_t _problem) : problem(_problem) {}
	const char* what()
	{
		switch (problem)
		{
		case problem_t::out_of_range:
			return "Out of range.";
		case problem_t::empty_graph:
			return "Empty graph.";
		case problem_t::invalid_value:
			return "Invalid value.";
		default:
			return "Unknown problem.";
		}
	}
};

/// Constructor of class 'my_graph'.
template<class key_t, class data_t, class weight_t>
my_graph<key_t, data_t, weight_t>::my_graph() : graph_order(0), graph_size(0), edges_count(0)
{
	
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
	std::cout << "order = " << graph_order << ", size = " << graph_size << ";" << std::endl
		<< "(key)\t{data}\toutedges: [ordinal|head:weight]" << std::endl;
	for (auto i = incidences.begin(); i != incidences.end(); ++i)
	{
		std::cout << "(" << i->first << ")\t{" << i->second.data << "}\t";
		if (i->second.outedges.empty())
			std::cout << "<none>";
		else
			for (auto o = i->second.outedges.begin(); o != i->second.outedges.end(); ++o)
				std::cout << "[" << o->ordinal << "|" << o->head << ":" << o->weight << "] ";
		std::cout << std::endl;
	}
}

/** Inserts vertex 'key', provided such key is not yet present in the graph,
* and assigns it with 'data', that is equal to data_t() by default.
* @param key_t key - the key that should be inserted
* @param data_t data - the data that should be assigned to the vertex
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
	for (auto i = incidences.begin(); i != incidences.end(); ++i)
	{
		auto o = i->second.outedges.begin();
		while (o != i->second.outedges.end())
		{
			if (o->head == key)
			{
				o = i->second.outedges.erase(o);
				--graph_size;
			}
			else
				++o;
		}
	}
}

/** Moves the data associated with vertex 'key' to a newly created vertex 'new_key'.
* The old vertex is erased and 'key' is replaced by 'new_key' in all the edges.
* @param key_t key - the key to be replaced
* @param key_t new_key - the key to replace with
* @throw error_t(problem_t::out_of_range) - if 'key' is not present in the graph
* @throw error_t(problem_t::invalid_value) - if 'new_key' is already present in the graph
*/
template<class key_t, class data_t, class weight_t>
void my_graph<key_t, data_t, weight_t>::reset_key(key_t key, key_t new_key)
{
	if (key == new_key)
		return;
	if (incidences.find(key) == incidences.end())
		throw error_t(problem_t::out_of_range);
	if (incidences.find(new_key) != incidences.end())
		throw error_t(problem_t::invalid_value);
	incidences[new_key] = std::move(incidences[key]);
	incidences.erase(key);
	for (auto i = incidences.begin(); i != incidences.end(); ++i)
	{
		auto o = i->second.outedges.begin();
		while (o != i->second.outedges.end())
		{
			if (o->head == key)
				o->head = new_key;
			++o;
		}
	}
}

/** Resets the data assigned to vertex 'key'.
* @param key_t key - the key of the vertex whose data should be reset
* @param data_t new_data - the new data that should be assigned to the vertex
* @throw error_t(problem_t::out_of_range) - if no such vertex is present in the graph
*/
template<class key_t, class data_t, class weight_t>
void my_graph<key_t, data_t, weight_t>::reset_data(key_t key, data_t new_data)
{
	if (incidences.find(key) == incidences.end())
		throw error_t(problem_t::out_of_range);
	incidences[key].data = new_data;
}

/** Returns reference to the data assigned to vertex 'key'.
* @param key_t key - the vertex whose data should be accessed
* @return reference to the data
* @throw error_t(problem_t::out_of_range) - if no such vertex is present in the graph
*/
template<class key_t, class data_t, class weight_t>
data_t& my_graph<key_t, data_t, weight_t>::vertex_data(key_t key)
{
	if (incidences.find(key) == incidences.end())
		throw error_t(problem_t::out_of_range);
	return incidences[key].data;
}

/** Counts the input degree of vertex 'key'.
* @param key_t key - the vertex whose input degree should be counted
* @return the number of edges leading to the vertex
* @throw error_t(problem_t::out_of_range) - if no such vertex is present in the graph
*/
template<class key_t, class data_t, class weight_t>
size_t my_graph<key_t, data_t, weight_t>::indegree(key_t key)
{
	if (incidences.find(key) == incidences.end())
		throw error_t(problem_t::out_of_range);
	size_t count = 0;
	for (auto i = incidences.begin(); i != incidences.end(); ++i)
	{
		for (auto o = i->second.outedges.begin(); o != i->second.outedges.end(); ++o)
		{
			if (o->head == key)
				++count;
		}
	}
	return count;
}

/** Counts the output degree of vertex 'key'.
* @param key_t key - the vertex whose output degree should be counted
* @return the number of edges going out from the vertex
* @throw error_t(problem_t::out_of_range) - if no such vertex is present in the graph
*/
template<class key_t, class data_t, class weight_t>
size_t my_graph<key_t, data_t, weight_t>::outdegree(key_t key)
{
	if (incidences.find(key) == incidences.end())
		throw error_t(problem_t::out_of_range);
	return incidences[key].outedges.size();
}

/** Counts the degree of vertex 'key'.
* @param key_t key - the vertex whose degree should be counted
* @return the number of edges incident on the vertex
* @throw error_t(problem_t::out_of_range) - if no such vertex is present in the graph
*/
template<class key_t, class data_t, class weight_t>
size_t my_graph<key_t, data_t, weight_t>::degree(key_t key)
{
	if (incidences.find(key) == incidences.end())
		throw error_t(problem_t::out_of_range);
	size_t count = incidences[key].outedges.size();
	for (auto i = incidences.begin(); i != incidences.end(); ++i)
	{
		for (auto o = i->second.outedges.begin(); o != i->second.outedges.end(); ++o)
		{
			if (o->head == key)
				++count;
		}
	}
	return count;
}

/** Counts the degrees of the vertices and returns the greatest one.
* @return degree of the graph
* @throw error_t(problem_t::empty_graph) - if the graph is empty
*/
template<class key_t, class data_t, class weight_t>
size_t my_graph<key_t, data_t, weight_t>::degree()
{
	if (incidences.empty())
		throw error_t(problem_t::empty_graph);
	std::map<key_t, size_t> counts;
	for (auto i = incidences.begin(); i != incidences.end(); ++i)
		counts.insert(std::pair<key_t, size_t>(i->first, i->second.outedges.size()));
	for (auto i = incidences.begin(); i != incidences.end(); ++i)
	{
		for (auto o = i->second.outedges.begin(); o != i->second.outedges.end(); ++o)
			++counts[o->head];
	}
	size_t max_count = 0;
	for (auto c = counts.begin(); c != counts.end(); ++c)
	{
		if (c->second > max_count)
			max_count = c->second;
	}
	return max_count;
}

/** Inserts an edge from 'tail' to 'head', provided such vertices are present in the graph,
 * and assigns it with 'weight', that is equal to 0.0 by default.
 * @param key_t tail - the initial vertex of the edge
 * @param key_t head - the terminal vertex of the edge
 * @param weight_t weight - the weight that should be assigned to the edge
 * @throw error_t(problem_t::out_of_range) - if vertices 'tail' and 'head' are not present in the graph
 */
template<class key_t, class data_t, class weight_t>
void my_graph<key_t, data_t, weight_t>::insert_edge(key_t tail, key_t head, weight_t weight)
{
	if (incidences.find(tail) == incidences.end() or incidences.find(head) == incidences.end())
		throw error_t(problem_t::out_of_range);
	incidences[tail].outedges.emplace_back(edge_t(++edges_count, head, weight));
	++graph_size;
}

 /** Inserts two edges: from 'tail' to 'head' and from 'head' to 'tail',
* and assigns them with 'weight', that is equal to 0.0 by default.
* @param key_t tail - an endpoint of the edges
* @param key_t head - the other endpoint of the edges
* @param weight_t weight - the weight that should be assigned to the edges
* @throw error_t(problem_t::out_of_range) - if vertices 'tail' and 'head' are not present in the graph
*/
template<class key_t, class data_t, class weight_t>
void my_graph<key_t, data_t, weight_t>::insert_undirected_edge(key_t tail, key_t head, weight_t weight)
{
	if (incidences.find(tail) == incidences.end() or incidences.find(head) == incidences.end())
		throw error_t(problem_t::out_of_range);
	incidences[tail].outedges.emplace_back(edge_t(++edges_count, head, weight));
	incidences[head].outedges.emplace_back(edge_t(++edges_count, tail, weight));
	graph_size += 2;
}

/** Returns the ordinal numbers of edges from 'tail' to 'head'.
* @param key_t tail - the initial vertex of the edges
* @param key_t head - the terminal vertex of the edges
* @return the vector of ordinal numbers of the edges
* @throw error_t(problem_t::out_of_range) - if 'tail' or 'head' is not present in the graph
*/
template<class key_t, class data_t, class weight_t>
std::vector<size_t> my_graph<key_t, data_t, weight_t>::edges(key_t tail, key_t head)
{
	if (incidences.find(tail) == incidences.end() or incidences.find(head) == incidences.end())
		throw error_t(problem_t::out_of_range);
	std::vector<size_t> numbers;
	for (auto o = incidences[tail].outedges.begin(); o != incidences[tail].outedges.end(); ++o)
	{
		if (o->head == head)
			numbers.push_back(o->ordinal);
	}
	return numbers;
}

/** Returns the ordinal numbers of edges outgoing from 'tail'.
* @param key_t tail - the initial vertex of the edges
* @return the vector of ordinal numbers of the edges
* @throw error_t(problem_t::out_of_range) - if 'tail' is not present in the graph
*/
template<class key_t, class data_t, class weight_t>
std::vector<size_t> my_graph<key_t, data_t, weight_t>::outedges(key_t tail)
{
	if (incidences.find(tail) == incidences.end())
		throw error_t(problem_t::out_of_range);
	std::vector<size_t> numbers;
	for (auto o = incidences[tail].outedges.begin(); o != incidences[tail].end(); ++o)
		numbers.push_back(o->number);
	return numbers;
}

/** Returns the ordinal numbers of edges leading to 'head'.
* @param key_t head - the terminal vertex of the edges
* @return the vector of ordinal numbers of the edges
* @throw error_t(problem_t::out_of_range) - if 'head' is not present in the graph
*/
template<class key_t, class data_t, class weight_t>
std::vector<size_t> my_graph<key_t, data_t, weight_t>::inedges(key_t head)
{
	if (incidences.find(head) == incidences.end())
		throw error_t(problem_t::out_of_range);
	std::vector<size_t> numbers;
	for (auto i = incidences.begin(); i != incidences.end(); ++i)
	{
		for (auto o = i->second.outedges.begin(); o != i->second.outedges.end(); ++o)
		{
			if (o->ordinal == head)
				numbers.push_back(o->ordinal);
		}
	}
	return numbers;
}

/** Erases the edge with ordinal 'number'. The edge is sought in all the graph.
* @param size_t number - the identifier of the edge that should be erased
*/
template<class key_t, class data_t, class weight_t>
void my_graph<key_t, data_t, weight_t>::erase_edge(size_t number)
{
	if (number > edges_count)
		return;
	for (auto i = incidences.begin(); i != incidences.end(); ++i)
	{
		for (auto o = i->second.outedges.begin(); o != i->second.outedges.end(); ++o)
		{
			if (o->ordinal == number)
			{
				i->second.outedges.erase(o);
				--graph_size;
				return;
			}
		}
	}
}

/** Erases the edge with ordinal 'number'. The edge is sought only among the edges going out from 'tail'.
* @param size_t number - the identifier of the edge that should be erased
* @param key_t tail - the initial vertex of the edge
*/
template<class key_t, class data_t, class weight_t>
void my_graph<key_t, data_t, weight_t>::erase_edge(size_t number, key_t tail)
{
	if (number > edges_count)
		return;
	if (incidences.find(tail) == incidences.end())
		return;
	for (auto it = incidences[tail].outedges.begin(); it != incidences[tail].outedges.end(); ++it)
	{
		if (it->ordinal == number)
		{
			incidences[tail].outedges.erase(it);
			--graph_size;
			return;
		}
	}
}

/** Erases all the edges from vertex 'tail' to 'head'.
* If no such vertices are present in the graph, nothing is done.
* @param key_t tail - the initial vertex of the edge
* @param key_t head - the terminal vertex of the edge
*/
template<class key_t, class data_t, class weight_t>
void my_graph<key_t, data_t, weight_t>::erase_edges(key_t tail, key_t head)
{
	if (incidences.find(tail) == incidences.end() or incidences.find(head) == incidences.end())
		return;
	auto o = incidences[tail].outedges.begin();
	while (o != incidences[tail].outedges.end())
	{
		if (o->head == head)
		{
			--graph_size;
			o = incidences[tail].outedges.erase(o);
		}
		else
			++o;
	}
}

template<class key_t, class data_t, class weight_t>
void my_graph<key_t, data_t, weight_t>::erase_inedges(key_t head)
{
	if (incidences.find(head) == incidences.end())
		return;
	for (auto i = incidences.begin(); i != incidences.end(); ++i)
	{
		for (auto o = i->second.outedges.begin(); o != i->second.outedges.end(); ++o)
		{
			if (o->head == head)
			{
				--graph_size;
				o = i->second.outedges.erase(o);
			}
			else
				++o;
		}
	}
}

template<class key_t, class data_t, class weight_t>
void my_graph<key_t, data_t, weight_t>::erase_outedges(key_t tail)
{
	if (incidences.find(tail) == incidences.end())
		return;
	graph_size -= incidences[tail].outedges.size();
	incidences[tail].outedges.clear();
}

template<class key_t, class data_t, class weight_t>
void my_graph<key_t, data_t, weight_t>::reset_weight(size_t number, weight_t new_weight)
{
	if (number > edges_count)
		throw error_t(problem_t::out_of_range);
	for (auto i = incidences.begin(); i != incidences.end(); ++i)
	{
		for (auto o = i->second.outedges.begin(); o != i->second.outedges.end(); ++o)
		{
			if (o->ordinal == number)
			{
				o->weight = new_weight;
				return;
			}
		}
	}
}

template<class key_t, class data_t, class weight_t>
void my_graph<key_t, data_t, weight_t>::reset_weight(size_t number, weight_t new_weight, key_t tail)
{
	if (number > edges_count)
		throw error_t(problem_t::out_of_range);
	if (incidences.find(tail) == incidences.end())
		return;
	for (auto o = incidences[tail].outedges.begin(); o != incidences[tail].outedges.end(); ++o)
	{
		if (o->ordinal == number)
		{
			o->weight = new_weight;
			return;
		}
	}
}

/** Resets the weight of the edge from 'tail' to 'head'.
* If no such are present in the graph or no edge joins them, nothing is done.
* If more than one such edges are present, the weights of all of them are reset.
* @param key_t tail
* @param key_t head
* @param weight_t new_weight
* @throw
*/
template<class key_t, class data_t, class weight_t>
void my_graph<key_t, data_t, weight_t>::reset_weights(key_t tail, key_t head, weight_t new_weight)
{
	if (incidences.find(tail) == incidences.end() or incidences.find(head) == incidences.end())
		throw error_t(problem_t::out_of_range);
	for (auto it = incidences[tail].outedges.begin(); it != incidences[tail].outedges.end(); ++it)
	{
		if (it->head == head)
			it->weight = new_weight;
	}
}

template<class key_t, class data_t, class weight_t>
weight_t& my_graph<key_t, data_t, weight_t>::edge_weight(size_t number)
{
	if (number > edges_count)
		throw error_t(problem_t::out_of_range);
	for (auto i = incidences.begin(); i != incidences.end(); ++i)
	{
		for (auto o = i->second.outedges.begin(); o != i->second.outedges.end(); ++o)
		{
			if (o->ordinal == number)
				return o->weight;
		}
	}
}

template<class key_t, class data_t, class weight_t>
weight_t& my_graph<key_t, data_t, weight_t>::edge_weight(size_t number, key_t tail)
{
	if (number > edges_count or incidences.find(tail) == incidences.end())
		throw error_t(problem_t::out_of_range);
	for (auto o = incidences[tail].outedges.begin(); o != incidences[tail].outedges.end(); ++o)
	{
		if (o->ordinal == number)
			return o->weight;
	}
}

/** Checks whether the graph is empty.
* @returns 'true' if the graph is empty, 'false' otherwise
*/
template<class key_t, class data_t, class weight_t>
bool my_graph<key_t, data_t, weight_t>::empty()
{
	return incidences.empty();
}
