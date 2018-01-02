
#include "nn.hpp"

using namespace std;
nn::nn(int layers, int sub_layer[],const char *file){
   network_file = (char *)file;
   layer = vector<vector<node> >(layers);
   /*if(layers != sizeof(sub_layer)){
	cout<<"number of layers doesnt equal number sub_layers: constructor"<<endl;
   }*/
   for(int i = 0; i< layers;++i){
	layer[i] = vector<node>(sub_layer[i]);
   }
   for(int i = 0; i< (layers - 1);++i){
	for(int j = 0; j < sub_layer[i];++j){
	    layer[i][j].weight = new double[layer[i+1].size()];
	    for(int n = 0; n < layer[i+1].size();++n){
		layer[i][j].weight[n] = 0.0;
	    }
	}
   }
   cout<<"constructed"<<endl;
}

nn::nn(const char *file){
    int specs;
    network_file = (char *)file;
    std::ifstream network(file);
    
    network.read((char*)&specs, 4);
    cout<<specs<<endl;
    
    layer = vector<vector<node> >(specs);
    cout<<specs<<endl;
    for(int i = 0;i<layer.size(); ++i){
	network.read((char *)&specs, 4);
        layer[i] = vector<node>(specs);
        
    }    
    for(int i = 1; i < layer.size(); ++i){
        for(int j = 0; j < layer[i-1].size();++j){
	    layer[i-1][j].weight = new double[(layer[i].size())*sizeof(double)];
	    network.read((char*)layer[i-1][j].weight, layer[i].size()*sizeof(double));
            network.read((char*)&layer[i-1][j].bias, sizeof(double));
        }
    }
}

nn::~nn(){
    int temp = layer.size();
    if(temp == 0) return;
    ofstream network(network_file);
    network.write((char *)&temp, sizeof(int));
    cout<<"printed "<< layer.size()<<endl;
    for(int i = 0; i < layer.size(); ++i){
        temp = layer[i].size();
	network.write((char *)&temp,sizeof(int));
        cout<<"printed "<< layer[i].size()<<endl;
    }
    for(int i = 0; i < (layer.size() - 1);++i){
        for(int j = 0; j < layer[i].size(); ++j){
    	    network.write((char *)layer[i][j].weight, layer[i+1].size()*sizeof(double));
            network.write((char *)&layer[i][j].bias, sizeof(double));
	}
    }
    cout<<"destructed"<<endl;
}
bool nn::train(unsigned char *img_data,int img_size, int img_value){

    
    init_network(img_data, img_size);  
    calculate_output();

    return 0;
}

void nn::init_network(unsigned char *img_data, int img_size){
    for(int i = 0; i < layer[0].size(); ++i){
	layer[0][i].value = (double)img_data[i];
    }
}

void nn::calculate_output(){
    for(int n = 1; n < layer.size(); ++n){
	for(int i = 0; i < layer[n].size(); ++i){
	    for(int j = 0; j < layer[n-1].size();++j){
	    	layer[n][i].value += layer[n-1][j].value*layer[n-1][j].weight[i];
	    }
	    layer[n][i].value += layer[n][i].bias;
	    layer[n][i].value = layer[n][i].value/(2.0*(1.0+abs(layer[n][i].value))) + 0.5;

    	    cout<<i<<" : "<<layer[n][i].value<<endl;
    	}
    }

}

void nn::back_prop(){
    

}


// thought i needed it but turn out didnt
template <typename T>
T nn::ltob(T value){
    char *temp = (char *)&value;
    char c;
    for(int i = 0; i< sizeof(T); ++i){
	c = temp[i];
	temp[i] = temp[sizeof(T) - i];
	temp[sizeof(T) - 1] = c;
    }   
    return (int)*temp;
}
