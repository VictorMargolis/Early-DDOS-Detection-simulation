#include "packet.h"

packet::packet() {
	this->src_ip = return_random(4);
	this->dst_ip = return_random(4);
	this->src_port = return_random(2);
	this->dst_port = return_random(2);
	this->protocol = return_random(0);
	this->hash = 0;
	this->last_of_flow = false;
}

packet::packet(unsigned int src_ip, unsigned int dst_ip, unsigned int src_port, unsigned int dst_port, unsigned int protocol) {
	this->src_ip = src_ip;
	this->dst_ip = dst_ip;
	this->src_port = src_port;
	this->dst_port = dst_port;
	this->protocol = protocol;
	this->hash = (src_ip * 59) ^ dst_ip ^ (src_port << 16) ^ dst_port ^ protocol;
	this->last_of_flow = false;
}

packet::packet(bool last) {
	this->src_ip = 0;
	this->dst_ip = 0;
	this->src_port = 0;
	this->dst_port = 0;
	this->protocol = 0;
	this->hash = 0;
	this->last_of_flow = last;
}

size_t packet::return_random(int power) {
	if (power == 0) {
		if (rand() % 2 == 0) {
			return 6;
		}
		else return 17;
	}
	unsigned int max = (unsigned int)pow(256, power)-1;
	unsigned int random = (unsigned int)(pow(rand(), 3)) % max;
	return random;
}

size_t packet::get_hash() {
	if (hash == 0) {
		hash = (src_ip * 59) ^ dst_ip ^ (src_port << 16) ^ dst_port ^ protocol;
	}
	return hash;
}

bool packet::is_last_in_flow() {
	return last_of_flow;
}