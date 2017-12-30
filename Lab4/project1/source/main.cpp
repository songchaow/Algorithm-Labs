#include <ctime>
#include <array>
#include "graph.cpp"
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
    int edgescale;
    
    add_random_edge(g,edgescale);

}

int main()
{
    array<int,10> SCALE_LIST = {8,16,32,64,128,256};
    for(int i=0; i<10 ; i++)
    {
        run(SCALE_LIST[i]);
    }
}