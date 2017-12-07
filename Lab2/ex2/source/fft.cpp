#include <iostream>
#include <fstream>
#include <vector>
#include <complex>
#include <random>
#include <cmath>
using namespace std;

vector<double> create_random_coeff(int n)
{
    vector<double> coeff_list;
    // read from file. If the file doesn't exist, create one.
    auto data_src = ifstream("../input/input.txt");
    if(data_src.is_open())
    {
        for(int i=0;i<n;i++)
        {
            double item;
            data_src>>item;
            coeff_list.push_back(item);
        }
    }
    else
    {
        auto data_output = std::ofstream("../input/input.txt");
        for(int i=0;i<10;i++)
        {
            int item = std::rand();
            data_output << (double)item/10000.0L << std::endl;
            coeff_list.push_back((double)item/10000.0L);
        }
        
    }
        return coeff_list;
}
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

vector<complex<double>> FFT(vector<double> &coeff_list)
{
    vector<complex<double>> val_list;
    // the size must have already been adjusted to power of 2 using `FFT_adjust`
    const double PI = std::acos(-1);
    int n = coeff_list.size();
    complex<double> base_root = exp(2*PI/n);
    complex<double> current_root = 1;
    if(n==1)
    {
        val_list.push_back(complex<double>(coeff_list[0],0));
        return val_list;
    }
    // construct sub coefficient lists
    vector<double> coeff_sub1,coeff_sub2;
    for(vector<double>::iterator it = coeff_list.begin();it<coeff_list.end();it+=2)
    {
        coeff_sub1.push_back(*it);
    }
    for(vector<double>::iterator it = coeff_list.begin()+1;it<coeff_list.end();it+=2)
    {
        coeff_sub2.push_back(*it);
    }
    vector<complex<double>> val_list1 = FFT(coeff_sub1);
    vector<complex<double>> val_list2 = FFT(coeff_sub2);
    val_list.reserve(n);
    for(int i = 0;i< n/2;i++)
    {
        val_list[i] = val_list1[i] + current_root*val_list2[i];
        val_list[i+n/2] = val_list1[i] - current_root*val_list2[i];
        current_root = current_root*base_root;
    }
    return val_list;
}

vector<complex<double>> FFT_reverse(vector<complex<double>> &value_list)
{
    vector<complex<double>> coeff_list;
    // the size must have already been adjusted to power of 2 using `FFT_adjust`
    const double PI = std::acos(-1);
    int n = value_list.size();
    complex<double> base_root = exp(-2*PI/n);
    complex<double> current_root = 1;
    if(n==1)
    {
        coeff_list.push_back(value_list[0]);
        return coeff_list;
    }
    // construct sub coefficient lists
    vector<complex<double>> coeff_sub1,coeff_sub2;
    for(vector<complex<double>>::iterator it = value_list.begin();it<value_list.end();it+=2)
    {
        coeff_sub1.push_back(*it);
    }
    for(vector<complex<double>>::iterator it = value_list.begin()+1;it<value_list.end();it+=2)
    {
        coeff_sub2.push_back(*it);
    }
    vector<complex<double>> coeff_list1 = FFT_reverse(coeff_sub1);
    vector<complex<double>> coeff_list2 = FFT_reverse(coeff_sub2);
    coeff_list.reserve(n);
    for(int i = 0;i< n/2;i++)
    {
        coeff_list[i] = coeff_list1[i] + current_root*coeff_list2[i];
        coeff_list[i+n/2] = coeff_list1[i] - current_root*coeff_list2[i];
    }
    return coeff_list;
}

vector<complex<double>> multiply_via_value(vector<complex<double>> &val_list1, vector<complex<double>> &val_list2)
{
    vector<complex<double>> post_multi;
    // the size of the two must be equal
    for(int i=0;i<val_list1.size();i++)
        post_multi.push_back(val_list1[i]*val_list2[i]);
    return post_multi;
}

int run(int n, vector<double> &lista, vector<double> &listb)
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
    int paddle_digit1=0,paddle_digit2=0;
    FFT_adjust(lista,paddle_digit1);
    FFT_adjust(listb,paddle_digit2);
    FFT_double(lista,paddle_digit1);
    FFT_double(listb,paddle_digit2);
    auto val_list1 = FFT(lista);
    auto val_list2 = FFT(listb);
    auto val_list_multi = multiply_via_value(val_list1,val_list2);
    auto coeff_multi = FFT_reverse(val_list_multi);
    for(int i=paddle_digit1+paddle_digit2;i>0;i--)
        coeff_multi.pop_back();

}

int main()
{
    int n = 32;
    auto coeff1 = create_random_coeff(n);
    auto coeff2 = create_random_coeff(n);
    run(32,coeff1,coeff2);
}