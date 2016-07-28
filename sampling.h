#pragma once

#include "traffic_array.h"

void run_sampling(traffic_array* traffic, int window_size, int sample_size, int sample_rate, double window_time, int number_of_flow_packets, bool sub_sampling, int sub_sample_size);
unsigned int* preliminary_sampling(traffic_array* traffic, int sample_size, int sample_rate, double window_time, int number_of_flow_packets, int current_flow);
void calculate_merge_sort(unsigned int** sample_array, unsigned int* sample, int sample_size, int window_size);
void insert_to_array_max_heap(unsigned int* sample, packet p, int sample_size);
int cardinality_compute(unsigned int* sample, int sample_size);
void calculate_sub_sample(unsigned int* sample, int sample_size, unsigned int* sub_sample, int sub_sample_size);
void sort_sample(unsigned int* sample, int sample_size);