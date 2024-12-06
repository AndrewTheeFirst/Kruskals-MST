#include "graph.h"
#include "disjointset.h"
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <iomanip>

#include <iostream>

struct edge{
    size_t vertex_1, vertex_2, weight;

    edge(size_t v1, size_t v2, size_t w): vertex_1(v1), vertex_2(v2), weight(w){}
};

struct compare_function{
    bool operator()(edge* edge_1, edge* edge_2){
        // True means e1 is heavier than e2, False means e1 is lighter than e2. 
        // Therefore we want e1 to be heavier when e1 > e2
        return edge_1->weight > edge_2->weight; 
    }
};

Graph::Graph(const std::string& filename){
    std::string line, city, distance;
    std::ifstream file(filename);
    std::istringstream line_stream;
    std::vector<int> distances;

    std::getline(file, line); // discards the first line in the txt file

    for(size_t line_index = 0; std::getline(file, line); line_index++){
        line_stream = std::istringstream(line);
        line_stream >> city; // first word of each line is a city
        this->label_to_vertex_map.insert({city, line_index});
        this->vertex_to_label_map.push_back(city);
        distances = std::vector<int>(); // ensures vector is clear before every loop
        while (line_stream >> distance){
            if (distance == "-"){
                distances.push_back(NO_CONNECTION);
            }
            else{
                distances.push_back(std::stoi(distance));
            }
        }
        this->adjacency_matrix.push_back(distances);
    }
}

size_t Graph::size() const{
    return vertex_to_label_map.size();
}

int Graph::weight() const {
    size_t num_cities = this->adjacency_matrix.size();
    int weight = 0;
    int total_weight = 0;
    for (size_t city_index = 0; city_index < num_cities; city_index++){
        for (size_t weight_index = city_index + 1; weight_index < this->adjacency_matrix[city_index].size(); weight_index++){
            weight = this->adjacency_matrix[city_index][weight_index];
            if (weight != NO_CONNECTION){
                total_weight += this->adjacency_matrix[city_index][weight_index];
            }
        }
    }
    return total_weight;
}

int Graph::breadth_first_path_weight(const std::string& begin, const std::string& end){
    size_t start = this->label_to_vertex_map[begin];
    size_t dest = this->label_to_vertex_map[end];

    size_t num_cities = this->size();
    std::vector<size_t> path;
    size_t city;
    int weight = 0;
    std::vector<bool> visited(num_cities, false); // initializes an array with size 'num_cities' to all zeros

    visited[start] = 1;
    bool done = false;
    std::queue<size_t> queue;
    queue.push(start);

    while(!queue.empty() && !done){
        city = queue.back(); queue.pop();
        path.push_back(city);
        for (size_t _city = 0; _city < this->adjacency_matrix[city].size(); _city++){
            if (this->adjacency_matrix[city][_city] != NO_CONNECTION && !visited[_city]){
                visited[_city] = true;
                queue.push(_city);
                if (_city == dest){
                    done = true;
                    path.push_back(_city);
                }
            }
        }
    }

    if (done){ 
        for (size_t index = 0; index < path.size() - 1; index++){
        weight += adjacency_matrix[path[index]][path[index + 1]];
        }
    } else { weight = NO_CONNECTION; } // if all paths were checked and dest was not found
    
    return weight;
}

Graph Graph::kruskal(){
    std::priority_queue<edge*, std::vector<edge*>, compare_function> queue;
    std::vector<int> adj_vector;
    size_t num_verticies = this->size();
    DisjointSet connections(num_verticies);

    for (size_t index = 0; index < num_verticies; index++){
        connections.Make_Set(index); // creates the forest of trees
        adj_vector = this->adjacency_matrix[index];
        for (size_t connection_index = index + 1; connection_index < adj_vector.size(); connection_index++){ // make sure to skip dupes
            edge* route = new edge(index, connection_index, adj_vector[connection_index]); 
            queue.push(route); // adds all edges into the priority queue
        }
    }

    Graph mst(*this); // uses copy constructor
    while (!queue.empty()){ // essentially a c&p from kruskal algorithm
        edge* route = queue.top(); queue.pop();
        size_t city_1 = route->vertex_1,
               city_2 = route->vertex_2;
        if (connections.Find(city_1) != connections.Find(city_2)){
            connections.Union(city_1, city_2);
        }
        else{
            mst.adjacency_matrix[city_1][city_2] = NO_CONNECTION; // updates the two places in the adjacency matrix that the edge represents
            mst.adjacency_matrix[city_2][city_1] = NO_CONNECTION;
        }
    }
    return mst;
}

std::ostream& operator<<(std::ostream& os, const Graph& g){
    size_t num_cities = g.adjacency_matrix.size();
    const std::streamsize SPACE = 12;
    std::vector<int> weights;
    int weight;
    std::string str_weight;
    if (num_cities > 0){
        std::string city;
        os << std::setw(SPACE) << ""; // spacer for header
        for (size_t index_1 = 0; index_1 < num_cities - 1; index_1++){ // prints the header (cities)
            city = g.vertex_to_label_map[index_1];
            os << std::right << std::setw(SPACE) << city + " ";
        }
        os << std::setw(SPACE) << g.vertex_to_label_map[num_cities - 1] + " "<< '\n'; // puts last city

        os << std::setw(SPACE - 1) << ""; // spacer for header
        os << std::left << std::setfill('-') << std::setw(SPACE * num_cities) << '+' << '\n'; // fills in the dashes

        for (size_t index_1 = 0; index_1 < num_cities; index_1++){
            city = g.vertex_to_label_map[index_1];
            os << std::right << std::setfill(' ') << std::setw(SPACE) << city + " |"; // left city label
            weights = g.adjacency_matrix[index_1];
            if (weights.size() > 0){
                weight = weights[0];
                if (weight == g.NO_CONNECTION){ str_weight = "-"; } 
                else { str_weight = std::to_string(weight); }
                os << std::setw(SPACE - 1) << str_weight + " ";
                for (size_t weight_index = 1; weight_index < weights.size(); weight_index++){ // prints weights for the city
                    weight = weights[weight_index];
                    if (weight == g.NO_CONNECTION){ str_weight = "-"; }
                    else { str_weight = std::to_string(weight); }
                    os << std::setw(SPACE) << str_weight + " ";
                }
            }
            os << "\n";
        }
    }
    return os;
}