#include "sampling.h"

void run_sampling(traffic_array* traffic, int window_size, int sample_size, int sample_rate, double window_time, int number_of_flow_packets, bool sub_sampling, int sub_sample_size) {
	std::ofstream output;
	output.open("output.txt");
	unsigned int** sample_array = new unsigned int*[window_size];
	if (!sample_array) {
		exit(1);
	}
	int current_flow = 1, cardinality = 0;
	unsigned int* sample = new unsigned int[sample_size];
	if (!sample) {
		exit(1);
	}
	unsigned int* sub_sample = new unsigned int[sub_sample_size];
	if (!sub_sample) {
		exit(1);
	}
	int temp_flow = (int)ceil(1 / window_time);
	while (current_flow <= traffic->return_flow_array_size()) {
		sample_array[current_flow % window_size] = preliminary_sampling(traffic, sample_size, sample_rate, window_time, number_of_flow_packets, current_flow - 1);
		temp_flow--;
		if (current_flow >= window_size) {
			std::fill(sample, sample + sample_size, 0);
			calculate_merge_sort(sample_array,sample,sample_size,window_size);
			if (sub_sampling) {
				std::fill(sub_sample, sub_sample + sub_sample_size, 0);
				calculate_sub_sample(sample, sample_size, sub_sample, sub_sample_size);
				cardinality = cardinality_compute(sub_sample, sub_sample_size);
			}
			else {
				cardinality = cardinality_compute(sample, sample_size);
			}
			//std::cout << "there are " << cardinality << " distinct packets at stream no " << (int)(current_flow) << std::endl;
			std::cout << "there are " << (int)(cardinality * 500) << " distinct packets at stream no " << (int)(current_flow) << std::endl;
			output << (int)(cardinality * 500) << std::endl;
		}
		if (temp_flow == 0) {
			current_flow++;
			temp_flow = (int)ceil(1 / window_time);
		}
	}
	output.close();
	for (int i = 0; i < window_size; i++) {
		delete[] sample_array[i];
	}
	delete[] sample_array;
}

unsigned int* preliminary_sampling(traffic_array* traffic, int sample_size, int sample_rate, double window_time, int number_of_flow_packets, int current_flow) {
	unsigned int* sample = new unsigned int[sample_size]();
	if (!sample) {
		exit(1);
	}
	packet p;
	int next_random_packet;
	int number_of_packets = (int)((number_of_flow_packets + traffic->current_flow_size(current_flow))*(double)window_time);
	int i = 0;
	while (i < number_of_packets +1) {
		next_random_packet = rand() % sample_rate;
		for (int j = 0; j < sample_rate; j++) {
			p = traffic->next_packet();
			i++;
			if (p.is_last_in_flow()) {
				break;
			}
			if (j == next_random_packet) {
				insert_to_array_max_heap(sample, p, sample_size);
			}
		}
	}
	sort_sample(sample, sample_size);
	return sample;
}

void calculate_merge_sort(unsigned int** sample_array, unsigned int* sample, int sample_size, int window_size) {
	for (int i = 0; i < window_size; i++) {
		int pointer_src = 0, pointer_dst = 0;
		for (int j = 0; j < sample_size; j++) {
			if (sample_array[i][pointer_src] == 0) {
				break;
			}
			if (sample_array[i][pointer_src] > sample[pointer_dst]) {
				for (int k = sample_size; k > pointer_dst; k--) {
					sample[k] = sample[k - 1];
				}
				sample[pointer_dst] = sample_array[i][pointer_src];
				pointer_src++;
				pointer_dst++;
			}
			else {
				pointer_dst++;
			}
		}
	}
}

void insert_to_array_max_heap(unsigned int* sample, packet p, int sample_size) {
	unsigned int hash = p.get_hash();
	if (hash > sample[0]) {
		unsigned int temp;
		sample[0] = hash;
		int i = 0;
		while (i * 2 + 1 < sample_size && sample[i] > sample[i * 2 + 1]) {
			if (i * 2 + 2 < sample_size && sample[i * 2 + 2] < sample[i * 2 + 1]) {
				temp = sample[i * 2 + 2];
				sample[i * 2 + 2] = sample[i];
				sample[i] = temp;
				i = i * 2 + 2;
			}
			else {
				temp = sample[i * 2 + 1];
				sample[i * 2 + 1] = sample[i];
				sample[i] = temp;
				i = i * 2 + 1;
			}
		}
	}
}

int cardinality_compute(unsigned int* sample, int sample_size) {
	int cardinality_count = 0;
	bool cardinality;
	for (int i = 0; i < sample_size; i++) {
		cardinality = true;
		for (int j = 0; j < sample_size; j++) {
			if (i != j && sample[i] == sample[j]) {
				cardinality = false;
				break;
			}
		}
		if (cardinality == true) {
			cardinality_count++;
		}
	}
	return cardinality_count;
}

void calculate_sub_sample(unsigned int* sample, int sample_size, unsigned int* sub_sample, int sub_sample_size) {
	if (sample_size >= sub_sample_size) {
		int position;
		for (int i = 0; i < sub_sample_size; i++) {
			position = rand() % (sample_size - i);
			sub_sample[i] = sample[position];
			sample[position] = sample[sample_size - i - 1];
		}
	}
}

void sort_sample(unsigned int* sample, int sample_size) {
	unsigned int temp;
	for (int j = 1, new_size = sample_size; j <= sample_size; j++, new_size--) {
		temp = sample[sample_size - j];
		sample[sample_size - j] = sample[0];
		sample[0] = temp;
		int i = 0;
		while (i * 2 + 1 < new_size && sample[i] > sample[i * 2 + 1]) {
			if (i * 2 + 2 < new_size && sample[i * 2 + 2] < sample[i * 2 + 1]) {
				temp = sample[i * 2 + 2];
				sample[i * 2 + 2] = sample[i];
				sample[i] = temp;
				i = i * 2 + 2;
			}
			else {
				temp = sample[i * 2 + 1];
				sample[i * 2 + 1] = sample[i];
				sample[i] = temp;
				i = i * 2 + 1;
			}
		}
	}
}