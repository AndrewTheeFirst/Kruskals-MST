#include <string>
#include <vector>
#include <unordered_map>
#include "disjointset.h"

DisjointSet::DisjointSet(size_t size){
    parents.reserve(size);
    ranks.reserve(size);
}

void DisjointSet::Make_Set(size_t vertex){
    parents.push_back(vertex);
    ranks.push_back(vertex);
};

size_t DisjointSet::Find(size_t vertex){ // uses path compression
    if (parents[vertex] != vertex){
        parents[vertex] = Find(parents[vertex]);
    }
    return parents[vertex];
}

void DisjointSet::Union(size_t vertex_1, size_t vertex_2){ // uses union by rank
    size_t parent_1 = Find(vertex_1),
        parent_2 = Find(vertex_2);
    size_t rank_1 = Rank(parent_1),
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

size_t DisjointSet::Rank(size_t vertex){
    return ranks[vertex];
}
