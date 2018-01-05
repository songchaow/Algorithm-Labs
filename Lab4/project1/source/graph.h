#include <vector>
#include <map>
#include <fstream>
using namespace std;

struct EdgeNode;

typedef struct GraphNode
{
    void* meta;
    EdgeNode* fedge;
    GraphNode* p; // used for root-tree
    int rank; // used for root-tree
} GNode;

typedef struct EdgeNode
{
    GNode* pointee;
    EdgeNode* next;
} ENode;

class DirectedGraph;

class DFSTool
{
private:
    enum Color
    {
        WHITE,
        GREY,
        BLACK
    };
public:
    typedef struct DFSData
    {
        int time_d;
        int time_f;
        Color color;
        void* p; // parent
    } DFSNodeRecord;
    
private:
    // struct cmpByTime
    // // we customize the compare functor, so the nodes are inserted according to `time_f` in descending order.
    // {
    //     bool operator()(pair<void*,DFSTool::DFSNodeRecord> const & a,pair<void*,DFSTool::DFSNodeRecord> const & b) const
    //     {
    //         return (a.second.time_f != b.second.time_f)?  a.second.time_f > b.second.time_f : false;
    //     }
    // };
    
    map<void*,DFSNodeRecord> DFSResult;
    int time = 0;
    DirectedGraph& g;
public:
    typedef map<void*,DFSNodeRecord> DFSState;
    DFSTool(DirectedGraph& g);
    
    void clear();
    DFSState getDFSResult();
    void DFSVisit(GNode* node);
    void DFS();
    void reOrderedDFS(vector<GNode*>& node_list);

};

class DirectedGraph
{
public:
    //DirectedGraph(const DirectedGraph&) = default;
    vector<GNode> pointlist;
    GNode* addNode(void* meta = nullptr);
    void addEdge(GNode* pointer, GNode* pointee);
    void makeSet(GNode* node, DFSTool::DFSState&& dfs_result); //abandoned

    GNode* findSet(GNode* node, DFSTool::DFSState& dfs_result);
    void link(GNode* node1, GNode* node2); // abandoned
    // void union_(GNode* node1, GNode* node2) // abandoned
    // {
    //     link(findSet(node1),findSet(node2));
    // }
    pair<DirectedGraph,map<GNode*,GNode*>> transpose();

    void find_scc();

};