#ifndef GETDATA_HPP
#define GETDATA_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <stdint.h>
#endif
struct img{
    int img_value;
    unsigned char* img_data;
};
class getdata{
  public:
    std::vector<img> imgs;

    unsigned char* data;
    int width;
    int height;
    int size;
    int value;
    std::ifstream images;
    std::ifstream labels;
    
    getdata(const char *, const char *, int);

    void print(img);
    img getimage(int i);

    void next_img(void);
  private:

};
