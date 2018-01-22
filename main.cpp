
#include <cstdlib>

#include "getdata.hpp"
#include "nn.hpp"

using namespace std;


int main(){
    int total_correct = 0;
    int offset = 0;
    int j = 0;
    int epoch = 10000;
    double learningrate = 0.3;
    string str1 = "images";
    string str2 = "labels";
    string str3 = "network.dat";
    const char* imgfile = str1.c_str();
    const char* label = str2.c_str();
    const char* network_file = str3.c_str();
    srand(time(NULL));
    

    getdata img_object(imgfile, label, epoch);
    struct img image;
    int sub_layer[] = {784, 32,16 , 10}; 
    //nn* network = new nn(4, sub_layer, network_file, learningrate);
    nn network(network_file, learningrate);
    //nn network(4, sub_layer, network_file, learningrate);
    do{
        /*if(i%20 == 0){
            network = new nn(4, sub_layer, network_file, learningrate);
            learningrate += 0.005;
            cout<<endl;
        }*/
        total_correct = 0;
        for(int i = 0; i < epoch;++i){
            if(i%1000 == 0){
                //cout<<"#"<<endl;
            }
            image = img_object.getimage(rand()%10000);
            //cout<<img_data.value<<endl;
            network.train(image.img_data, img_object.size, image.img_value);
            //cout<<network.nn_guess<<endl;
            if(image.img_value == network.nn_guess)  total_correct++;
            if(i%10 == 9)network.descend();
            //img_object.print(image);
            //img_data.next_img();
        }
    
        cout<<"\n"<<j<<" rate "<<network.learningrate<<"percent correct "<<(total_correct/(double)epoch)<<endl;
        j++;
    }while(total_correct/(double)epoch < 0.97);
    //cout<<"percent correct "<<(total_correct/2000.0)<<endl;
   
    network.~nn();        
    return 0;
    
}
