#ifndef my_macro
#define my_macro
#include <vector>
#include <cmath>
#include <fstream>
#include <iostream>
#endif 

struct node{
    double* weight;
    double bias;
    int value;
};

class nn{

   public:
     std::vector<std::vector<node> > layer;
     char *network_file;
     nn(int layers, int sub_layer[], const char *file);
     nn(const char *file);
     ~nn();
     bool train(unsigned char* img_data,int size, int value);
     void calculate_output();

   private:
     template <class T>
     T ltob(T);
};
