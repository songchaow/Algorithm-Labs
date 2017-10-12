#include <cstdlib>
#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <array>
#include <algorithm>
#include <map>
#include <chrono>
#include <fstream>
#include <utility>
#include <sstream>

using namespace std;

#define TREE_PARENT(i) (i/2)
#define TREE_LEFT(i) (2*i)
#define TREE_RIGHT(i) (2*i+1)
#define INT_RANGE 65535
#define DIGIT_NUM 5
string to_string(int);

static map<int,int> placeholder;
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
    static void merge_sort(MyIterator begin, MyIterator end, map<int,int> &info=placeholder)
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
    static void merge_sort_info(vector<int>::iterator begin, vector<int>::iterator end, map<int,int> &info)
    {
        if(end-begin>=2)
        {
            auto diff=end-begin;
            auto mid = begin+diff/2;
            merge_sort_info(begin,mid,info); //the element pointed by mid is in Part two.
            merge_sort_info(mid,end,info);           
            merge_info(begin,mid,end,info);
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
        quick_sort(mid+1,end);
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
            data_pool.at(*i-1)++; // data_pool now stores the number of each value occurs
        for(auto i=data_pool.begin()+1;i<data_pool.end();i++)
            *i=*i+*(i-1);
        // fill the sorted data back
        for(auto i=begin;i<end;i++)
        {
            output[data_pool[*i-1]-1] = *i;
            data_pool[*i-1]--;
        }
        auto i=begin,j=output.begin();
        for(;i<end;)
            *i++=*j++;
    }
    static void radix_sort(vector<int>::iterator begin, vector<int>::iterator end)
    {
        for(auto i=1;i<=DIGIT_NUM;i++)
        {
            //make a map between real values and the current digit(based on 10)
            map<int,int> info;
            info.clear();
            for(auto j=begin;j<end;j++)
                info.insert(pair<int,int>(*j,find_digit(*j,i)));
            //sort according to the digit
            merge_sort_info(begin,end,info);
        }

    }
protected:
    static int find_digit(int num, int index)
    {
        // eg: num=`6554` index=`1`, and we get `4`
        int leave;
        for(auto i=1;i<=index;i++)
        {
            leave = num%10;
            num = (num-leave)/10;
        }
        return leave;
    }
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
    static void merge_info(vector<int>::iterator begin, vector<int>::iterator mid, vector<int>::iterator end, map<int,int> &info=placeholder)
    {
        vector<int> A,B;
        for(auto i=begin;i<mid;i++)
            A.push_back(*i);
        for(auto i=mid;i<end;i++)
            B.push_back(*i);
        auto a=A.begin(),b=B.begin();
        while(a!=A.end() && b!=B.end())
        {
            if(compare(info[*a],info[*b]))
            {
                *begin++=*b;
                b++;
            }
            else
            {
                *begin++=*a;
                a++;
            }
        }
        if(a!=A.end())
        while(a!=A.end()) *begin++=(*a++);
        else while(b!=B.end()) *begin++=(*b++);
    }
    template <class MyIterator, class myvector>
    static void merge(MyIterator begin, MyIterator mid, MyIterator end, map<int,int> &info=placeholder)
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
            else
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
        if(begin+TREE_LEFT(i)-1<end)
        if(compare(*(begin+TREE_LEFT(i)-1),*(begin+i-1)) )
            max = begin+TREE_LEFT(i)-1;
        if(begin+TREE_RIGHT(i)-1<end)
        if(compare(*(begin+TREE_RIGHT(i)-1),*max) )
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
class TestCase
{
public:
    TestCase()
    {
        srand(time(0));
    }
    int SCALE_LIST[29] = {4,32,256,2048,3100,4200,5600,6700,7800,8900,10000,13000
        ,16384,17000,18000,19000,21000,22000,23000,24000,25000,26000,27000,28000,29000,30000,40000,80000,131072}; 

    template<typename myvector>
    void run_lab1(int scale, myvector &unsorted, myvector &sample)
    {
        sample = unsorted;
        // executes the four algorithms
        // measure execution time of insert-sort
        cout << "-----Quick Sort-----" << "scale: " << scale << ' ';
        auto start = std::chrono::high_resolution_clock::now();
        SortTools::quick_sort(sample.begin(),sample.end());
        auto end = std::chrono::high_resolution_clock::now();
        auto diff=(end-start);
        auto period = chrono::high_resolution_clock::period::den;
        cout << "Execution time: " << diff.count() << endl;;

        // measure execution time of heap-sort
        sample = unsorted;
        cout << "-----Heap Sort-----" << "scale: " << scale << ' ';
        start = std::chrono::high_resolution_clock::now();
        SortTools::heap_sort(sample.begin(),sample.end());
        end = std::chrono::high_resolution_clock::now();
        diff=(end-start);
        cout << "Execution time: " << diff.count()<<endl;
        
        // measure execution time of merge-sort
        sample = unsorted;
        cout << "-----Merge Sort-----" << "scale: " << scale << ' ';
        start = std::chrono::high_resolution_clock::now();
        SortTools::merge_sort<typename myvector::iterator, myvector>(sample.begin(),sample.end());
        end = std::chrono::high_resolution_clock::now();
        diff=(end-start);
        cout << "Execution time: " << diff.count()<<endl;

        // measure execution time of quick-sort
        sample = unsorted;
        cout << "-----Insert Sort-----" << "scale: " << scale << ' ';
        start = std::chrono::high_resolution_clock::now();
        SortTools::insert_sort(sample.begin(),sample.end());
        end = std::chrono::high_resolution_clock::now();
        diff=(end-start);
        cout << "Execution time: " << diff.count()<<endl;

    }

    template<typename myvector>
    void run_lab2(int scale, myvector &unsorted, myvector &sample)
    {
        sample = unsorted;
        // executes the four algorithms
        // measure execution time of insert-sort
        cout << "-----Bubble Sort-----" << "scale: " << scale << ' ';
        auto start = std::chrono::high_resolution_clock::now();
        SortTools::bubble_sort(unsorted.begin(),unsorted.end());
        auto end = std::chrono::high_resolution_clock::now();
        auto diff=(end-start);
        cout << "Execution time: " << diff.count()<<endl;

        // measure execution time of quick-sort
        sample = unsorted;
        cout << "-----Insert Sort-----" << "scale: " << scale << ' ';
        start = std::chrono::high_resolution_clock::now();
        SortTools::insert_sort(sample.begin(),sample.end());
        end = std::chrono::high_resolution_clock::now();
        diff=(end-start);
        cout << "Execution time: " << diff.count()<<endl;

        // measure execution time of radix-sort
        sample = unsorted;
        cout << "-----Radix Sort-----" << "scale: " << scale << ' ';
        start = std::chrono::high_resolution_clock::now();
        SortTools::radix_sort(sample.begin(),sample.end());
        end = std::chrono::high_resolution_clock::now();
        diff=(end-start);
        cout << "Execution time: " << diff.count()<<endl;

        // measure execution time of counting-sort
        sample = unsorted;
        cout << "-----Counting Sort-----" << "scale: " << scale << ' ';
        start = std::chrono::high_resolution_clock::now();
        SortTools::counting_sort(sample.begin(),sample.end());
        end = std::chrono::high_resolution_clock::now();
        diff=(end-start);
        cout << "Execution time: " << diff.count()<<endl;
    }

    void run_wrapper_int()
    {
        // deals with IO and executes `run`
        // for `int` type
        ifstream fstr("../input/data_number.txt");
        if(!fstr.is_open())
            {cout << "Can not import data from data_number.txt" << endl;return;}
        vector<int> unsorted;
        vector<int> sorted;
        int a;
        for(auto i=SCALE_LIST;i<SCALE_LIST+29;i++)
        {
            unsorted.clear();
            fstr.seekg(0);
            for(auto n=1;n<=*i;n++)
            {
                fstr >> a;
                unsorted.push_back(a);
            }
            run_lab2(*i,unsorted,sorted);

            stringstream filename;
            filename << "../output/data_number_sorted" << *i << ".txt";
            ofstream fsorted(filename.str());
            if(!fsorted.is_open())
                {cout << "Can not create file data_number" << endl;return;}
            for(auto i=sorted.begin();i<sorted.end();i++)
                fsorted << *i << endl;
            fsorted.close();
        }
    }
    void run_wrapper_string()
    {
        // deals with IO and executes `run`
        // for `string` type
        ifstream fstr("../input/data_string.txt");
        if(!fstr.is_open())
            {cout << "Can not import data from data_string.txt" << endl;return;}
        vector<string> unsorted;
        vector<string> sorted;
        string a;
        for(auto i=SCALE_LIST;i<SCALE_LIST+29;i++)
        {
            unsorted.clear();
            fstr.seekg(0);
            for(auto n=1;n<=*i;n++)
            {
                fstr >> a;
                unsorted.push_back(a);
            }
            run_lab1(*i,unsorted,sorted);
            stringstream filename;
            filename << "../output/data_string_sorted" << *i << ".txt";
            ofstream fsorted(filename.str());
            if(!fsorted.is_open())
                {cout << "Can not create file data_string" << endl;return;}
            for(auto i=sorted.begin();i<sorted.end();i++)
                fsorted << *i << endl;
            fsorted.close();
        }
        
    }
private:

};

int main()
{
    TestCase test;
    test.run_wrapper_string();
}

int OldTest()
{
    vector<string> origin;
    vector<int> origin_num;
    ofstream f("data_string.txt",std::ios::out);
    ofstream g("data_number.txt",std::ios::out);
    srand(time(0));
    //--------------------------- test block 1
    generator(200000,origin);
    generator(200000,origin_num);
    for(auto i=origin.begin();i<origin.end();i++)
    {
        f << *i << endl;
    }
    f.close();
    for(auto i=origin_num.begin();i<origin_num.end();i++)
    {
        g << *i << endl;
    }
    g.close();


    vector<string> testvec=origin;
    // for(auto i=testvec.begin();i<testvec.end();i++)
    //     cout << *i << endl;
    SortTools::radix_sort(origin_num.begin(),origin_num.end());
    SortTools::counting_sort(origin_num.begin(),origin_num.end());
    SortTools::quick_sort(testvec.begin(),testvec.end());
    testvec = origin;
    SortTools::merge_sort<vector<string>::iterator,vector<string>>(testvec.begin(),testvec.end());
    testvec = origin;
    SortTools::heap_sort(testvec.begin(),testvec.end());
    testvec = origin;
    SortTools::bubble_sort(testvec.begin(),testvec.end());
    cout << "sorted:" << endl;
    for(auto i=testvec.begin();i<testvec.end();i++)
        cout << *i << endl;
    //--------------------------- test block 1
    origin;
}
