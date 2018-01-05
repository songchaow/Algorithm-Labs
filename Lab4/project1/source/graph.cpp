#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <utility>

#include "graph.h"
using namespace std;

class DirectedGraph;


    DFSTool::DFSTool(DirectedGraph& g) : g(g)
    {
        // make data record for each Gnode
        for(auto&& point : g.pointlist)
        {
            DFSNodeRecord record;
            record.color = Color::WHITE;
            DFSResult.insert(pair<GNode*,DFSNodeRecord>(&point,record));
        }
    }
    
    void DFSTool::clear()
    {
        // clear the runtime record stored in DFSResult
        for(auto&& point:DFSResult)
            point.second.color = Color::WHITE;
        time = 0;
    }
    DFSTool::DFSState DFSTool::getDFSResult()
    {
        return DFSResult;
    }
    void DFSTool::DFSVisit(GNode* node)
    {
        // there may be GREY nodes in previous unfinished visits.
        if(DFSResult[(void*)node].color == Color::WHITE)
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
    void DFSTool::DFS()
    {
        for(auto&& gnode:g.pointlist)
        {
            // After this iteration, all the nodes visited will be black,
            // with some other nodes being white. There will be no grey nodes.
            if(DFSResult[&gnode].color == Color::WHITE)
            {
                DFSResult[&gnode].p = &gnode; // the root
                DFSVisit(&gnode);
            }
        }
    }
    void DFSTool::reOrderedDFS(vector<pair<void*,DFSTool::DFSNodeRecord>>& node_seq) 
    {
        // used to find the SSC
        for(auto&& node_rec:node_seq)
        {
            auto gnode = (GNode*)node_rec.first;
            if(DFSResult[gnode].color == Color::WHITE)
            {
                DFSResult[&gnode].p = &gnode; // the root
                DFSVisit(gnode);
            }
        }
    }


    GNode* DirectedGraph::addNode(void* meta)
    {
        GNode node;
        node.meta = meta;
        node.fedge = nullptr; // it has no edges initially
        node.p = nullptr; // it doesn't belong to any sets initially
        pointlist.push_back(node);
        return &pointlist[pointlist.size()-1];
    }
    void DirectedGraph::addEdge(GNode* pointer, GNode* pointee)
    {
        auto secondEdge = pointer->fedge;
        auto fEdge = new ENode;
        fEdge->next = secondEdge;
        fEdge->pointee = pointee;
        pointer->fedge = fEdge;
    }
    void DirectedGraph::makeSet(GNode* node, DFSTool::DFSState&& dfs_result) //abandoned
    {
        dfs_result[node].p = node;
        node->p = node;
        node->rank = 0;
    }
    GNode* DirectedGraph::findSet(GNode* node, DFSTool::DFSState& dfs_result)
    {
        if(node != dfs_result[node].p)
            dfs_result[node].p = findSet((GNode*)dfs_result[node].p, dfs_result);
        return (GNode*)dfs_result[node].p;
    }
    void DirectedGraph::link(GNode* node1, GNode* node2) // abandoned
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
    // void union_(GNode* node1, GNode* node2) // abandoned
    // {
    //     link(findSet(node1),findSet(node2));
    // }
    pair<DirectedGraph,map<GNode*,GNode*>> DirectedGraph::transpose()
    {
        // a temporary map is required to convert old ptrs to new ones
        map<GNode*,GNode*> new_ptr;
        DirectedGraph gt;
        // copy all nodes to the new graph
        for(auto&& point:pointlist)
        {
            auto newnode = gt.addNode(point.meta);
        }
        for(int i = 0;i<pointlist.size();i++)
        {

            new_ptr.insert(pair<GNode*,GNode*>(&pointlist[i],&gt.pointlist[i]));
        }
        for(auto&& point:pointlist)
        {
            // `edge` is from old graph, we will not meet an edge more than once!
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
        return pair<DirectedGraph,map<GNode*,GNode*>>(gt,new_ptr);
    }
    void DirectedGraph::find_scc()
    {
        ofstream scc_output = ofstream("../output/output1.txt");
        DFSTool tool1 = DFSTool(*this);
        tool1.DFS();
        auto node_seq_tree = tool1.getDFSResult();
        vector<pair<void*,DFSTool::DFSNodeRecord>> node_seq(node_seq_tree.begin(),node_seq_tree.end());
        auto trans_result = transpose();
        DirectedGraph gt = trans_result.first;
        DFSTool tool2 = DFSTool(gt);
        
        auto cmp = [](pair<void*,DFSTool::DFSNodeRecord> const & a,pair<void*,DFSTool::DFSNodeRecord> const & b)
        {
            return (a.second.time_f != b.second.time_f)?  a.second.time_f > b.second.time_f : false;
        };

        sort(node_seq.begin(),node_seq.end(),cmp); 
        tool2.reOrderedDFS(node_seq);
        // now, generate scc
        // each pair's first stores the representative.
        map<GNode*,vector<GNode*>> scc_set;
        auto dfs_result = tool2.getDFSResult();
        for(auto&& record: dfs_result)
        {
            GNode* node = (GNode*)record.first;
            auto representative = findSet(node,dfs_result);
            if(scc_set.find(representative) != scc_set.end())
            {
                scc_set[representative].push_back(node);
            }
            else
                scc_set.insert(pair<GNode*,vector<GNode*>>(representative,vector<GNode*>()));
        }
        int count = 1;
        for(auto&& p : scc_set)
        {
            scc_output << "SCC SET " << count <<": "<< p.first;
            for(auto&& el : p.second)
                scc_output << '\t' << el << endl;
        }
    }
