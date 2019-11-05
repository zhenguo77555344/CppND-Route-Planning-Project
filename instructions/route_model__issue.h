#pragma once

#include <limits>
#include <cmath>
#include <unordered_map>
#include "model.h"
#include <iostream>

class RouteModel : public Model {

  public:
    class Node : public Model::Node {
      public:
        // Add public Node variables and methods here.
        
        Node(){}
        //Node(int idx, RouteModel * search_model, Model::Node node) : Model::Node(node), parent_model(search_model), index(idx) {}
        //Node(int idx, RouteModel * search_model, Model::Node node) : index(idx),parent_model(search_model), Model::Node(node){}
        Node(int idx, RouteModel* search_model, Model::Node node);
        /*
        Node(int idx, RouteModel * search_model, Model::Node node){
          index = idx;
          parent_model = search_model;
          Model::Node = node;// this is an error, will not pass the compilation process
       }*/
      public:
        float distance(RouteModel::Node other) const{          
          //return sqrtf((this->x-node.x)*(this->x-node.x)+(this->y-node.y)*(this->y-node.y));
          return std::sqrt(std::pow((x - other.x), 2) + std::pow((y - other.y), 2));
        } 
        void FindNeighbors(void);

      private:
        RouteModel::Node * FindNeighbor(std::vector<int> node_indices) const;     

      public:
        //1.Add the following variables to the RouteModel::Node class
        Node * parent = nullptr;
        float h_value = std::numeric_limits<float>::max();
        float g_value = 0.0;
        bool visited = false;
        std::vector<Node*> neighbors;

      private:
        // Add private Node variables and methods here.
        int index;
        RouteModel * parent_model = nullptr;
    };
    
    // Add public RouteModel variables and methods here.
    RouteModel(const std::vector<std::byte> &xml);  
    std::vector<Node> path; // This variable will eventually store the path that is found by the A* search.
  private:
    void CreateNodeToRoadHashmap(void);
    
  public:
    std::unordered_map<int,std::vector<const Model::Road*>> &GetNodeToRoadMap(){
      return node_to_road;
    }
    

  public:
     std::vector<Node> SNodes(){
       return m_Nodes;
     }
  public:
     Node& FindClosestNode(float x, float y);
  
  private:
    // Add private RouteModel variables and methods here.
    std::vector<Node> m_Nodes;
    std::unordered_map<int,std::vector<const Model::Road* >> node_to_road;

};
