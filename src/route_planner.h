#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "route_model.h"


class RoutePlanner {
  public:
    RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y);
    // Add public variables or methods declarations here.
    void AStarSearch(void);
    float GetDistance(){return distance;}

  //private:
    std::vector<RouteModel::Node> ConstructFinalPath(RouteModel::Node* current_note);
    float CalculateHvalue(const RouteModel::Node);
    void AddNeighbors(RouteModel::Node* );
    RouteModel::Node* NextNode();

  private:
    // Add private variables or methods declarations here.
    RouteModel &m_Model;
    RouteModel::Node* start_node = nullptr;
    RouteModel::Node* end_node = nullptr;
    float distance;
    std::vector<RouteModel::Node*> open_list;

};
