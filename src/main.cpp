#include <optional>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <io2d.h>
#include "route_model.h"
#include "render.h"
#include "route_planner.h"

using namespace std::experimental;

#define ROUTE_BUTTON 1

static std::optional<std::vector<std::byte>> ReadFile(const std::string &path)
{   
    std::ifstream is{path, std::ios::binary | std::ios::ate};
    if( !is )
        return std::nullopt;
    
    auto size = is.tellg();
    std::vector<std::byte> contents(size);    
    
    is.seekg(0);
    is.read((char*)contents.data(), size);

    if( contents.empty() )
        return std::nullopt;
    return std::move(contents);
}

int main(int argc, const char **argv)
{    
    std::string osm_data_file = "";
    if( argc > 1 ) {
        for( int i = 1; i < argc; ++i )
            if( std::string_view{argv[i]} == "-f" && ++i < argc )
                osm_data_file = argv[i];
    }
    else {
        std::cout << "Usage: [executable] [-f filename.osm]" << std::endl;    
    }
    
    std::vector<std::byte> osm_data;
 
    if( osm_data.empty() && !osm_data_file.empty() ) {
        std::cout << "Reading OpenStreetMap data from the following file: " <<  osm_data_file << std::endl;
        auto data = ReadFile(osm_data_file);
        if( !data )
            std::cout << "Failed to read." << std::endl;
        else
            osm_data = std::move(*data);
    }

    // Build Model.
    RouteModel model{osm_data};

#if ROUTE_BUTTON
    float start_x,start_y,end_x,end_y;
    //std::cout<<"Please Input `start_x`,`start_y`,`end_x`,and `end_y` "<<std::endl;
    //std::cin>>start_x>>start_y>>end_x>>end_y; 
    // search the route

    start_x = 5;
    start_y = 5;
    end_x =60;
    end_y =50;

    RoutePlanner route_planner{model, start_x, start_y, end_x, end_y};
    route_planner.AStarSearch();
    std::cout<<"start_x="<<start_x<<"start_y="<<start_y<<"end_x="<<end_x<<"end_y="<<end_y;
    //std::cout<<"Distance: "<<route_planner.GetDistance()<<"meters."<<std::endl;
#endif

    // Perform search and render results.
    Render render{model};

    auto display = io2d::output_surface{800, 800, io2d::format::argb32, io2d::scaling::none, io2d::refresh_style::fixed, 30};

    display.size_change_callback([](io2d::output_surface& surface){
        surface.dimensions(surface.display_dimensions());
    });
    display.draw_callback([&](io2d::output_surface& surface){
        render.Display(surface);
    });
    display.begin_show();
}
