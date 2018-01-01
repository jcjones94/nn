#include "getdata.hpp"

using namespace std;


getdata::getdata(const char *img_file, const char *label_file){
    	
    char *init_data = new char[16];
    
    images.open(img_file, ifstream::binary|ios::in);
    labels.open(label_file, ifstream::binary|ios::in);
  
    images.read(init_data, 16);
    size = ((int)(init_data[11]) & 0xff)*((int)init_data[15] & 0xff); 
    width = ((int)(init_data[11]) & 0xff);
    height = ((int)(init_data[15]) & 0xff);

    labels.read(init_data, 8); 
    data = new unsigned char[size];

    images.read((char*)data, size);
    labels.read(init_data, 1);
    value = init_data[0] & 0xff;  
}

void getdata::print(){
	
    for(int i = 0;i < height;i++){
        for(int n = 0; n < width;n++){
       	    cout<<hex<<(data[width*i + n] & 0xff)<< ", ";
	}
	cout<<endl;
    }
    std::cout<<value<<std::endl;
}

void getdata::next_img(){
   char temp[1];
   labels.read(temp, 1);
   images.read((char*)data, size);
   value = temp[0] & 0xff;
}

