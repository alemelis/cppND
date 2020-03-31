#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y,
                           float end_x, float end_y)
    : m_Model(model) {
  // Convert inputs to percentage:
  start_x *= 0.01;
  start_y *= 0.01;
  end_x *= 0.01;
  end_y *= 0.01;

  // Use the m_Model.FindClosestNode method to find the closest nodes to the
  // starting and ending coordinates. Store the nodes you find in the
  // RoutePlanner's start_node and end_node attributes.
  start_node = &m_Model.FindClosestNode(start_x, start_y);
  end_node = &m_Model.FindClosestNode(end_x, end_y);
}

// Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another
// node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
  return node->distance(*end_node);
}

// Complete the AddNeighbors method to expand the current node by adding all
// unvisited neighbors to the open list. Tips:
// - Use the FindNeighbors() method of the current_node to populate
// current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the
// g_value.
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and
// set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
  current_node->FindNeighbors();
  RouteModel::Node *neighbor = nullptr;
  for (int i = 0; i < current_node->neighbors.size(); i++) {
    neighbor = current_node->neighbors[i];
    neighbor->parent = current_node;
    neighbor->g_value =
        current_node->g_value + current_node->distance(*neighbor);
    neighbor->h_value = CalculateHValue(neighbor);
    open_list.push_back(neighbor);
    neighbor->visited = true;
  }
}

// Complete the NextNode method to sort the open list and return the
// next node. Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

// https://www.fluentcpp.com/2018/09/18/how-to-remove-pointers-from-a-vector-in-cpp/
void clean_nullptr(std::vector<RouteModel::Node *> &open_list, int rm_idx) {
  open_list[rm_idx] = nullptr;
  open_list.erase(std::remove(open_list.begin(), open_list.end(), nullptr),
                  open_list.end());
}

RouteModel::Node *RoutePlanner::NextNode() {
  int min_sum_idx;
  float min_sum = 1000.0;
  float f;
  for (int i = 0; i < open_list.size(); i++) {
    f = open_list[i]->g_value + open_list[i]->h_value;
    if (f < min_sum) {
      min_sum = f;
      min_sum_idx = i;
    }
  }
  RouteModel::Node *pointer_to_min_sum_node = open_list[min_sum_idx];
  clean_nullptr(open_list, min_sum_idx);
  return pointer_to_min_sum_node;
}

// Complete the ConstructFinalPath method to return the final path found
// from your A* search. Tips:
// - This method should take the current (final) node as an argument and
// iteratively follow the
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to
// the distance variable.
// - The returned vector should be in the correct order: the start node should
// be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node>
RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
  // Create path_found vector
  distance = 0.0f;
  std::vector<RouteModel::Node> path_found;

  // Implement your solution here.
  path_found.push_back(*current_node);
  while (current_node->parent) {
    distance += current_node->distance(*current_node->parent);
    path_found.push_back(*current_node->parent);
    current_node = current_node->parent;
  }

  distance *= m_Model.MetricScale(); // Multiply the distance by the scale of
                                     // the map to get meters.

  std::reverse(path_found.begin(), path_found.end());
  return path_found;
}

// Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node
// to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method
// to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits.
// This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {
  RouteModel::Node *current_node = nullptr;

  // Implement your solution here.
  current_node = start_node;
  current_node->visited = true;
  AddNeighbors(current_node);
  while (!open_list.empty()) {
    RouteModel::Node *next_node = NextNode();
    if (next_node->x == end_node->x && next_node->y == end_node->y) {
      break;
    }
    AddNeighbors(next_node);
  }
  std::vector<RouteModel::Node> final_path = ConstructFinalPath(end_node);
  m_Model.path = final_path;
}
