#include <ctime>
#include <array>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <string>
#include "graph.h"
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

void add_random_edge(DirectedGraph &g, int scale, int nodescale)
{
    // see if there's a input file, if not, generate one.
    auto random_input = ifstream("../input/size"+to_string(nodescale)+"/input.txt");
    ofstream random_output;
    if(!random_input)
    {
        random_input.close();
        random_output = ofstream("../input/size"+to_string(nodescale)+"/input.txt");
    }
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
        if(!random_input)
            random_output << g.pointlist[node_index].id_no << "," << g.pointlist[pointee_index].id_no << endl;
        g.addEdge(&g.pointlist[node_index],&g.pointlist[pointee_index]);
        edge_added++;
    }
    if(!random_input)
    for(int i =0 ;i < 500; i++)
        random_output << rand()%nodenum << "," << rand()%nodenum << endl;
    random_output.close();
}
int run(int scale, ofstream &scc_output)
{
    ofstream time_output = ofstream("../output/size"+to_string(scale)+"/output1.txt");
    scc_output << "SCALE: " << scale << endl;
    DirectedGraph g;
    int edgescale = scale*log((double)scale);
    for(int i = 0;i<scale;i++)
        g.addNode();
    add_random_edge(g,edgescale,scale);
    serializer(g);
    if(scale < 100)
    system(("dot -Tpdf graph >> graph"+to_string(scale)+"_origin"+".pdf").c_str());
    // test
    auto trans_result = g.transpose();
    auto gt = trans_result.first;
    serializer(gt);
    if(scale < 100)
    system(("dot -Tpdf graph >> graph"+to_string(scale)+"_transposed"+".pdf").c_str());
    // test end
    g.find_scc(scc_output);
    return 0;
}

int main()
{
    array<int,6> SCALE_LIST = {8,16,32,64,128,256};
    for(auto&& scale : SCALE_LIST)
    {
        ofstream scc_output = ofstream("../output/size"+to_string(scale)+"/output1.txt");
        run(scale,scc_output);
    }
}