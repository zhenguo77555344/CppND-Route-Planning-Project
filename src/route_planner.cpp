#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    start_x *=0.01;
    start_y *=0.01;
    end_x *=0.01;
    end_y *=0.01;

    start_node = &m_Model.FindClosestNode(start_x,start_y);
    end_node   = &m_Model.FindClosestNode(end_x,end_y);


}

 std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node* current_note){
     
     std::vector<RouteModel::Node> path_found;
     this->distance =0.f;
     RouteModel::Node parent;
     
     while(current_note->parent!=nullptr){
         
         path_found.push_back(*current_note);
         parent = *(current_note->parent);
         this->distance += current_note->distance(parent);

         current_note = current_note->parent;
     } 

     path_found.push_back(*current_note);
     distance *= m_Model.MetricScale();

    return path_found;
 }

void RoutePlanner::AStarSearch(void){
    start_node = end_node->parent; 
    m_Model.path = ConstructFinalPath(end_node);

}