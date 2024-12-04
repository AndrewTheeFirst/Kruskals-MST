#include "graph.h"
#include "disjointset.h"
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>


Graph::Graph(const std::string& filename){
    std::string line, city, distance;
    std::ifstream file(filename);
    std::istringstream line_stream;
    std::vector<int> distances;

    std::getline(file, line); // discards the first line in the txt file

    for(size_t line_index = 0; std::getline(file, line); line_index++){
        std::istringstream line_stream(line);
        line_stream >> city;
        label_to_vertex_map.insert({city, line_index});
        vertex_to_label_map.push_back(city);
        
        while (line_stream >> distance){
            if (distance == "-"){
                distances.push_back(NO_CONNECTION);
            }
            else{
                distances.push_back(std::stoi(distance));
            }
        }
        adjacency_matrix.push_back(distances);
    }

}

size_t Graph::size() const{
    return vertex_to_label_map.size();
}

int Graph::weight() const {
    size_t num_cities = adjacency_matrix.size();
    int total_weight = 0;
    for (size_t city_index = 0; city_index < num_cities; city_index++){
        for (size_t weight_index = 0;  adjacency_matrix[city_index].size(); weight_index++){
            total_weight += adjacency_matrix[city_index][weight_index];
        }
    }
    return total_weight;
}

int Graph::breadth_first_path_weight(const std::string& begin, const std::string& end){
    return 0;
}

// struct compare_function{
//     bool operator()(HuffmanNode* node1, HuffmanNode* node2){
//         return (node1->get_frequency() > node2->get_frequency());
//     }
// };

// create a local struct to represent an edge: start, end, weight
// use the indicies in the vertex_to_label in the disjoint set
// 


Graph Graph::kruskal(){

    
    Graph mst(*this);
    
    return mst;
}

std::ostream& operator<<(std::ostream& os, const Graph& g){
    return os;
}

struct edge{
    std::string from;
    std::string to;
    int weight;
    edge(std::string from, std::string to, int weight):
    from(from), to(to), weight(weight){}
};