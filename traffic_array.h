#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "packet.h"

class traffic_array {
private:
	int* flow_array;
	int flow_array_size;
	packet* users;
	int number_of_users;
	int packets_pet_user;
	double user_deviation;
	int current_flow;
	int current_number_of_users;
	int current_packet_number;
	int number_of_packets;
	packet** malicious;
public:
	traffic_array(char* file_name, int number_of_users, double user_deviation, int packets_per_user);
	~traffic_array();
	packet next_packet();
	int initialization_ckeck();
	int current_flow_size(int flow_number);
	int return_flow_array_size();
};
