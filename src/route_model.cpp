#include "route_model.h"
#include <iostream>
using namespace std;

void RouteModel::Node::find_neighbors(void){
    	
    /* Get all of the roads that the current node belongs to */
    for (auto& road : parent_model->node_to_road[this->index]){
		
        /* Access all the indices of the nodes on that road and find the closest one */
		auto* new_neighbor = this->find_neighbor(parent_model->Ways()[road->way].nodes);
        
		/* We find a new neighbor, so add it to our neighbors*/
        
		if (new_neighbor){
			
            this->neighbors.push_back(new_neighbor);
            std::cout<<"new_neighbor->(x,y)="<<new_neighbor->x<<","<<new_neighbor->y<<std::endl;

		}
	}    
    //std::cout<<">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<<std::endl;
}

RouteModel::Node::Node(int idx, RouteModel* search_model, Model::Node node) : Model::Node(node),parent_model(search_model), index(idx){}

RouteModel::Node* RouteModel::Node::find_neighbor(std::vector<int> node_indices) const
{
	Node* closest_node = nullptr;

	/* Go through all node indices and find closest */
	for (auto node_idx : node_indices)
	{
		Node node = parent_model->SNodes()[node_idx];

		/* Do not return the node itself and check it hasn't been visited so far */
		if ((this->distance(node) != 0) && (!node.visited))
		{
			/* If there is no closest node or current one is closer than previous */
			if ((closest_node == nullptr) || (this->distance(node) < this->distance(*closest_node)))
			{
				/* Set our closest node to the address of new closest node */
				closest_node = &parent_model->SNodes()[node_idx];
			}
		}
	}

	return closest_node;
}


RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
    //Creat RouteModel Nodes
    // Add all nodes to the vector of nodes
    int counter = 0;
    for(Model::Node node: this->Nodes()){
        m_nodes.push_back(RouteModel::Node(counter++,this,node));
    }
    
    //Find all roads that correspond to those nodes */
    create_node_to_road_hashmap();

}

RouteModel::Node& RouteModel::find_closest_node(float x, float y)
{
	Node input;
	input.x = x;
	input.y = y;

	float min_dist = std::numeric_limits<float>::max();
	int closest_idx = 0;

	for (auto& road : Roads())
	{
		if (road.type != Road::Type::Footway)
		{
			for (auto node_idx : Ways()[road.way].nodes)
			{
				float dist = input.distance(SNodes()[node_idx]);
				if (dist < min_dist)
				{
					closest_idx = node_idx;
					min_dist = dist;
				}
			}
		}
	}

	return SNodes()[closest_idx];
}



/*
RouteModel::Node& RouteModel::FindClosestNode(float x, float y){
    Node node_temp;
    node_temp.x = x;
    node_temp.y = y;

    float min_dist = std::numeric_limits<float>::max();
    int closest_idx =0;// std::numeric_limits<int>::max();

    for(auto i:node_to_road){
       float current_dis = SNodes()[i.first].distance(node_temp);
       if(current_dis < min_dist){
           min_dist = current_dis;
           closest_idx = i.first;
       }
    } 
    return SNodes()[closest_idx];
}*/


void RouteModel::create_node_to_road_hashmap(void)
{
	/* Go through all roads */
	for (auto& road : Roads())
	{
		/* Exclude footways */
		if (road.type != Road::Type::Footway)
		{
			/* For each node along the road, get our way and his nodes as int's */
			for (auto node_idx : Ways()[road.way].nodes)
			{
				/* If our node to road does not have this node in it, create an empty road first */
				if (node_to_road.find(node_idx) == node_to_road.end())
				{
					node_to_road[node_idx] = std::vector<const Model::Road*>();
				}
				/* Afterwards push the address of that road to the hashmap */
				node_to_road[node_idx].push_back(&road);
			}
		}
	}
}



/*
void RouteModel::CreateNodeToRoadHashmap(void){
    for(const Model::Road &this_road: this->Roads()){
        if(this_road.type != Model::Road::Type::Footway){
            for(int node_idx :this->Ways()[this_road.way].nodes){
                //find untill the end of map means can not find
                if(node_to_road.find(node_idx) == node_to_road.end()){
                    node_to_road[node_idx] = std::vector<const Model::Road *> ();
                    //node_to_road.at(node_idx) = std::vector<const Model::Road *> ();//C++ exception with description "_Map_base::at" thrown in the test fixture's constructor.           
                }
                // set up the link between nodes and road adress
                node_to_road[node_idx].push_back(&this_road);
            }
        }
    }
}

*/



/*
RouteModel::Node* RouteModel::Node::FindNeighbor(std::vector<int> node_indices){
    Node* closest_node = nullptr;

    for(int index:node_indices){
        auto current_node = parent_model->SNodes()[index];
        float dis_current_to_this = RouteModel::Node::distance(current_node);

        if(current_node.visited == false && dis_current_to_this>1e-5){
            float dis_closest_to_this = RouteModel::Node::distance(current_node);
           if(closest_node == nullptr || dis_current_to_this < dis_closest_to_this){
               closest_node = &parent_model->SNodes()[index];
           }
        }
    }
    return closest_node;
}
*/





/*
RouteModel::Node* RouteModel::Node::FindNeighbor(std::vector<int> node_indices){
    Node* closest_node = nullptr;
    float minDistance = std::numeric_limits<float>::max();

    auto nodelist = parent_model->SNodes();

    for(int index:node_indices){

        if(nodelist[index].visited== false){

            float currentDist = distance(nodelist[index]);

            if(currentDist < minDistance && currentDist !=0){

                minDistance = currentDist;
                closest_node =&nodelist[index]; 
            }
        }
    }
    return closest_node;
}
*/
/*
void RouteModel::Node::FindNeighbors(void){ 
    //Node* node= &parent_model->node_to_road[this->index];
    for(auto road:parent_model->node_to_road[this->index]){
        //Node* pNode = FindNeighbor(road->way);
        Node* pNode = FindNeighbor(parent_model->Ways()[road->way].nodes);
        if(pNode!=nullptr){
            //this->neighbors.push_back(pNode);
            this->neighbors.emplace_back(pNode);
        }
    }
}
*/




