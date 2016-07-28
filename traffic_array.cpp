#include "traffic_array.h"

#define DIVIDER 1000000 // used to allocate large numbers of packets

traffic_array::traffic_array(char* file_name, int number_of_users, double user_deviation, int packets_per_user) {
	std::cout << "Starting Initialization Process" << std::endl;
	std::ifstream flow_file;
	std::string line;
	int i = 0;
	flow_file.open(file_name, std::ios::in);
	if (flow_file.is_open()){
		while (std::getline(flow_file, line))
		{
			i++;
		}
		this->flow_array = new int[i];
		if (!this->flow_array) {
			std::cout << "bad allocation" << std::endl;
			exit(1);
		}
		this->flow_array_size = i;
		flow_file.clear();
		flow_file.seekg(0, std::ios::beg);
		i = 0;
		this->number_of_packets = 0;
		while (std::getline(flow_file, line))
		{
			this->flow_array[i] = atoi(line.c_str());
			this->number_of_packets += this->flow_array[i];
			i++;
		}
		flow_file.close();
	}
	this->users = new packet[number_of_users];
	for (int j = 0; j < number_of_users; j++) {
		this->users[j] = packet();
	}
	this->number_of_users = number_of_users;
	this->user_deviation = user_deviation;
	this->packets_pet_user = packets_per_user;
	this->current_flow = 0;
	this->current_number_of_users = 0;
	this->current_packet_number = 0;
	this->number_of_packets *= (int)2;
	int divided_number_of_packets = (int)ceil(number_of_packets / DIVIDER);
	if (divided_number_of_packets > 71) {
		std::cout << "cannot allocate more than 2GB of space (71 Million packets)" << std::endl;
		exit(1);
	}
	this->malicious = new packet*[divided_number_of_packets];
	if (!this->malicious) {
		std::cout << "bad allocation" << std::endl;
		exit(1);
	}
	int packet_counter = 0;
	for (int i = 0; i <= divided_number_of_packets; i++) {
		this->malicious[i] = new packet[DIVIDER];
		if (!this->malicious[i]) {
			std::cout << "bad allocation" << std::endl;
			exit(1);
		}
		for (int j = 0; j < DIVIDER; j++) {
			this->malicious[i][j] = packet();
			packet_counter++;
			if (packet_counter == number_of_packets) {
				break;
			}
		}
		if (packet_counter == number_of_packets) {
			break;
		}
	}
	std::cout << "Initialization Process Complete" << std::endl;
}

traffic_array::~traffic_array() {
	delete[] this->flow_array;
	delete[] this->users;
}

int traffic_array::initialization_ckeck() {
	if (!this->flow_array || !this->users) {
		std::cout << "problem with initialization" << std::endl;
		return 1;
	}
	return 0;
}

packet traffic_array::next_packet() {
	if (current_packet_number > (current_number_of_users * packets_pet_user) + flow_array[current_flow]) {
		return packet(true);
	}
	if (current_packet_number == 0) {
		current_number_of_users = (int)ceil((number_of_users * (double)(1 - user_deviation)) + rand() % (int)(number_of_users * (double)(user_deviation)+1));
	}
	int current_number_of_packets = (current_number_of_users * packets_pet_user) + flow_array[current_flow];
	int probability = (int)(pow(rand(), 2)) % ((current_number_of_users * packets_pet_user) + flow_array[current_flow]);
	packet p;
	if (probability >= flow_array[current_flow]) {
		p = users[rand() % current_number_of_users];
	}
	else {
		if (number_of_packets == 0) {
			std::cout << "not enough packets generated" << std::endl;
			exit(1);
		}
		int this_packet = rand() % number_of_packets;
		int inner_array = this_packet % DIVIDER;
		int outer_array = (int)(this_packet / DIVIDER);
		p = malicious[outer_array][inner_array];
		int k = (int)floor(number_of_packets / DIVIDER);
		int l = number_of_packets % DIVIDER - 1;
		malicious[outer_array][inner_array] = malicious[k][l];
		number_of_packets--;
	}
	current_packet_number++;
	if (current_packet_number >= (current_number_of_users * packets_pet_user) + flow_array[current_flow]) {
		p = packet(true);
		current_flow++;
		current_packet_number = 0;
	}
	return p;
}

int traffic_array::current_flow_size(int flow_number) {
	return flow_array[flow_number];
}

int traffic_array::return_flow_array_size() {
	return flow_array_size;
}