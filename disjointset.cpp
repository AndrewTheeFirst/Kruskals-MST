#include <string>
#include <vector>
#include <unordered_map>
#include "disjointset.h"

void DisjointSet::Make_Set(int vertex){
    parents.push_back(vertex);
    ranks.push_back(vertex);
};

int DisjointSet::Find(int vertex){ // uses path compression
    if (parents[vertex] != vertex){
        parents[vertex] = Find(parents[vertex]);
    }
    return parents[vertex];
}

void DisjointSet::Union(int vertex_1, int vertex_2){ // uses union by rank
    int parent_1 = Find(vertex_1),
        parent_2 = Find(vertex_2);
    int rank_1 = Rank(parent_1),
        rank_2 = Rank(parent_2);

    if (rank_1 < rank_2){
        parents[parent_1] = parent_2; // adds the lower rank to the higher rank
    }
    else if (rank_2 < rank_1){
        parents[parent_2] = parent_1;
    }
    else{ // if they have same rank, the one thats being added to must increase in rank
        parents[parent_2] = parent_1;
        ranks[parent_1]++;
    }
}

int DisjointSet::Rank(int vertex){
    return ranks[vertex];
}
