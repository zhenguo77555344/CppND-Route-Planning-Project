#include "route_model.h"
#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
    //Creat RouteModel Nodes
    int counter = 0;
    for(Model::Node node: this->Nodes()){
        m_Nodes.push_back(RouteModel::Node(counter++,this,node));
        //still don't have clear clue about what happens in this code line
        
        //std::cout<<counter<<std::endl;
        //std::cout<<node.x<<","<<node.y<<std::endl;
        //std::cout<<counter<<","<<m_Nodes.at(counter).x<<std::endl;
    }

        /*
        std::cout<<counter<<","<<m_Nodes.at(0).x<<std::endl;
        std::cout<<counter<<","<<m_Nodes.at(1).x<<std::endl;
        std::cout<<counter<<","<<m_Nodes.at(2).x<<std::endl;
        std::cout<<counter<<","<<m_Nodes.at(3).x<<std::endl;
        */
    CreateNodeToRoadHashmap();
/*
    typedef std::unordered_map<int,char> myMap;
    myMap c1;
    c1.insert(myMap::value_type(1,'a'));
    c1.insert(myMap::value_type(2,'b'));
    c1.insert(myMap::value_type(3,'c'));
    //std::cout<<"c1 is:"<<c1.at(1)<<std::endl;
    //std::cout<<"c1 is:"<<c1.at(2)<<std::endl;

    //if(c1.find('a') ==c1.end())
    std::cout<<"c1.find('a')"<<std::boolalpha<<(c1.find(4)==c1.end())<<std::endl;
  */  
}

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

void RouteModel::Node::FindNeighbors(void)
{
	/* Get all of the roads that the current node belongs to */
    //parent_model->node_to_road[this->index];


	for (auto& road : parent_model->node_to_road[this->index])
	{
		/* Access all the indices of the nodes on that road and find the closest one */
		auto* new_neighbor = this->FindNeighbor(parent_model->Ways()[road->way].nodes);
        
        std::cout<<"new_neighbor->(x,y)="<<new_neighbor->x<<","<<new_neighbor->y<<std::endl;


		/* We find a new neighbor, so add it to our neighbors*/
		if (new_neighbor)
		{
			//this->neighbors.emplace_back(new_neighbor);
            this->neighbors.push_back(new_neighbor);
                   
		}
        
	}

    std::cout<<"this->neighbors.size()="<<this->neighbors.size()<<std::endl;
    std::cout<<"this->neighbors.[0]->(x,y)="<<this->neighbors[0]->x<<","<<this->neighbors[0]->y<<std::endl;
    std::cout<<"this->neighbors.[1]->(x,y)="<<this->neighbors[1]->x<<","<<this->neighbors[1]->y<<std::endl;

    
}


RouteModel::Node& RouteModel::FindClosestNode(float x, float y){
    Node node_temp;
    node_temp.x = x;
    node_temp.y = y;

    float min_dist = std::numeric_limits<float>::max();
    int closest_idx = std::numeric_limits<int>::max();

    for(auto i:node_to_road){
       float current_dis = SNodes()[i.first].distance(node_temp);
       if(current_dis < min_dist){
           min_dist = current_dis;
           closest_idx = i.first;
       }
    } 
    return SNodes()[closest_idx];
}

