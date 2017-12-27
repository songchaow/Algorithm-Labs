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