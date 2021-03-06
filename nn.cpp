
#include "nn.hpp"

using namespace std;
nn::nn(int layers, int sub_layer[],const char *file, double rate){
    learningrate = rate;
   network_file = (char *)file;
   srand(time(NULL));
   std::default_random_engine generator;
   std::normal_distribution<double> distribution(0.0,1.0);
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
		   layer[i][j].weight[n] = distribution(generator)/*((double)(rand()%100))/10.0 - 5.0*/;
	    }
        layer[i][j].bias = distribution(generator); //added
	}
   }
   cout<<"constructed"<<endl;
}

nn::nn(const char *file, double rate){
    learningrate = rate;
    int specs;
    network_file = (char *)file;
    std::ifstream network(file);
    
    network.read((char*)&specs, 4);
    cout<<specs<<endl;
    
    layer = vector<vector<node> >(specs);
    grad = vector<vector<node> >(specs);
   
   // cout<<specs<<endl;
    for(int i = 0;i<layer.size(); ++i){
	    network.read((char *)&specs, 4);
        cout<<specs<<" ";
        layer[i] = vector<node>(specs);
        grad[i] = vector<node>(specs);
        cout<<layer[i].size()<<endl;
        
    }  
    cout<<"DSAD"<<endl;  
    for(int i = 1; i < layer.size(); ++i){
        for(int j = 0; j < layer[i-1].size();++j){
	        layer[i-1][j].weight = new double[(layer[i].size())*sizeof(double)];
	        grad[i-1][j].weight = new double[(layer[i].size())*sizeof(double)];
	        network.read((char*)layer[i-1][j].weight, layer[i].size()*sizeof(double));
            network.read((char*)&layer[i-1][j].bias, sizeof(double));
            /*for(int n = 0; n < layer[i].size();++i){
                grad[i][j].weight[n] = 0.0;
            }*/
	        grad[i][j].value = 0.0;
        }
    }
    cout<<"DSAD"<<endl;  
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

    cost(img_value);

    
    for(int l = grad.size() - 1; l > 0; --l){
	    for(int i = 0; i < grad[l].size();++i){
	        for(int j = 0; j < grad[l - 1].size();++j){
		        grad[l - 1][j].weight[i] += grad[l][i].value*layer[l-1][j].value*sigmodprime(layer[l][i].z);

		        grad[l - 1][j].value += grad[l][i].value*layer[l-1][j].weight[i]*sigmodprime(layer[l][i].z);

                //cout<<layer[l-1][j].weight[i]<<" ";
	        }
	        grad[l][i].bias += grad[l][i].value*sigmodprime(layer[l][i].z);
            //cout<<layer[l][i].z<<" ";
            //cout<<endl;
	    }
        
    }
    
    //cout<<endl;
    return 0;
}

void nn::init_network(unsigned char *img_data, int img_size){
    for(int i = 0; i < layer[0].size(); ++i){
	   layer[0][i].value = (double)img_data[i]/255.0;
       //if(i%28 == 0) cout<<endl;
       //cout<<(double)img_data[i]<<" ";

    }
}

void nn::calculate_output(){
    for(int n = 1; n < layer.size(); ++n){
    	for(int i = 0; i < layer[n].size(); ++i){
    	    for(int j = 0; j < layer[n-1].size();++j){
    		
    	    	layer[n][i].value += layer[n-1][j].value*layer[n-1][j].weight[i];
    	    }
    	    layer[n][i].value += layer[n][i].bias;
    	    layer[n][i].z = layer[n][i].value;
    	    layer[n][i].value = sigmod(layer[n][i].value);
    	    grad[n][i].value = 0.0;
        }
    }
    for(int i = 0; i < 10;++i){
       //cout<<layer[layer.size() - 1][i].value<<endl;
    }
    
}

void nn::descend(){
    //cout<<grad[0][0].value<<endl;
    for(int l = 1; l < grad.size();++l){
    	for(int i = 0; i < grad[l].size();++i){
    	   
    	   for(int j = 0; j < grad[l- 1].size();++j){
    		  layer[l - 1][j].weight[i] -= grad[l-1][j].weight[i]*learningrate;
              //cout<<grad[l-1][100].weight[i]<< " ";
    		  grad[l-1][j].weight[i] = 0.0;
    	
    	   }
    	   //cout<<grad[l][12].bias<<endl;
    	   layer[l][i].bias -= grad[l][i].bias*learningrate;
    	   grad[l][i].bias = 0.0;
    	   //grad[l][i].value = 0.0;

	   }
    }

}

void nn::back_prop(){
    

}

double nn::sigmod(double x){

    return (1.0/(1.0+exp(-x)));
}
double nn::sigmodprime(double x){

    return sigmod(x)*(1 - sigmod(x));
}

void nn::cost(int img_value){
    double brightest = 0.0;
    for(int i = 0; i < grad[grad.size() - 1].size();++i){
        int x;
        if(img_value == i){
            x = 1;
        }else{
            x = 0;
        }
        if(layer[layer.size() - 1][i].value > brightest){
            brightest = layer[layer.size() - 1][i].value;
            nn_guess = i;
        }
        grad[grad.size() - 1][i].value = (layer[layer.size() - 1][i].value - x);
    }
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
