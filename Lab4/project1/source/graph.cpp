#include <vector>
#include <list>
#include <map>
#include <algorithm>
using namespace std;

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
    map<void*,DFSNodeRecord> DFSResult;
    int time = 0;
    DirectedGraph& g;
public:
    DFSTool(DirectedGraph& g) : g(g)
    {
        // make data record for each Gnode
        for(auto&& point:g.pointlist)
        {
            DFSNodeRecord record;
            record.color = Color::WHITE;
            DFSResult.insert(make_pair<void*,DFSNodeRecord>(&point,record));
        }
    }
    void clear()
    {
        // clear the runtime record stored in DFSResult
        for(auto&& point:DFSResult)
            point.second.color = Color::WHITE;
        time = 0;
    }
    map<void*,DFSNodeRecord> getDFSResult()
    {
        return DFSResult;
    }
    void DFSVisit(DirectedGraph::GNode* node)
    {
        // there may be GREY nodes in previous unfinished visits.
        if(DFSResult[node].color == Color::WHITE)
        {
            time++;
            DFSResult[node].color = Color::GREY;
            DFSResult[node].time_d = time;
            for(auto edge = node->fedge;edge!=nullptr;edge = edge->next)
            {
                DFSResult[edge->pointee].p = node;
                DFSVisit(edge->pointee);
            }
            time++;
            DFSResult[node].time_f = time;
            DFSResult[node].color = Color::BLACK;
        }
    }
    void DFS()
    {
        for(auto&& gnode:g.pointlist)
        {
            // After this iteration, all the nodes visited will be black,
            // with some other nodes being white. There will be no grey nodes.
            if(DFSResult[&gnode].color == Color::WHITE)
            {
                DFSResult[&gnode].p = nullptr; // the root
                DFSVisit(&gnode);
            }
        }
    }
    void reOrderedDFS(map<void*,DFSNodeRecord>& node_seq)
    {

    }

};

class DirectedGraph
{
private:
    struct EdgeNode;
public:
    typedef struct GraphNode
    {
        void* meta;
        EdgeNode* fedge;
        GraphNode* p; // used for root-tree
        int rank; // used for root-tree
    } GNode;

private:
    
    typedef struct EdgeNode
    {
        GNode* pointee;
        EdgeNode* next;
    } ENode;
    
    // rewrite DFS:
        // sort the pairs stored in DFSResult according to time_f.
        

    
public:
    
    vector<GNode> pointlist;
    GNode* addNode(void* meta = nullptr)
    {
        GNode node;
        node.meta = meta;
        node.fedge = nullptr; // it has no edges initially
        node.p = nullptr; // it doesn't belong to any sets initially
        pointlist.push_back(node);
    }
    void addEdge(GNode* pointer, GNode* pointee)
    {
        auto secondEdge = pointer->fedge;
        auto fEdge = new ENode;
        fEdge->next = secondEdge;
        fEdge->pointee = pointee;
        pointer->fedge = fEdge;
    }
    void makeSet(GNode* node)
    {
        node->p = node;
        node->rank = 0;
    }
    GNode* findSet(GNode* node)
    {
        if(node != node->p)
            node->p = findSet(node->p);
        return node->p;
    }
    void link(GNode* node1, GNode* node2)
    {
        if(node1->rank>node2->rank)
            node2->p = node1;
        else
        {
            node1->p = node2;
            if(node1->rank==node2->rank)
                node2->rank++;
        }
        
    }
    void union_(GNode* node1, GNode* node2)
    {
        link(findSet(node1),findSet(node2));
    }
    DirectedGraph transpose()
    {
        // a temporary map is required to convert old ptrs to new ones
        map<GNode*,GNode*> new_ptr;
        DirectedGraph gt;
        // copy all nodes to the new graph
        for(auto&& point:pointlist)
        {
            auto newnode = gt.addNode(point.meta);
            new_ptr.insert(pair<GNode*,GNode*>(&point,newnode));
        }
        for(auto&& point:pointlist)
        {
            // `edge` is from old graph
            auto edge = point.fedge;
            while(edge)
            {
                auto new_pointee = new_ptr[edge->pointee];
                auto edge0 = new_pointee->fedge;
                // add `edge` to pointee's set
                ENode* new_edge = new ENode;
                new_edge->pointee = new_ptr[&point];
                new_edge->next = edge0;
                new_pointee->fedge = new_edge;
                // deal with the next edge
                edge = edge->next;
            }
        }
        return gt;
    }
    void find_gcc()
    {
        DFSTool tool1 = DFSTool(*this);
        tool1.DFS();
        map<void*,DFSTool::DFSNodeRecord> node_seq = tool1.getDFSResult();
        DirectedGraph gt = transpose();
        DFSTool tool2 = DFSTool(gt);
        auto cmp = [](std::pair<void*,DFSTool::DFSNodeRecord> const & a, std::pair<void*,DFSTool::DFSNodeRecord> const & b) 
        { 
             return a.second.time_f != b.second.time_f?  a.second.time_f > b.second.time_f : a.first < b.first;
        };
        sort(node_seq.begin(),node_seq.end(),cmp);
        for(auto&& node_rec:node_seq)
        {
            auto gnode = (DirectedGraph::GNode*)node_rec.first;
            if(DFSResult[gnode].color == Color::WHITE)

        }
    }

};