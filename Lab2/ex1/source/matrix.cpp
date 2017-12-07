#include <cstdlib>
#include <ctime>
#include <array>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <chrono>
template<typename ElementT>
class AbstractMatrix
{
protected:
    unsigned int height;
    unsigned int length;

public:
    AbstractMatrix(unsigned int h, unsigned int l):height(h), length(l) {};

    unsigned int getHeight()
    {   return height;  }
    unsigned int getLength()
    {   return length;}
};

template<typename ElementT>
class Matrix: public AbstractMatrix<ElementT>
{
public:
    Matrix(unsigned int h, unsigned int l):AbstractMatrix<ElementT>(h,l)
    {
        array_ptr = new ElementT*[h]; // array_ptr's type: ElementT**
        for(int i=0;i<h;i++)
            array_ptr[i] = new ElementT[l];
    }
    ElementT* operator[](std::size_t n)
    {   return array_ptr[n];    }
    const ElementT* operator[](std::size_t n) const
    {   return array_ptr[n];    }
    
private:
    ElementT** array_ptr;

};

std::vector<unsigned int> getRandomList(int n)
{
    std::vector<unsigned int> random_list;
    auto data_src = std::ifstream("../input/input.txt");
    if(data_src.is_open())
    {
        for(int i=0;i<n;i++)
        {
            int item;
            data_src>>item;
            random_list.push_back(item);
        }
    }
    else
    {
        std::srand(std::time(0));
        auto data_output = std::ofstream("../input/input.txt");
        for(int i=0;i<10;i++)
        {
            int item = std::rand();
            data_output<<item<<std::endl;
            random_list.push_back(item);
        }
        for(int i =0;i<500;i++)
        {
            int item = std::rand();
            data_output<<item<<std::endl;
        }
        data_output.close();
    }
    
    return random_list;
}

int run(int n,std::vector<unsigned int> &random_list,unsigned long long** &m, int** &s)
{
    // initialize n matrixs
    std::vector<AbstractMatrix<int>> matrix_array;
    for(int i=0;i<n;i++)
        matrix_array.push_back(AbstractMatrix<int>(random_list[i],random_list[i+1]));
    m = new unsigned long long*[n]; // stores the minimal multiplication times
    s = new int*[n]; // stores dividing positions
    for(int i=0;i<n;i++)
    {
        m[i] = new unsigned long long[n];
        s[i] = new int[n];
    }
    int l,i,j,k;
    for(int i=0;i<n;i++)
        m[i][i] = 0; 
    for(l=2;l<=n;l++) // l: the length of current group
    {
        for(i=0;i<=n-l;i++)
        {
            j = i+l-1;
            m[i][j] = -1; // -1 stands for infinity
            for(k=i;k<=j-1;k++)
            {
                unsigned long long currTimes = matrix_array[i].getHeight()*matrix_array[k].getLength()*matrix_array[j].getLength()
                                +m[i][k]+m[k+1][j];
                if(currTimes<m[i][j] || m[i][j]==-1)
                    {m[i][j] = currTimes;s[i][j]=k;}
            }
        }

    }
    return 0;

}

void printMatrix(std::vector<unsigned int> &random_list)
{
    std::cout<<random_list.size()-1<<" Matrix's sizes are shown below."<<std::endl;
    // size() returns n+1
    for(int i = 0; i < random_list.size()-1; i++)
        std::cout<<"Height: "<<random_list[i]<<" Length: "<<random_list[i+1]<<std::endl;
}

void printOptimalSequnce(int n, unsigned long long** &m, int** &s, int i, int j)
{
    if(i!=j)
    {
        int k = s[i][j];
        printOptimalSequnce(n,m,s,i,k);
        printOptimalSequnce(n,m,s,k+1,j);
        std::cout<<"Multiply matrix ";
        if(k==i)
            std::cout<<i;
        else
            std::cout<<"group "<<i<<" to "<<k;
        std::cout<<" and matrix ";
        if(k+1==j)
            std::cout<<j;
        else
            std::cout<<"group "<<k+1<<" to "<<j;
        std::cout<<std::endl;

        
    }
}

int main()
{
    auto time_output = std::ofstream("../output/performance_analysis.txt");
    std::vector<unsigned int> random_list;
    unsigned long long** m;
    int** s;
    time_output<<"{";
    std::array<int,43> N_list = {5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200};
    for(auto&& n:N_list)
    {
        time_output<<"{"<<n<<",";
        random_list = getRandomList(n+1);
        auto start = std::chrono::high_resolution_clock::now();
        run(n,random_list,m,s);
        auto stop = std::chrono::high_resolution_clock::now();
        time_output<<(stop-start).count()<<"}"<<",";
        printMatrix(random_list);
        printOptimalSequnce(n,m,s,0,n-1);

    }
    time_output<<"}";

}