#include "route_planner.h"
#include <algorithm>
#include<iostream>

using namespace std;

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    start_x *=0.01;
    start_y *=0.01;
    end_x *=0.01;
    end_y *=0.01;

    start_node = &m_Model.find_closest_node(start_x,start_y);
    end_node   = &m_Model.find_closest_node(end_x,end_y);

    std::cout<<"start_node(x,y) = "<<start_node->x<<","<<start_node->y<<std::endl;

}

void RoutePlanner::AStarSearch(){

    //Start with starting node
    start_node->visited = true;
    open_list.emplace_back(start_node);

    while (!open_list.empty()){

        // Find best next node for exploration
        RouteModel::Node* current_node = NextNode();//open_list

        //std::cout<<"current_node->(x,y)="<<current_node->x<<","<<current_node->y<<std::endl;

        if(current_node->distance(*end_node) == 0 ){
            m_Model.path = ConstructFinalPath(end_node);
           return;
        }

        //Otherwise, add current node to the neighbors
        AddNeighbors(current_node);//this function with issue
    }

}// end of AStarSearch

void RoutePlanner::AddNeighbors(RouteModel::Node* current_node){
    
    //Populate current node's neighbors vector
    current_node->find_neighbors();//this function with issue

    for(auto neighbor:current_node->neighbors){
        
        neighbor->parent = current_node;
        neighbor->g_value = current_node->g_value + current_node->distance(*neighbor);
        neighbor->h_value = CalculateHvalue(neighbor);

        open_list.emplace_back(neighbor);
        neighbor->visited = true;
        
        //std::cout<<"neighbor->(h_value,g_value),H+G= "<<neighbor->h_value <<","<<neighbor->g_value<<","<<neighbor->h_value+neighbor->g_value<<std::endl;  
    
    }
}

RouteModel::Node* RoutePlanner::NextNode(){

/* Sort our list of open nodes by their sum of h and g value */
	std::sort(open_list.begin(), open_list.end(), [](const auto & _1st, const auto & _2nd)
	{
		return (_1st->h_value + _1st->g_value) < (_2nd->h_value + _2nd->g_value);
	});

	/* Get the node with lowest value and remove it from the list */
	RouteModel::Node* lowest_node = open_list.front();
	open_list.erase(open_list.begin());
	return lowest_node;
}

float RoutePlanner::CalculateHvalue( RouteModel::Node* node) const{
    return node->distance(*end_node);
}

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node* current_node){
    distance = 0.0f;
	std::vector<RouteModel::Node> path_found;

	/* Repeat as long as we are not at the starting point */
	while(current_node->parent != nullptr)
	{
		path_found.emplace_back(*current_node);
		const RouteModel::Node parent = *(current_node->parent);
		distance += current_node->distance(parent);
		current_node = current_node->parent;
	}

	/* Add start node and multiply with metric scale to convert to meters */
	path_found.emplace_back(*current_node);
	distance *= m_Model.MetricScale();
	return path_found;
 }








