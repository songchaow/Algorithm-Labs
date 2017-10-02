#include <cstdlib>
#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <array>
#include <algorithm>
using namespace std;

#define TREE_PARENT(i) (i/2)
#define TREE_LEFT(i) (2*i)
#define TREE_RIGHT(i) (2*i+1)

class SortTools
{
    //the large elements are on the left side when sorted.
public:
    static bool compare(string &a, string &b)
    // a='bc' b='cc' outputs false
    {
        if(a.length()>b.length())
            return true;
        else if(a.length()<b.length())
            return false;
        else
            return ~a.compare(b);
    }
    static bool compare(int a, int b)
    {
        if(a>b) return true;
        else return false;
    }
    //whether to compare via lengths and characters, or directly via magnitude, need to be implemented later.
    template <typename MyIterator>
    static void insert_sort(MyIterator begin,MyIterator end)
    {
        for(;begin!=end;begin++)
        {
            MyIterator min=begin;
            MyIterator j;
            for(j=begin+1;j<end;j++)
                //find min element in  ... end
                if(compare(*min,*j)) min=j;
            if(min!=begin) swap(*min,*begin);
        }
    }
    template <typename MyIterator>
    static void heap_sort(MyIterator begin, MyIterator end)
    {
        buildHeap(begin,end);
        for(auto i=end-1;i>begin;i--)
        {
            swap(*begin,*i);
            maxHeapify(begin,i+1,1);
        }
    }
protected:
    template <typename MyIterator>
    static void maxHeapify(MyIterator begin, MyIterator end, int i)
    {
        // It's assumed that the heap is sorted except for the layer containing element No.i .
        // Initially i = 1
        MyIterator max = begin+i-1;
        if(compare(*(begin+TREE_LEFT(i)),*(begin+i-1)) && begin+TREE_LEFT(i)<end)
            max = begin+TREE_LEFT(i);
        if(compare(*(begin+TREE_RIGHT(i)),*max) && begin+TREE_RIGHT(i)<end)
            max = begin+TREE_RIGHT(i);
        if(max!= begin+i-1)
        {
            swap(*max,*(begin+i-1));
            auto diff = max-begin;
            // is this ok?
            maxHeapify(begin,end,(max-begin));
        }
    }
    template<typename MyIterator>
    static void buildHeap(MyIterator begin, MyIterator end)
    {
        auto n = end-begin;
        auto bottom_root = begin+(n/2)-1;
        for(auto j=bottom_root;j>=begin;j--)
            maxHeapify(begin,end,bottom_root-begin+1);
    }
};


int generator(int n,vector<string> &origin)
{
    // generate a random-length(from 0 to 32) string
    for(int i=1;i<=n;i++)
    {
        int str_len = rand()%32+1;
        string item;
        for(int j=1;j<=str_len;j++)
            item+=rand()%26+97;
        origin.push_back(item);
    }
    return 0;
}
int generator(int n,vector<int> &origin)
{
    ;
}


int main()
{
    vector<string> origin;
    srand(time(0));
    //--------------------------- test block 1
    generator(15,origin);
    vector<string> testvec=origin;
    for(auto i=testvec.begin();i<testvec.end();i++)
        cout << *i << endl;
    SortTools::heap_sort(testvec.begin(),testvec.end());
    cout << "sorted:" << endl;
    for(auto i=testvec.begin();i<testvec.end();i++)
        cout << *i << endl;
    //--------------------------- test block 1
    origin;
}