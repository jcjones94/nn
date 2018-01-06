
#include "nn.hpp"

using namespace std;
nn::nn(int layers, int sub_layer[],const char *file){
   network_file = (char *)file;
   srand(time(NULL));
   layer = vector<vector<node> >(layers);
   grad = vector<vector<node> >(layers);
   /*if(layers != sizeof(sub_layer)){
	cout<<"number of layers doesnt equal number sub_layers: constructor"<<endl;
   }*/
   for(int i = 0; i< layers;++i){
	layer[i] = vector<node>(sub_layer[i]);
	grad[i] = vector<node>(sub_layer[i]);
   }
   for(int i = 0; i< (layers - 1);++i){
	for(int j = 0; j < sub_layer[i];++j){
	    layer[i][j].weight = new double[layer[i+1].size()];
	    grad[i][j].weight = new double[layer[i+1].size()];
	    for(int n = 0; n < layer[i+1].size();++n){
		layer[i][j].weight[n] = ((double)(rand()%100))/10.0 - 5.0;
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
    grad = vector<vector<node> >(specs);
   
    cout<<specs<<endl;
    for(int i = 0;i<layer.size(); ++i){
	network.read((char *)&specs, 4);
        layer[i] = vector<node>(specs);
        grad[i] = vector<node>(specs);
        
    }    
    for(int i = 1; i < layer.size(); ++i){
        for(int j = 0; j < layer[i-1].size();++j){
	    layer[i-1][j].weight = new double[(layer[i].size())*sizeof(double)];
	    grad[i-1][j].weight = new double[(layer[i].size())*sizeof(double)];
	    network.read((char*)layer[i-1][j].weight, layer[i].size()*sizeof(double));
            network.read((char*)&layer[i-1][j].bias, sizeof(double));
	    grad[i][j].value = 0.0;
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

    for(int i = 0; i < grad[grad.size() - 1].size();++i){
	int x;
	if(img_value == i){
	    x = 1;
	}else{
	    x = 0;
	}
	grad[grad.size() - 1][i].value = 2*(x - layer[layer.size() - 1][i].value);
	//cout<<grad[grad.size() - 1][i].value<<endl;
    }

    for(int l = grad.size() - 1; l > 0; --l){
	for(int i = 0; i < grad[l].size();++i){
	    for(int j = 0; j < grad[l - 1].size();++j){
		grad[l - 1][j].weight[i] += grad[l][i].value*layer[l-1][j].value/(2*pow(1+abs(grad[l][i].z),2));
//		if(l > 2)cout<<grad[l-1][j].weight[i]<<" : ";
		grad[l - 1][j].value += grad[l][i].value*layer[l-1][j].weight[i]/(2*pow(1+abs(grad[l][i].z),2));
//		if(j == 0 && l == 3)cout<<grad[l-1][j].value<<endl;
	    }
	    
	    grad[l][i].bias += grad[l][i].value/(2*pow(1+abs(grad[l][i].z),2));
//	    cout<<layer[l][i].value<<endl;
	}
    }

    //descend();
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
		//if(i < 1) cout<<layer[n-1][j].value<<endl;
	    	layer[n][i].value += layer[n-1][j].value*layer[n-1][j].weight[i];
	    }
	    layer[n][i].value += layer[n][i].bias;
	    layer[n][i].z = layer[n][i].value;
	    layer[n][i].value = layer[n][i].value/(2.0*(1.0+abs(layer[n][i].value))) + 0.5;
	    grad[n][i].value = 0;

    	  //  cout<<i<<" : "<<layer[n][i].value<<endl;
    	}
    }
    for(int i = 0;i < 16;++i){
	cout<<layer[3][i].value<<endl;
	//if(i%28 == 0)cout<<endl;
    }

}

void nn::descend(){
   
    for(int l = 1; l < grad.size();++l){
	for(int i = 0; i < grad[l].size();++i){
	   //if(l==grad.size() -2)cout<<grad[l][i].value<<endl;
	   for(int j = 0; j < grad[l- 1].size();++j){
		layer[l - 1][j].weight[i] += grad[l-1][j].weight[i];
		grad[l-1][j].weight[i] = 0.0;
	//	if(l == grad.size() - 1)cout<<":"<<grad[l-1][j].weight[i];
	   }
	  // cout<<endl;
	   layer[l][i].bias += grad[l][i].bias;
	   grad[l][i].bias = 0.0;
	   grad[l][i].value = 0.0;
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
