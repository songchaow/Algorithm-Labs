#include <set>
#include <fstream>
#include <string>

#include "graph.h"

using namespace std;

WGNode* WeightedDirectedGraph::addNode(void* meta)
{
    WGNode node;
    node.meta = meta;
    node.fedge = nullptr; // it has no edges initially
    node.p = nullptr; // it doesn't belong to any sets initially
    node.id_no = curr_index++;
    pointlist.push_back(node);
    return &pointlist[pointlist.size()-1];
}

void WeightedDirectedGraph::addEdge(WGNode* pointer, WGNode* pointee, int weight)
{
    auto secondEdge = pointer->fedge;
    auto fEdge = new WENode;
    fEdge->weight = weight;
    fEdge->next = secondEdge;
    fEdge->pointee = pointee;
    pointer->fedge = fEdge;
}

WGNode* ShortestPathTool::addVirtualNode()
{
    auto s = g.addNode();
    ShortestPathData entry;
    entry.distance = 0;
    entry.p = nullptr;
    record_map[s] = entry;
    for(auto it = g.pointlist.begin();it != g.pointlist.end()-1; it++)
    // we skip the last element, i.e., s.
        g.addEdge(s,&*it,0);
    return s;
}

ShortestPathTool::ShortestPathTool(WeightedDirectedGraph& g) : g(g)
{
    for(auto&& point : g.pointlist)
    {
        ShortestPathData entry;
        entry.distance = 0;
        entry.p = nullptr;
        entry.reachable = false;
        record_map[&point] = entry;
    }
}

bool ShortestPathTool::BellmanFord(WGNode* start)
// make sure `s` is deleted before called
{
    bool err_flag = false;
    record_map[start].reachable = true;
    // only s is reachable at the beginning
    for(auto&& i = 1;i<=g.pointlist.size();i++)
    {
        // for each edge
        for(auto&& point : g.pointlist)
        {
            if(!record_map[&point].reachable)
                continue; // this point is unreachable
            auto edge = point.fedge;
            while(edge != nullptr)
            {
                if(!record_map[edge->pointee].reachable)
                {
                    record_map[edge->pointee].distance = record_map[&point].distance + edge->weight;
                    record_map[edge->pointee].reachable = true;
                }
                else if(record_map[&point].distance + edge->weight < record_map[edge->pointee].distance)
                    record_map[edge->pointee].distance = record_map[&point].distance + edge->weight;
            }
        }
    }
    // loop once more
    for(auto&& point : g.pointlist)
    {
        if(!record_map[&point].reachable)
            continue; // this point is unreachable
        auto edge = point.fedge;
        while(edge != nullptr)
        {
            if(!record_map[edge->pointee].reachable)
                err_flag = true;
            else if(record_map[&point].distance + edge->weight < record_map[edge->pointee].distance)
                {err_flag = true; break;}
        }
        if(err_flag) break;
    }
    return err_flag;
}

void ShortestPathTool::cleanRecord()
{
    for(auto&& record : record_map)
    {
        record.second.distance = 0;
        record.second.p = nullptr;
        record.second.reachable = false;
    }
}

bool ShortestPathTool::hascircus()
{
    bool result = BellmanFord(&g.pointlist[0]);
    cleanRecord();
    return result;
}

void ShortestPathTool::Dijkstra(WGNode* start)
{
    // record must be cleaned before called!
    record_map[start].reachable = true;
    set<WGNode*> node_list = set<WGNode*>(g.pointlist.begin(),g.pointlist.end());
    // first, find out the min distance
    while(!node_list.empty())
    {
        WGNode* min_node = *(node_list.begin());
        for(auto&& node : node_list)
        {
            if(!record_map[min_node].reachable && record_map[node].reachable)
                min_node = node;
            if(record_map[node].reachable && record_map[node].distance < record_map[min_node].distance)
                min_node = node;
        }
        auto edge = min_node->fedge;
        while(edge)
        {
            if(!record_map[edge->pointee].reachable)
            {
                record_map[edge->pointee].distance = record_map[min_node].distance + edge->weight;
                record_map[edge->pointee].reachable = true;
                record_map[edge->pointee].p = min_node;
            }
            else if(record_map[min_node].distance + edge->weight < record_map[edge->pointee].distance)
            {
                record_map[edge->pointee].distance = record_map[min_node].distance + edge->weight;
                record_map[edge->pointee].p = min_node;
            }
            edge = edge->next;
        }
        node_list.erase(min_node);
    }
}

void ShortestPathTool::johnson()
{
    ofstream path_output = ofstream("../output/size"+to_string(g.pointlist.size())+"/output2.txt");
    // record must be cleaned before called!
    auto s = addVirtualNode();
    BellmanFord(s);
    g.pointlist.pop_back(); // delete s;
    // change the weight of each edge
    for(auto&& node : g.pointlist)
    {
        auto edge = node.fedge;
        while(edge)
        {
            edge->weight = edge->weight + record_map[&node].distance - record_map[edge->pointee].distance;
            edge = edge->next;
        }
    }
    cleanRecord();
    // execute Dijkstra
    for(auto&& node: g.pointlist)
    {
        Dijkstra(&node);
        path_output << "Shortest path from node " << node.id_no << endl;
        // output sequences
        for(auto&& term : g.pointlist)
        {
            if(&term == &node)
                continue;
            auto point = &term;
            path_output << '\t' << "node " << point->id_no << " : ";
            while(point)
            {
                path_output << point->id_no << "<- ";
                point = record_map[point].p;
            }
            path_output << endl;
        }
        cleanRecord();
    }
    
    
}