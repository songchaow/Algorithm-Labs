#include "graph.h"
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
        // first we generate negative edges
        int weight = rand()%nodenum;
        if(neg_edge_num>0) 
        {
            weight = -(rand()%neg_edge_num);
            neg_edge_num--;
        }
        g.addEdge(&g.pointlist[node_index],&g.pointlist[pointee_index],weight);
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

}