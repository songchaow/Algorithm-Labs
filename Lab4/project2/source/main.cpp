#include <ctime>
#include <array>
#include <cmath>

#include "graph.h"

void serializer(WeightedDirectedGraph &g)
{
    // output text in dot language
    //15.03-15.42 sleep
    ofstream fdot("graph-project2");
    fdot << "digraph G {" << endl;
    for(auto&& point: g.pointlist)
    {
        auto edge = point.fedge;
        if(edge == nullptr)
            fdot << "\"" << point.id_no << "\"" << ";" << endl;
        while(edge != nullptr)
        {
            fdot << "\"" << point.id_no << "\"" << " -> " << "\"" << edge->pointee->id_no << "\"" ;
            fdot << "[ " << "label= \"" << edge->weight <<"\"];" << endl;
            edge = edge->next;
        }
    } 
    fdot << "}" <<endl;
}

void add_random_edge(WeightedDirectedGraph &g, int scale, int nodescale)
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
    int neg_edge_num = scale/nodenum;
    int neg_edge_num0 = neg_edge_num;
    int edge_added = 0;
    for(;edge_added<scale;)
    {
        bool continue_flag = false;
        // randomly choose a GNode
        int node_index = rand()%nodenum;
        // randomly choose a pointee
        int pointee_index = rand()%nodenum;
        // locate these two elements:
        auto it = g.pointlist.begin();
        for(int i = 0; i<node_index ; i++)
            it++;
        auto pointer = it; it = g.pointlist.begin();
        for(int i = 0; i<pointee_index ; i++)
            it++;
        auto pointee = it;

        // check if this edge already exists
        auto edge = pointer->fedge;
        while(edge != nullptr)
        {
            if(edge->pointee == &*pointee)
            {
                continue_flag = true;
                break;
            }
            edge = edge->next;
        }
        if(continue_flag) continue; // try again, without increasing `edge_added`
        if(!random_input)
            random_output << pointer->id_no << "," << pointee->id_no << endl;
        // first we generate negative edges
        int weight = rand()%nodenum;
        if(neg_edge_num>0) 
        {
            weight = -(rand()%neg_edge_num0);
            neg_edge_num--;
        }
        g.addEdge(&*pointer,&*pointee,weight);
        edge_added++;
    }
    if(!random_input)
    for(int i =0 ;i < 500; i++)
        random_output << rand()%nodenum << "," << rand()%nodenum << endl;
    random_output.close();
}

int run(int scale)
{
    WeightedDirectedGraph g;
    int edgescale = scale*log((double)scale);
    for(int i = 0; i < scale; i++)
        g.addNode();
    add_random_edge(g,edgescale,scale);
    // test if the edges are fine
    ShortestPathTool tool = ShortestPathTool(g);
    while(tool.hascircus())
    {
        g.clearEdge();
        add_random_edge(g,edgescale,scale);
    }
    serializer(g);
    system(("dot -Tpdf graph-project2 >> graph"+to_string(scale)+"_origin"+".pdf").c_str());
    tool.johnson();
}

int main()
{
    array<int,6> SCALE_LIST = {8,16,32,64,128,256};
    for(auto&& scale : SCALE_LIST)
    {
        run(scale);
    }

}