/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/topological_sort.hpp"

#include <deque>
#include <iostream>
#include <iterator>
#include <tuple>
#include <vector>

/**
 *  \brief create a directed graph
 */
using EdgeWeightProperty = boost::property<boost::edge_weight_t, int>;
using DirectedGraph = boost::adjacency_list<boost::listS, boost::vecS, boost::directedS, boost::no_property, EdgeWeightProperty>;
using edge_iterator = boost::graph_traits<DirectedGraph>::edge_iterator;

TEST(boost, directed_graph) {
  std::vector<std::tuple<int, int, int>> uvps{ 
    {0, 1, 8}, {0, 3, 18}, {1, 2, 20}, {2, 3, 2}, {3, 1, 1}, 
    {1, 3, 7}, {1, 4, 1}, {4, 5, 6}, {2, 5, 7} };
  
  DirectedGraph g;
  for (auto [u, v, p] : uvps) {
    boost::add_edge(u, v, p, g);
  }
  ASSERT_EQ(9, boost::num_edges(g)) << "Number of edges";

  std::cout << "Edge list:\n";
  std::pair<edge_iterator, edge_iterator> ei = boost::edges(g);

  std::copy(ei.first, ei.second,
    std::ostream_iterator<boost::adjacency_list<>::edge_descriptor>{
    std::cout, "\n"});
}
