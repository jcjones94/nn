#include "getdata.hpp"

using namespace std;


getdata::getdata(const char *img_file, const char *label_file, int load){
    	
    char *init_data = new char[16];
    
    images.open(img_file, ifstream::binary|ios::in);
    labels.open(label_file, ifstream::binary|ios::in);
  
    images.read(init_data, 16);
    size = ((int)(init_data[11]) & 0xff)*((int)init_data[15] & 0xff); 
    width = ((int)(init_data[11]) & 0xff);
    height = ((int)(init_data[15]) & 0xff);

    labels.read(init_data, 8); 
    imgs = vector<img>(load);
    char temp[1];
    for(int i = 0;i<load;++i){
       imgs[i].img_data = new unsigned char[size];
      
       labels.read(temp, 1);
       images.read((char*)imgs[i].img_data, size);
       imgs[i].img_value = temp[0] & 0xff;
    }

    //images.read((char*)data, size);
    //labels.read(init_data, 1);
    //value = init_data[0] & 0xff;  
}

void getdata::print(img image){
	
    for(int i = 0;i < height;i++){
        for(int n = 0; n < width;n++){
       	    if(image.img_data[width*i + n] > 0 )cout<<1;
            else cout<<0;
	  }
	cout<<endl;
  }
  std::cout<<image.img_value<<std::endl;
}
img getdata::getimage(int i){
   return imgs[i];
}
void getdata::next_img(){
   char temp[1];
   labels.read(temp, 1);
   images.read((char*)data, size);
   value = temp[0] & 0xff;
}

