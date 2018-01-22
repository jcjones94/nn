#ifndef my_macro
#define my_macro
#include <vector>
#include <cmath>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <random>
#endif 

struct node{
    double* weight;
    double bias;
    double value;
    double z;
};

class nn{

   public:
     double learningrate;
     int nn_guess;
     std::vector<std::vector<node> > layer;
     std::vector<std::vector<node> > grad;
     char *network_file;
     nn(int layers, int sub_layer[], const char *file, double);
     nn(const char *file, double);
     ~nn();
     bool train(unsigned char* img_data,int size, int value);
     void calculate_output();
     void back_prop();
     void init_network(unsigned char *img_data, int size);
     void descend();
    
   private:
     double sigmod(double);
     double sigmodprime(double);
     void cost(int);
     template <class T>
     T ltob(T);

};

