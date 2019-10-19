#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    start_x *=0.01;
    start_y *=0.01;
    end_x *=0.01;
    end_y *=0.01;

    start_node = &m_Model.FindClosestNode(start_x,start_y);
    end_node   = &m_Model.FindClosestNode(end_x,end_y);

    //std::cout<<"start_node = "<<start_node<<std::endl;
    //std::cout<<"end_node = "<<end_node<<std::endl;


}

 std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node* current_note){
     
     std::vector<RouteModel::Node> path_found;
     this->distance =0.f;
     RouteModel::Node parent;
     
     //std::cout<<"current_note->parent = "<<current_note->parent<<std::endl;
     while(current_note->parent != nullptr){
         
         path_found.push_back(*current_note);
         parent = *(current_note->parent);
         this->distance += current_note->distance(parent);

         current_note = current_note->parent;
         
         //std::cout<<"path_found.size() in while = "<<path_found.size()<<std::endl;

     } 

     path_found.push_back(*current_note);
     distance *= m_Model.MetricScale();

     //std::cout<<"path_found"<<path_found[0].x<<path_found[1].x<<std::endl;
    //std::cout<<"path_found.size() = "<<path_found.size()<<std::endl;
    return path_found;
 }

void RoutePlanner::AStarSearch(void){
    end_node->parent = start_node; 
    m_Model.path = ConstructFinalPath(end_node);

    //std::cout<<"m_Model.path.size()="<<m_Model.path.size()<<std::endl;
}