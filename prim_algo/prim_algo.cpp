// prim_algo.cpp : Defines the entry point for the console application.
//

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <functional>

class mst_solver
{
public:
   mst_solver(const char* input_file);
   long long solve();

   struct edge_from_file
   {
     bool operator<(const edge_from_file & data)
     {
       return weight < data.weight;
     }

     int from_node;
     int to_node;
     int weight;
   };

   struct edge
   {
     edge():node(-1), weight(0){}
		 edge(int a_node, int a_weight):node(a_node), weight(a_weight){}
     edge(const edge_from_file& rhs)
       : node(rhs.to_node-1)
       , weight(rhs.weight)
     {}

     bool operator<(const edge& data)
     {
       return weight < data.weight;
     }

     int node;
     int weight;
   };

   typedef std::vector<edge_from_file>  mst_adopted;;
   typedef std::vector<std::vector<edge> > adjucent_list;
   typedef std::vector<edge> minimal_weight_edges;
   typedef std::vector<bool>  node_set;
    
   int m_nodes_nbr;
   int m_edges_nbr;
   
   adjucent_list				m_graph;
   minimal_weight_edges m_min_edges;
   node_set							m_unhandled_nodes;
   mst_adopted					m_mst_edges;
   edge_from_file				m_init_edge;
 
   /*node_set m_handled_nodes;*/
};

bool operator< (const mst_solver::edge_from_file & lhs, const mst_solver::edge_from_file & rhs)
{
	return lhs.weight < rhs.weight;
}

bool operator< (const mst_solver::edge & lhs, const mst_solver::edge & rhs)
{
	return lhs.weight < rhs.weight;
}

bool operator> (const mst_solver::edge & lhs, const mst_solver::edge & rhs)
{
	return  lhs.weight > rhs.weight;
}
struct edge_adder
{
  edge_adder(mst_solver::adjucent_list& graph, mst_solver::edge_from_file& min_edge)
    : m_graph(graph), m_min_edge(min_edge) {}

  void operator()(const mst_solver::edge_from_file& data) 
  {
		m_min_edge = std::min(m_min_edge, data);
    m_graph[data.from_node-1].push_back(mst_solver::edge(data));
		m_graph[data.to_node-1].push_back(mst_solver::edge(data.from_node-1, data.weight));
  }
  mst_solver::adjucent_list&	m_graph;
  mst_solver::edge_from_file&	m_min_edge;
};

mst_solver::mst_solver( const char* input_file )
{
  std::fstream fin(input_file);
  if(!fin)
    return;

  fin >> m_nodes_nbr >> m_edges_nbr;
  m_graph.resize(m_nodes_nbr);
	m_init_edge.weight = std::numeric_limits<int>::max();

  std::for_each(std::istream_iterator<edge_from_file>(fin), 
    std::istream_iterator<edge_from_file>(), edge_adder(m_graph, m_init_edge));

	m_unhandled_nodes.resize(m_nodes_nbr, true);
}

long long mst_solver::solve()
{
	int current_node = m_init_edge.from_node - 1;
	long long mst_len = 0;
	while(std::any_of(m_unhandled_nodes.begin(), m_unhandled_nodes.end(), [](bool data){return data;}))
	{
		for(auto & data : m_graph[current_node])
		{
			
			if(m_unhandled_nodes[data.node]) 
			{
				m_min_edges.push_back(data);
				std::push_heap(m_min_edges.begin(), m_min_edges.end(), std::greater<edge>());
			}
		}

		m_unhandled_nodes[current_node] = false;

		edge min_edge;
		bool min_edge_found = false;
		while(!m_min_edges.empty())
		{
				pop_heap(m_min_edges.begin(), m_min_edges.end(), std::greater<edge>());
				min_edge = m_min_edges.back();
				m_min_edges.pop_back();
				if(m_unhandled_nodes[min_edge.node])
				{
					min_edge_found=true;
					break;
				}
		}

		if(min_edge_found)
		{
			current_node = min_edge.node;	
			int edge_len = min_edge.weight;
			mst_len += edge_len;
		}
	}
	return mst_len;
}

std::istream & operator>>(std::istream & strm, mst_solver::edge_from_file& data)
{
  strm >> data.from_node >> data.to_node >> data.weight;
  return strm;
}

int main(int argc, char* argv[])
{
  mst_solver solver(argv[1]);
	std::cout << "mst:" << solver.solve() << '\n';
	return 0;
}

