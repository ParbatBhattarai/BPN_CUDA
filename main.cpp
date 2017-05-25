<<<<<<< HEAD
//#define USE_CUDA

//#include "cuda_runtime.h"
//#include "device_launch_parameters.h"

#define CUDA_HEADER
#include <stdlib.h>

#ifdef USE_CUDA
	#include "bpn_cuda.h"
#endif

#ifndef USE_CUDA
	#include "bpn.h"
#endif

#include "img_dataset_creator.h"
#include "mnist.h"

#define DEFAULT_NODE_COUNT 4000
#define DEFAULT_LAYER_COUNT 8

int main(int argc , char ** argv){

	int d_num , d_rows , d_cols , d_numl;
	double *dataset , *label;

	int n_count = DEFAULT_NODE_COUNT , n_layers = DEFAULT_LAYER_COUNT;

	if(argc > 1)
		n_count = atoi(argv[1]); 
	
	if(argc > 2)
		n_layers = atoi(argv[2]);


	Type *type = new Type[n_layers];
	int *layers = new int[n_layers];

	

	dataset = ReadMNISTIMAGE("train-images.idx3-ubyte" , d_num , d_rows , d_cols);
	label   = ReadMNISTLABEL("train-labels.idx1-ubyte" , d_numl);

	if(dataset == NULL || label == NULL){
		printf("Fatal: Couldn't read a MNIST file\n");
		exit(1);
	}

	if(d_num != d_numl){
		printf("Fatal: Image and label size didn't match.\n");
		exit(1);
	}


	int lambda = 2 * n_count / n_layers;

	if(lambda <= d_rows * d_cols){
=======
#include "common.h"
#include "bpn.h"
#include "bpn_cuda.h"

#include "img_dataset_creator.h"
#include "mnist.h"

#define TEST_OUTPUT "output.txt"

struct __data{
	double cpu_time;
	double gpu_time;
	double no_nodes;
	double no_weights;
	double no_layers;
	double no_iterations;
};

std::ofstream outfile_bin , outfile;

void write_file_binary(__data data){
	if(!outfile_bin.is_open())
		outfile.open("output.bin-utf8" , std::ios::binary);

	outfile.write((char*)&data , sizeof(data));
	return;
}

void close_file(){
	outfile.close();
	return;
}

void write_file(__data data){
	if(!outfile.is_open())
		outfile.open("output.txt");
	outfile<<"For Node Count: "<<data.no_nodes;
	outfile<<"\n\tTotal Number of Layers: "<<data.no_layers;
	outfile<<"\n\tTotal Number of Weights: "<<data.no_weights;
	outfile<<"\n\tTotal Iterations: "<<data.no_iterations;
	
	outfile<<"\n\tCPU Time: "<<data.cpu_time;
	outfile<<"\n\tGPU Time: "<<data.gpu_time<<"\n\n\n";
	return;
}


#define DEFAULT_TEST_CONFIG "test.conf"

void read_from_file(int** no_layers , int** no_nodes , int** no_minst , int** iterations , int& total){
	std::ifstream infile(DEFAULT_TEST_CONFIG);
	std::istream_iterator<int> begin(infile);
	std::istream_iterator<int> end;

	std::vector<int> contents;
	std::copy(begin , end , std::back_inserter(contents));

	total = contents.size() / 4;

	*no_layers = new int[total];
	*no_nodes = new int[total];
	*iterations = new int[total];
	*no_minst = new int[total];

	for(int i = 0 ; i < total ; i ++){
		*(*no_layers + i) = contents[i * 4 + 0];
		*(*no_nodes + i) = contents[i * 4 + 1];
		*(*no_minst + i) = contents[i * 4 + 2];
		*(*iterations + i) = contents[i * 4 + 3];
	}

	return;
}

Type* create_type(int node_count , Type t = Linear){
	Type* type = new Type[node_count];
	for(int i = 0 ; i < node_count - 1 ; i ++)
		type[i] = t;

	return type;
}

int* create_layers(int n_count , int input_size , int n_layers){
	int* layers = new int[n_layers];
	int lambda = 2 * n_count / n_layers;

	if(lambda <= input_size){
>>>>>>> f1d2786c10ab6d111ef46c4a7e3a62789bd140a2
		printf("Fatal: Input vector too large for the given node count.\n");
		exit(1);
	}

	for(int i = 0 ; i < n_layers ; i ++){
		if(i % 4 == 0)
<<<<<<< HEAD
			layers[i] = d_rows * d_cols;
		else if(i % 4 == 1)
			layers[i] = lambda - 1;
		else if(i % 4 == 2)
			layers[i] = lambda - d_rows * d_cols;
		else
			layers[i] = 1;
	
		type[i] = Linear;
	}
	
	double rate;

#ifndef USE_CUDA
	BPN* network;
	network = new BPN;
	rate = 0.0001;  /*Don't know the problem, but the network diverges for 
				large training_rate when CPU is used, but converges too 
				slow for small training_rate when CUDA is used*/

#else
	BPN_CUDA* network;
	network = new BPN_CUDA;
	rate = 0.0001;
#endif

	initialize(network , layers , n_layers , type , rate);
	
	clock_t begin = clock();
	int count = train(network , dataset , label , d_num , d_rows * d_cols , 1 , 5);
	clock_t end = clock();

	double time_spent = (double)(end - begin) / (CLOCKS_PER_SEC * count) * 1000;
	
	printf("Network Information:\n");
	printf("Number of Nodes : %d \n Number of Weight connections : %d \n" , network->noNodes , network->noWeight);
	printf("Time Spent : %f" , time_spent);
	


=======
			layers[i] = input_size;
		else if(i % 4 == 1)
			layers[i] = lambda - 1;
		else if(i % 4 == 2)
			layers[i] = lambda - input_size;
		else
			layers[i] = 1;
	}

	return layers;
}

int main(){

	
	int *total_layers , *total_nodes , *iterations , *sample_size , total;
	
	read_from_file(&total_layers , &total_nodes , &sample_size , &iterations , total);

	int d_num , d_rows , d_cols , d_numl;
	double *dataset , *label;

	dataset = ReadMNISTIMAGE("train-images.idx3-ubyte" , d_num , d_rows , d_cols);
	label   = ReadMNISTLABEL("train-labels.idx1-ubyte" , d_numl);

	if(dataset == NULL || label == NULL){
		printf("Fatal: Couldn't read a MNIST file\n");
		exit(1);
	}

	if(d_num != d_numl){
		printf("Fatal: Image and label size didn't match.\n");
		exit(1);
	}

	__data _write_out;
	
	double rate = 0.0001;

	CPU::BPN* network;
	network = new CPU::BPN;
	
	GPU::BPN_CUDA* _network;
	_network = new GPU::BPN_CUDA;

	//Begin Iteration
	for(int i = 0 ; i < total ; i ++){
		Type *type = create_type(total_layers[i]);
		int *layers = create_layers(total_nodes[i] , d_rows * d_cols , total_layers[i]);
		double time_spent;
	
		_write_out.no_layers = total_layers[i];
		_write_out.no_nodes = total_nodes[i];
		_write_out.no_iterations = iterations[i];

		for(int j = 0 ; j < 2 ; j ++){
			
			if(j == 0){
				CPU::initialize(network , layers , total_layers[i] , type , rate);
				clock_t begin = clock();
				int count = CPU::train(network , dataset , label , sample_size[i] , d_rows * d_cols , 1 , iterations[i]);
				clock_t end = clock();

				_write_out.no_weights = network->noWeight;
				_write_out.cpu_time = (double)(end - begin) / (CLOCKS_PER_SEC * count) * 1000;

			}
			else{
				GPU::initialize(_network , layers , total_layers[i] , type , rate);
				clock_t begin = clock();
				int count = GPU::train(_network , dataset , label , sample_size[i] , d_rows * d_cols , 1 , iterations[i]);
				clock_t end = clock();

				_write_out.gpu_time = (double)(end - begin) / (CLOCKS_PER_SEC * count) * 1000;
	
			}

		}
		printf("\n\nNetwork Information:\n");
		printf("\tNumber of Nodes : %d \n Number of Weight connections : %d \n" , _write_out.no_nodes , _write_out.no_weights);
		printf("\tCPU Time : %f\n\tGPU Time : %f" , _write_out.cpu_time , _write_out.gpu_time);
			
		write_file(_write_out);
		write_file_binary(_write_out);

		delete[] type , layers;

	}
>>>>>>> f1d2786c10ab6d111ef46c4a7e3a62789bd140a2
	return 0;
}

