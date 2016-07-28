#pragma once
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

class packet {
private:
	unsigned int src_ip;
	unsigned int dst_ip;
	unsigned int src_port;
	unsigned int dst_port;
	unsigned int protocol;
	unsigned int hash;
	unsigned int return_random(int power);
	bool last_of_flow;
public:
	packet();
	packet(unsigned int src_ip, unsigned int dst_ip, unsigned int src_port, unsigned int dst_port, unsigned int protocol);
	packet(bool last);
	unsigned int get_hash();
	bool is_last_in_flow();
};
