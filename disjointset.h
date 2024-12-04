#include <string>
#include <vector>
#include <unordered_map>

class DisjointSet{

private:

    std::vector<int> parents;

    std::vector<int> ranks;

    int Rank(int vertex);

public:
    void Make_Set(int vertex);

    int Find(int vertex);

    void Union(int vertex_1, int vertex_2);
    

};