#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <complex>
#include <random>
#include <cmath>
#include <chrono>
using namespace std;
class RandomGenerator
{
private:
    ifstream data_src;
    ofstream data_output;
public:
    RandomGenerator()
    {
        data_src = ifstream("../input/input.txt");
    }
    ~RandomGenerator()
    {
        data_src.close();
        if(data_output.is_open())
            data_output.close();
    }

    vector<long double> create_random_coeff(int n)
    {
        vector<long double> coeff_list;
        // read from file. If the file doesn't exist, create one.
        if(data_src.is_open())
        {
            for(int i=0;i<n;i++)
            {
                long double item;
                data_src>>item;
                coeff_list.push_back(item);
            }
        }
        else
        {
            data_output = std::ofstream("../input/input.txt");
            for(int i=0;i<n;i++)
            {
                int item = std::rand();
                data_output << (long double)item/10000.0L << std::endl;
                coeff_list.push_back((long double)item/10000.0L);
            }
            for(int i=0;i<500;i++)
            {
                int item = std::rand();
                data_output << (long double)item/10000.0L << std::endl;
            }
        }
            return coeff_list;
    }
    void rewind()
    {
        if(data_src.is_open())
            data_src.seekg(0);
    }
};

template<typename ElemType>
void FFT_adjust(vector<ElemType> &coeff_list, int &paddle_digit)
{
    int n = coeff_list.size();
    // first check if n is power of 2
    // this trick is from https://stackoverflow.com/questions/108318/whats-the-simplest-way-to-test-whether-a-number-is-a-power-of-2-in-c
    while((n & (n - 1)) != 0)
    {
        // push zero so n is power of 2
        coeff_list.push_back(0L);
        n++;paddle_digit++;
    }
}

template<typename ElemType>
void FFT_double(vector<ElemType> &coeff_list, int &paddle_digit)
{
    int n = coeff_list.size();
    // first check if n is power of 2
    // this trick is from https://stackoverflow.com/questions/108318/whats-the-simplest-way-to-test-whether-a-number-is-a-power-of-2-in-c
    for(int i=0;i<n;i++)
    {
        // push zero so n is power of 2
        coeff_list.push_back(0L);
    }
}

vector<complex<long double>> FFT(vector<long double> &coeff_list)
{
    vector<complex<long double>> val_list;
    // the size must have already been adjusted to power of 2 using `FFT_adjust`
    const long double PI = 3.14159265358979323846264338328;
    int n = coeff_list.size();
    complex<long double> base_index =complex<long double>(0,2*PI/n);
    complex<long double> curr_index =0;
    complex<long double> current_root = 1;
    if(n==1)
    {
        val_list.push_back(complex<long double>(coeff_list[0],0));
        return val_list;
    }
    // construct sub coefficient lists
    vector<long double> coeff_sub1,coeff_sub2;
    for(vector<long double>::iterator it = coeff_list.begin();it<coeff_list.end();it+=2)
    {
        coeff_sub1.push_back(*it);
    }
    for(vector<long double>::iterator it = coeff_list.begin()+1;it<coeff_list.end();it+=2)
    {
        coeff_sub2.push_back(*it);
    }
    vector<complex<long double>> val_list1 = FFT(coeff_sub1);
    vector<complex<long double>> val_list2 = FFT(coeff_sub2);
    val_list.resize(n,0);
    for(int i = 0;i< n/2;i++)
    {
        val_list[i] = val_list1[i] + current_root*val_list2[i];
        val_list[i+n/2] = val_list1[i] - current_root*val_list2[i];
        curr_index+=base_index;
        current_root = exp(curr_index);
    }
    return val_list;
}

vector<complex<long double>> FFT_reverse(vector<complex<long double>> &value_list)
{
    vector<complex<long double>> coeff_list;
    // the size must have already been adjusted to power of 2 using `FFT_adjust`
    const long double PI = 3.14159265358979323846264338328;
    int n = value_list.size();
    complex<long double> base_index =complex<long double>(0,2*PI/n);
    complex<long double> curr_index =0;
    complex<long double> current_root = 1;
    if(n==1)
    {
        coeff_list.push_back(value_list[0]);
        return coeff_list;
    }
    // construct sub coefficient lists
    vector<complex<long double>> value_sub1,value_sub2;
    for(vector<complex<long double>>::iterator it = value_list.begin();it<value_list.end();it+=2)
    {
        value_sub1.push_back(*it);
    }
    for(vector<complex<long double>>::iterator it = value_list.begin()+1;it<value_list.end();it+=2)
    {
        value_sub2.push_back(*it);
    }
    vector<complex<long double>> coeff_list1 = FFT_reverse(value_sub1);
    vector<complex<long double>> coeff_list2 = FFT_reverse(value_sub2);
    coeff_list.resize(n,0);
    for(int i = 0;i< n/2;i++)
    {
        coeff_list[i] = (coeff_list1[i] + current_root*coeff_list2[i]);
        coeff_list[i+n/2] = (coeff_list1[i] - current_root*coeff_list2[i]);
        curr_index-=base_index;
        current_root = exp(curr_index);
    }
    return coeff_list;
}

vector<complex<long double>> FFT_reverse_wrapper(vector<complex<long double>> &value_list)
{
    auto coeff_list = FFT_reverse(value_list);
    auto n_comp = complex<long double>(value_list.size(),0);
    for(int i=0;i<coeff_list.size();i++)
        coeff_list[i]/=n_comp;
    return coeff_list;
}

vector<complex<long double>> multiply_via_value(vector<complex<long double>> &val_list1, vector<complex<long double>> &val_list2)
{
    vector<complex<long double>> post_multi;
    // the size of the two must be equal
    for(int i=0;i<val_list1.size();i++)
        post_multi.push_back(val_list1[i]*val_list2[i]);
    return post_multi;
}

int run(vector<complex<long double>> &result,vector<long double> &lista, vector<long double> &listb)
{
    srand(time(0));
    // record how many zero digits have been paddled
    /* There are 3 circumstances when zero digits are needed:
       1. when before FFT is executed
       2. when before FFT reverse is executed
       3. when multiplying two value lists
       2 and 3 won't occur in this lab.
       But 3 may be encountered in other cases, when polynoials with different
       term numbers are multiplied.
     */
    int n = lista.size();
    int paddle_digit1=0,paddle_digit2=0;
    FFT_adjust(lista,paddle_digit1);
    FFT_adjust(listb,paddle_digit2);
    FFT_double(lista,paddle_digit1);
    FFT_double(listb,paddle_digit2);
    auto val_list1 = FFT(lista);
    auto val_list2 = FFT(listb);
    auto val_list_multi = multiply_via_value(val_list1,val_list2);
    result = FFT_reverse_wrapper(val_list_multi);
    for(int i=paddle_digit1+paddle_digit2+1;i>0;i--)
        result.pop_back();

}

int main()
{
    auto time_output = std::ofstream("../output/performance_analysis.txt");
    auto data_output = std::ofstream("../output/data.txt");
    RandomGenerator gen;
    array<int,32> n_list={4,16,32,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,130,140,150,160,170,180,190,200,210,220,230,240};
    time_output<<"{";
    for(auto&& n:n_list)
    {
        time_output<<"{"<<n<<",";
        gen.rewind();
        vector<complex<long double>> result;
        auto coeff1 = gen.create_random_coeff(n);
        auto coeff2 = gen.create_random_coeff(n);
        data_output << "Coefficients of the first polynomial with length "<<n<<":"<<endl;
        for(int i=0;i<n;i++)
            data_output<<coeff1[i]<<endl;
        data_output << "Coefficients of the second polynomial with length "<<n<<":"<<endl;
        for(int i=0;i<n;i++)
            data_output<<coeff2[i]<<endl;
        auto start = std::chrono::high_resolution_clock::now();
        run(result,coeff1,coeff2);
        auto stop = std::chrono::high_resolution_clock::now();
        data_output << "Coefficients of multiply results of n=" << n <<endl;
        time_output << (stop-start).count()<<"}"<<",";
        for(auto&& item:result) // length is not neccessarily n.
            data_output<<item.real()<<endl;
    }
    time_output<<"}";
    time_output.close();
    data_output.close();
    return 0;
    
}