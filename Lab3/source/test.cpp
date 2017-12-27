#include "tree.cpp"
#include <array>
using namespace std;
int main()
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

}