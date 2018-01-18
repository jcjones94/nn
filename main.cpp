
#include <cstdlib>

#include "getdata.hpp"
#include "nn.hpp"

using namespace std;


int main(){
    int total_correct = 0;
    string str1 = "images";
    string str2 = "labels";
    string str3 = "network.dat";
    const char* img = str1.c_str();
    const char* label = str2.c_str();
    const char* network_file = str3.c_str();
    
    getdata img_data(img, label);

    int sub_layer[] = {784, 16, 16, 10}; 
    nn* network = new nn(4, sub_layer, network_file);
//    nn* network = new nn(network_file);
    for(int i = 0; i < 5000;++i){
        cout<<img_data.value<<endl;
        network->train(img_data.data, img_data.size, img_data.value);
        if(img_data.value == network->nn_guess)  total_correct++;
        img_data.next_img();
	   if(i%10 == 9)network->descend();
    }
    cout<<"percent correct "<<(total_correct/5000.0)<<endl;
   
    network->~nn();        
    return 0;
    
}
