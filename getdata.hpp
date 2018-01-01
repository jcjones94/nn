#ifndef GETDATA_HPP
#define GETDATA_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>
#endif

class getdata{
  public:
    unsigned char* data;
    int width;
    int height;
    int size;
    int value;
    std::ifstream images;
    std::ifstream labels;
    
    getdata(const char *, const char *);

    void print(void);

    void next_img(void);
  private:

};
