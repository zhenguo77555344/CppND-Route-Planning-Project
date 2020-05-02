#pragma once

#include <limits>
#include <cmath>
#include <unordered_map>
#include "model.h"

class RouteModel : public Model 
{

public:
    RouteModel(const std::vector<std::byte>& xml);	

public:
	class Node : public Model::Node {
	public:

		Node() = default;
		Node(int idx, RouteModel* search_model, Model::Node node);

	public:
		void find_neighbors();
		float distance (Node other) const
		{
			return std::sqrt(std::pow((x - other.x), 2) + std::pow((y - other.y), 2));
		}

	public:
		Node* parent = nullptr;
		float h_value = std::numeric_limits<float>::max();
		float g_value = 0.0;
		bool visited = false;
		std::vector<Node*> neighbors;

	private:
		Node* find_neighbor(std::vector<int> node_indices) const;
	
	private:
		int index{};
		RouteModel* parent_model = nullptr;
		
	};

	
	Node& find_closest_node(float x, float y);

	auto& SNodes(){
		return m_nodes;
	}

	auto& get_node_to_roadmap(){
		return node_to_road;
	}
	
public:	
	std::vector<Node> path;


private: 
	void create_node_to_road_hashmap();

private:
	std::unordered_map<int, std::vector<const Model::Road*>> node_to_road;
	std::vector<Node> m_nodes;

};
