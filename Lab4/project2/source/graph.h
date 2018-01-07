#include <map>
#include <list>

#include "../../project1/source/graph.h"

struct WGNode;

struct WENode : ENode
{
    WGNode* pointee;
    WENode* next;
    int weight;
};

struct WGNode : GNode
{
    WENode* fedge;
    WGNode* p;
};

class WeightedDirectedGraph : public DirectedGraph
{
public:
    list<WGNode> pointlist;
    WGNode* addNode(void* meta = nullptr);
    void addEdge(WGNode* pointer, WGNode* pointee, int weight); // reimplementation
    void clearEdge();
};

class ShortestPathTool
{
    // Of course, every object of this class must be associated with a DirectedGraph.
    // But it shouldn't change the original graph. As the changes we make in this
    // tool class are only used for measuring the shortest path.
    // But when it comes to adding nodes(the node s), it's hard to maintain the original
    // graph. Anyway, I won't take care of this in this lab.
public:
    ShortestPathTool(WeightedDirectedGraph& g);
    struct ShortestPathData
    {
        int distance;
        bool reachable; // when set false, the distance is thought to be infinity.
        WGNode* p;
    };
    typedef map<WGNode*,ShortestPathData> ShortestPathRecord;
    bool BellmanFord(WGNode* start); // make sure `s` is deleted before this is called
    void Dijkstra(WGNode* start);
    bool hascircus();
    void johnson();
private:
    WeightedDirectedGraph &g;
    ShortestPathRecord record_map;
    void cleanRecord();
    WGNode* addVirtualNode();
};