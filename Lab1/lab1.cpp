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
#define INT_RANGE 65535

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
            return a.compare(b)>0;
    }
    static bool compare(int a, int b)
    {
        if(a>b) return true;
        else return false;
    }
    //whether to compare via both lengths and characters, or directly via magnitude, need to be implemented later.
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
            maxHeapify(begin,i,1);
        }
    }
    template <class MyIterator, class myvector>
    static void merge_sort(MyIterator begin, MyIterator end)
    // top-down approach
    {
        if(end-begin>=2)
        {
            auto diff=end-begin;
            auto mid = begin+diff/2;
            merge_sort<MyIterator,myvector>(begin,mid); //the element pointed by mid is in Part two.
            merge_sort<MyIterator,myvector>(mid,end);
            merge<MyIterator,myvector>(begin,mid,end);
        }
    }
    template <typename MyIterator>
    static void bubble_sort(MyIterator begin, MyIterator end)
    {
	for(auto i=begin;i<end-1;i++)
 	    {
            for(auto j=begin;j<end-1-(i-begin);j++)
            {
                if(compare(*j,*(j+1))) swap(*j,*(j+1));
            }
	    }
	    
    }
    template <typename MyIterator>
    static void quick_sort(MyIterator begin, MyIterator end)
    {
        if(end-begin<2) return;
        auto mid = partition(begin,end);
        quick_sort(begin,mid);
        quick_sort(mid,end);
    }
    static void counting_sort(vector<int>::iterator begin, vector<int>::iterator end)
    {
        // This member functin applies only to `integer` elements.
        array<int,INT_RANGE> data_pool;
        vector<int> output;
        output.resize(end-begin+1);
        for(auto i=data_pool.begin();i<data_pool.end();i++)
            *i = 0;
        for(auto i=begin;i<end;i++)
            data_pool.at(*i)++; // data_pool now stores the number of each value occurs
        for(auto i=data_pool.begin()+1;i<data_pool.end();i++)
            *i=*i+*(i-1);
        // fill the sorted data back
        for(auto i=begin;i<end;i++)
        {
            output[data_pool[*i]-1] = *i;
            data_pool[*i]--;
        }
        auto i=begin,j=output.begin();
        for(;i<end;)
            *i++=*j++;
    }
protected:
    template <typename MyIterator>
    static MyIterator partition(MyIterator begin, MyIterator end)
    {
        auto pivot = *(end-1);
        auto i=begin-1,j=begin;
        for(j=begin;j<end-1;j++)
        {
            if(compare(pivot,*j)) // j<pivot
            {
                swap(*++i,*j);
            }
        }
        swap(*(i+1),*(end-1));
        return (i+1);
    }
    template <class MyIterator, class myvector>
    static void merge(MyIterator begin, MyIterator mid, MyIterator end)
    {
        // Array A and B have been sorted so that large elements are on right side.
        myvector A,B;
        for(auto i=begin;i<mid;i++)
            A.push_back(*i);
        for(auto i=mid;i<end;i++)
            B.push_back(*i);
        auto a=A.begin(),b=B.begin();
        while(a!=A.end() && b!=B.end())
        {
            if(compare(*a,*b))
            {
                *begin++=*b;
                b++;
            }
            else if(compare(*b,*a))
            {
                *begin++=*a;
                a++;
            }
        }
        if(a!=A.end())
            while(a!=A.end()) *begin++=(*a++);
        else while(b!=B.end()) *begin++=(*b++);
        

    }
    template <typename MyIterator>
    static void maxHeapify(MyIterator begin, MyIterator end, int i)
    {
        // It's assumed that the heap is sorted except for the layer containing element No.i .
        // Initially i = 1, indicating the index of largest element
        MyIterator max = begin+i-1;
        if(compare(*(begin+TREE_LEFT(i)-1),*(begin+i-1)) && begin+TREE_LEFT(i)-1<end)
            max = begin+TREE_LEFT(i)-1;
        if(compare(*(begin+TREE_RIGHT(i)-1),*max) && begin+TREE_RIGHT(i)-1<end)
            max = begin+TREE_RIGHT(i)-1;
        if(max!= begin+i-1)
        {
            swap(*max,*(begin+i-1));
            auto diff = max-begin;
            // is this ok?
            maxHeapify(begin,end,(max-begin+1));
        }
    }
    template<typename MyIterator>
    static void buildHeap(MyIterator begin, MyIterator end)
    {
        auto n = end-begin;
        auto bottom_root = begin+(n/2)-1;
        for(auto j=bottom_root;j>=begin;j--)
            maxHeapify(begin,end,j-begin+1);
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
    for(int i=1;i<=n;i++)
    {
        int item = rand()%65535+1;
        origin.push_back(item);
    }
}

//template  void SortTools::merge_sort<vector<string>::iterator,vector<string>>(vector<string>::iterator,vector<string>::iterator);
//template  void SortTools::merge<vector<string>::iterator, vector<string>>(vector<string>::iterator,vector<string>::iterator,vector<string>::iterator);

int main()
{
    vector<string> origin;
    vector<int> origin_num;
    srand(time(0));
    //--------------------------- test block 1
    generator(15,origin);
    generator(15,origin_num);
    vector<string> testvec=origin;
    for(auto i=testvec.begin();i<testvec.end();i++)
        cout << *i << endl;
    SortTools::counting_sort(origin_num.begin(),origin_num.end());
    SortTools::quick_sort(testvec.begin(),testvec.end());
    SortTools::bubble_sort(testvec.begin(),testvec.end());
    SortTools::merge_sort<vector<string>::iterator,vector<string>>(testvec.begin(),testvec.end());
    SortTools::heap_sort(testvec.begin(),testvec.end());
    cout << "sorted:" << endl;
    for(auto i=testvec.begin();i<testvec.end();i++)
        cout << *i << endl;
    //--------------------------- test block 1
    origin;
}
