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
     
     std::cout<<"current_note->parent = "<<current_note->parent<<std::endl;
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
    std::cout<<"path_found.size() = "<<path_found.size()<<std::endl;
    return path_found;
 }

void RoutePlanner::AStarSearch(void){
    //end_node->parent = start_node; 
    //m_Model.path = ConstructFinalPath(end_node);
    //std::cout<<"m_Model.path.size()="<<m_Model.path.size()<<std::endl;

    start_node->visited = true;

    start_node->g_value = 0;
    start_node->h_value = start_node->distance(*end_node);

    //std::cout<<"start_node->h_value ="<<start_node->h_value<<std::endl;

    //open_list.push_back(start_node);
    open_list.emplace_back(start_node);

    //RouteModel::Node* current_node = nullptr;
    std::cout<<"open_list.size()="<<open_list.size()<<std::endl;

    while (open_list.size() > 0){   
        
        /* Find best next node for exploration*/
        RouteModel::Node* current_node = NextNode();//sort
        //std::cout<<"current_node->(g_value,h_value) = "<<current_node->g_value<<","<<current_node->h_value<<std::endl;
       //std::cout<<"current_node->(x,y)="<<current_node->x<<","<<current_node->y<<std::endl;
       
       //std::cout<<"start_node-(x,y)="<<start_node->x<<","<<start_node->y<<std::endl;
        //std::cout<<"current_node->distance(*end_node)= "<<current_node->distance(*end_node)<<std::endl;
        
       
        //std::cout<<"end_node-(x,y)="<<end_node->x<<","<<end_node->y<<std::endl;

        if(current_node->distance(*end_node) <1e-5 ){
            m_Model.path = ConstructFinalPath(current_node);
            //std::cout<<"m_Model.path.size()="<<m_Model.path.size()<<std::endl;
            return;
        }

        AddNeighbors(current_node);//this function has issue
        //std::cout<<"open_list.size()="<<open_list.size()<<std::endl;
    }

    //std::cout<<"open_list.size()---="<<open_list.size()<<std::endl;
    //return;  
}

float RoutePlanner::CalculateHvalue(const RouteModel::Node current_note){

    return current_note.distance(*end_node);
}

void RoutePlanner::AddNeighbors(RouteModel::Node* current_node){
    current_node->FindNeighbors();

    //std::cout<<"current_node->neighbors.size()="<<current_node->neighbors.size()<<std::endl;//current_node->neighbors.size() always ==1, this is an issue
    //std::cout<<"current_node->(x,y) in Add="<<current_node->x<<","<<current_node->y<<std::endl;
      
    for(auto neighbor:current_node->neighbors){
        
        neighbor->parent = current_node;
        neighbor->g_value = current_node->g_value + current_node->distance(*neighbor);
        neighbor->h_value = CalculateHvalue(*neighbor);

        //std::cout<<"current_node->distance(*neighbor)= "<<current_node->distance(*neighbor)<<std::endl;
        
        //open_list.push_back(neighbor);
        open_list.emplace_back(neighbor);
        neighbor->visited = true;
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
	//open_list.erase(open_list.begin());
	return lowest_node;


/*
    RouteModel::Node* node_point;
    for(auto node:open_list){
         float min_f_value = std::numeric_limits<float>::max();
         float f_value = node->g_value + node->h_value;

         if(f_value < min_f_value){
            min_f_value = f_value;
            node_point = node;
         }
    }
    open_list.erase(node_point);    
   return node_point;
*/
/*
    std::sort(open_list.begin(),open_list.end(),[](const auto &_1st,const auto &_2nd){
        return (_1st->h_value + _1st->g_value) < (_2nd->h_value + _2nd->g_value);
    });

    RouteModel::Node * result = open_list.front();
    open_list.erase(open_list.begin());

    return result;

    */




}


