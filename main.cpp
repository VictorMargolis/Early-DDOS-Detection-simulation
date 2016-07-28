#include "traffic_array.h"
#include "sampling.h"
#include <random>

#define SOURCE_FILE "flow.txt"
#define NUM_OF_USERS 50
#define PACKETS_PER_USER 100
#define WINDOW_SIZE 1 // V
#define WINDOW_TIME 1 // WINDOW_TIME is a double in (0,1] which determines the time of the sampling for each element in the window
#define SAMPLE_SIZE 200 // Z
#define SAMPLE_RATE 100 // SAMPLE_RATE k meaning sampeling 1 out of each k packets
#define USER_DEVIATOION 0 // USER_DEVIATOION d meaning every cycle arandom number of users will be generatad from NUM_OF_USERS to NUM_OF_USERS*(1-d)
#define SUB_SAMPLING false
#define SUB_SAMPLE_SIZE 100 // U

int main() {
	traffic_array traffic(SOURCE_FILE, NUM_OF_USERS, USER_DEVIATOION, PACKETS_PER_USER);
	if (traffic.initialization_ckeck() != 0) {
		return 1;
	}
	run_sampling(&traffic, WINDOW_SIZE, SAMPLE_SIZE, SAMPLE_RATE, WINDOW_TIME, NUM_OF_USERS*PACKETS_PER_USER, SUB_SAMPLING, SUB_SAMPLE_SIZE);
	return 0;
}
