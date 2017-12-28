#include "tree.cpp"
#include <array>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
using namespace std;
int simpleTest()
{
    BinSearchTree<int> bintree;
    RedBlackTree<int> rbtree;
    BinSearchTree<int>::Node* p;
    array<int,10> elements = {5,29,38,54,17,19,743,654,21,8};
    // test insert
    for(auto&& el:elements)
    {
        bintree.insert(el);
        rbtree.insert(el);
    }
    auto el = bintree.search(17);
    bintree.delete_(el);
    el = bintree.search(29);
    bintree.delete_(el);

    el = rbtree.search(17);
    rbtree.delete_(el);
    el = rbtree.search(21);
    rbtree.delete_(el);
    el = rbtree.search(29);
    rbtree.delete_(el);
}

vector<int> run(vector<int> &data_list, RedBlackTree<int> &rbtree)
{
    vector<int> deltas;
    int count = 1;
    auto start = chrono::high_resolution_clock::now();
    for(auto&& item:data_list)
    {
        rbtree.insert(item);
        if(count%10==0)
        {
            auto stop = chrono::high_resolution_clock::now();
            deltas.push_back((stop-start).count());
        }
    }
    return deltas;
}
vector<int> run2(vector<int> &data_list, BinSearchTree<int> &rbtree)
{
    vector<int> deltas;
    int count = 1;
    auto start = chrono::high_resolution_clock::now();
    for(auto&& item:data_list)
    {
        rbtree.insert(item);
        if(count%10==0)
        {
            auto stop = chrono::high_resolution_clock::now();
            deltas.push_back((stop-start).count());
        }
        count++;
    }
    // delete elements with index n/3, n/4
    start = chrono::high_resolution_clock::now();
    auto el = rbtree.select(rbtree.getSize()/3);
    deltas.push_back(el->data);
    rbtree.delete_(el);
    auto stop = chrono::high_resolution_clock::now();
    deltas.push_back((stop-start).count());

    start = chrono::high_resolution_clock::now();
    el = rbtree.select(rbtree.getSize()/4);
    deltas.push_back(el->data);
    rbtree.delete_(el);
    stop = chrono::high_resolution_clock::now();
    deltas.push_back((stop-start).count());
    return deltas;
}


void printPreOrder(typename BinSearchTree<int>::Node *x, ofstream &f)
{
    f << x->data;
    f << ' ';
    if(x->lchild)
    printPreOrder(x->lchild,f);
    if(x->rchild)
    printPreOrder(x->rchild,f);
}
void printMidOrder(typename BinSearchTree<int>::Node *x, ofstream &f)
{
    if(x->lchild)
    printMidOrder(x->lchild,f);
    f << x->data;
    f << ' ';
    if(x->rchild)
    printMidOrder(x->rchild,f);
}
void printPostOrder(typename BinSearchTree<int>::Node *x, ofstream &f)
{
    if(x->lchild)
    printPostOrder(x->lchild,f);
    if(x->rchild)
    printPostOrder(x->rchild,f);
    f << x->data;
    f << ' ';
}


int main()
{
    auto data_src = ifstream("../input/input.txt");
    auto time_output = ofstream("../output/time1.txt");
    auto data_output = ofstream();
    vector<int> data_list;
    array<int,20> SCALE_LIST = {12,24,36,48,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210};
    for(auto&& scale:SCALE_LIST)
    {
        data_list.clear();
        for(int i =0;i<scale;i++)
        {
            int num;
            data_src>>num;
            data_list.push_back(num);
        }
        //RedBlackTree<int> rbtree;
        BinSearchTree<int> rbtree;
        auto deltas = run2(data_list,rbtree);
        string prefix = "../output/size";
        if(scale<=60)
        {
            data_output.open(prefix+to_string(scale)+"/preorder.txt");
            printPreOrder(rbtree.getHead(),data_output);
            data_output.close();
            data_output.open(prefix+to_string(scale)+"/inorder.txt");
            printMidOrder(rbtree.getHead(),data_output);
            data_output.close();
            data_output.open(prefix+to_string(scale)+"/inorder.txt");
            printPostOrder(rbtree.getHead(),data_output);
            data_output.close();
            data_output.open(prefix+to_string(scale)+"/deldata.txt");
            data_output << deltas.at(deltas.size()-1)<<endl;
            data_output << deltas.at(deltas.size()-2)<<endl;
            data_output.open(prefix+to_string(scale)+"/time2.txt");
            data_output << "用时分别为：" << endl;
            data_output << deltas.at(deltas.size()-1)<<endl;
            data_output << deltas.at(deltas.size()-2)<<endl;
            deltas.pop_back();
            deltas.pop_back();
            data_output.close();
            data_output.open(prefix+to_string(scale)+"/time1.txt");
            int i = 1;
            for(auto&& item:deltas)
            {
                data_output << (i-1)*10+1 << '-' << i*10 << "元素插入耗时:";
                data_output << item << endl;
                i++;
            }
            data_output.close();
        }
    }
}