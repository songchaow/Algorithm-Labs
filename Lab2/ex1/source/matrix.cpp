#include <cstdlib>
#include <ctime>
#include <array>
#include <vector>
#include <utility>
#include <iostream>
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
    std::srand(std::time(0));
    for(int i=0;i<n;i++)
        random_list.push_back(std::rand());
    return random_list;
}

int run(int n,std::vector<unsigned int> &random_list,int** &m, int** &s)
{
    // initialize n matrixs
    std::vector<AbstractMatrix<int>> matrix_array;
    for(int i=0;i<n;i++)
        matrix_array.push_back(AbstractMatrix<int>(random_list[i],random_list[i+1]));
    m = new int*[n]; // stores the minimal multiplication times
    s = new int*[n]; // stores dividing positions
    for(int i=0;i<n;i++)
    {
        m[i] = new int[n];
        s[i] = new int[n];
    }
    int l,i,j,k;
    for(int i=0;i<n;i++)
        m[i][i] = -1; // -1 stands for infinity
    for(l=2;l<n;l++) // l: the length of current group
    {
        for(i=0;i<=n-l;i++)
        {
            j = i+l-1;
            m[i][j] = -1;
            for(k=i;k<=j-1;k++)
            {
                int currTimes = matrix_array[i].getHeight()*matrix_array[k].getLength()*matrix_array[j].getLength()
                                +m[i][k]+m[k+1][j];
                if(currTimes<m[i][j] && m[i][j]!=-1)
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

void printOptimalSequnce(int n, int** &m, int** &s, int i, int j)
{
    if(i!=j)
    {
        int k = s[i][j];
        printOptimalSequnce(n,m,s,i,k);
        printOptimalSequnce(n,m,s,k+i,j);
        if(k==i)
        std::cout<< "Multiply matrix" << i << " and matrix" << j << std::endl;
        else
        std::cout<< "Multiply matrix from" << i << " to" << k << " and from "<<k+1<<" to" <<j<<std::endl;

        
    }
}

int main()
{
    std::vector<unsigned int> random_list;
    int** m;
    int** s;
    std::array<int,21> N_list = {5,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200};
    for(auto&& n:N_list)
    {
        random_list = getRandomList(n+1);
        run(n,random_list,m,s);
        printMatrix(random_list);
        printOptimalSequnce(n,m,s,0,n-1);

    }

}