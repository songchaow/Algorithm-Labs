#include <ctime>
#include <array>
#include <fstream>
#include <cmath>
#include "graph.cpp"
using namespace std;
void serializer(DirectedGraph &g)
{
    // output text in dot language
    //15.03-15.42 sleep
    ofstream fdot("graph");
    fdot << "digraph G {" << endl;
    for(auto&& point: g.pointlist)
    {
        auto edge = point.fedge;
        while(edge != nullptr)
        {
            fdot << &point << " -> " << edge->pointee <<";" <<endl;
            edge = edge->next;
        }
    } 
}

void add_random_edge(DirectedGraph &g, int scale)
{
    srand(time(0));
    // generate `scale` edges at random.
    int nodenum = g.pointlist.size();
    int edge_added = 0;
    for(;edge_added<=scale;)
    {
        // randomly choose a GNode
        int node_index = rand()%nodenum;
        // randomly choose a pointee
        int pointee_index = rand()%nodenum;
        // check if this edge already exists
        auto edge = g.pointlist[node_index].fedge;
        while(edge != nullptr)
        {
            if(edge->pointee == &g.pointlist[pointee_index])
                continue; // try again
            edge = edge->next;
        }
        g.addEdge(&g.pointlist[node_index],&g.pointlist[node_index]);
        edge_added++;
    }
    
}
int run(int scale)
{
    DirectedGraph g;
    int edgescale = log((double)scale);
    for(int i = 0;i<scale;i++)
        g.addNode();
    add_random_edge(g,edgescale);
    serializer(g);
    return 0;
}

int main()
{
    array<int,10> SCALE_LIST = {8,16,32,64,128,256};
    for(int i=0; i<10 ; i++)
    {
        run(SCALE_LIST[i]);
    }
}