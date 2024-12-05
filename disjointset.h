#include <string>
#include <vector>
#include <unordered_map>

class DisjointSet{

private:
    std::vector<size_t> parents;

    std::vector<size_t> ranks;

    size_t Rank(size_t vertex);

public:
    DisjointSet(size_t size);

    void Make_Set(size_t vertex);

    size_t Find(size_t vertex);

    void Union(size_t vertex_1, size_t vertex_2);
};