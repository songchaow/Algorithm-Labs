#include <ctime>
#include <array>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <string>
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
        if(edge == nullptr)
            fdot << "\"" << point.id_no << "\"" << ";" << endl;
        while(edge != nullptr)
        {
            fdot << "\"" << point.id_no << "\"" << " -> " << "\"" << edge->pointee->id_no << "\"" <<";" <<endl;
            edge = edge->next;
        }
    } 
    fdot << "}" <<endl;
}

void add_random_edge(DirectedGraph &g, int scale)
{
    srand(time(0));
    // generate `scale` edges at random.
    int nodenum = g.pointlist.size();
    int edge_added = 0;
    for(;edge_added<scale;)
    {
        bool continue_flag = false;
        // randomly choose a GNode
        int node_index = rand()%nodenum;
        // randomly choose a pointee
        int pointee_index = rand()%nodenum;
        // check if this edge already exists
        auto edge = g.pointlist[node_index].fedge;
        while(edge != nullptr)
        {
            if(edge->pointee == &g.pointlist[pointee_index])
            {
                continue_flag = true;
                break;
            }
            edge = edge->next;
        }
        if(continue_flag) continue; // try again, without increasing `edge_added`
        g.addEdge(&g.pointlist[node_index],&g.pointlist[pointee_index]);
        edge_added++;
    }
    
}
int run(int scale)
{
    DirectedGraph g;
    int edgescale = scale*log((double)scale);
    for(int i = 0;i<scale;i++)
        g.addNode();
    add_random_edge(g,edgescale);
    serializer(g);
    system(("dot -Tpdf graph >> graph"+to_string(scale)+"_origin"+".pdf").c_str());
    // test
    auto trans_result = g.transpose();
    auto gt = trans_result.first;
    serializer(gt);
    system(("dot -Tpdf graph >> graph"+to_string(scale)+"_transposed"+".pdf").c_str());
    // test end
    g.find_scc();
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