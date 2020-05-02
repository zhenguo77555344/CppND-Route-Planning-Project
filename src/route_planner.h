#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "route_model.h"


class RoutePlanner {
  public:
    RoutePlanner() = default; 
    RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y);
    
  public:
    float GetDistance(){return distance;}
    void AStarSearch();
    
  private:
    void AddNeighbors(RouteModel::Node* current_node);
    RouteModel::Node* NextNode();
    float CalculateHvalue(RouteModel::Node* node) const;
    std::vector<RouteModel::Node> ConstructFinalPath(RouteModel::Node* current_note);
    
  private:
    std::vector<RouteModel::Node*> open_list;
    RouteModel &m_Model;
    //RouteModel::Node* start_node{}; 
    //RouteModel::Node* end_node{};
    RouteModel::Node* start_node = nullptr;
    RouteModel::Node* end_node = nullptr;
    float distance;
};
