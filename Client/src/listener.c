#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

#include <SDL_net.h>

#include "state.h"

#include "listener.h"

#define PORT 6969
#define BUFFER_SIZE 8192
#define PACKET_SIZE 508

struct ListenerState* listener_init(struct StateManager* state_mgr, struct in_addr server_addr);
void listener_destroy(struct ListenerState* listener);
void* listener(void* args);
int randrange(int min, int max);

struct ListenerState {
	pthread_t thread_id;
	struct ListenerArgs {
		struct StateManager* state_mgr;
		struct in_addr* server_addr;
	} args;
};

struct ListenerState* listener_init(struct StateManager* state_mgr, struct in_addr server_addr)
{
	struct ListenerState* listener_state = NULL;
	struct in_addr* server_addr_mem;

	if (!(listener_state = malloc(sizeof(*listener_state))))
		goto listener_init_cleanup;

	if (!(server_addr_mem = malloc(sizeof(*server_addr_mem))))
		goto listener_init_cleanup;

	*server_addr_mem = server_addr;

	listener_state->args.state_mgr = state_mgr;
	listener_state->args.server_addr = server_addr_mem;

	if (pthread_create(&listener_state->thread_id, NULL, &listener, &listener_state->args) != 0)
		goto listener_init_cleanup;

	return listener_state;

listener_init_cleanup:
	if (listener_state)
		free(listener_state);

	return NULL;
}

void listener_destroy(struct ListenerState* listener)
{
	// TODO: need a way to message listener that the main program is killed
	pthread_kill(listener->thread_id, SIGKILL);
	pthread_join(listener->thread_id, NULL);

	free(listener->args.server_addr);
	free(listener);
}

void* listener(void* listener_args)
{
	struct ListenerArgs args = *(struct ListenerArgs*) listener_args;
	struct State* state;
	unsigned int machine_id = randrange(1, 8192);

	srand(time(NULL));
	fprintf(stderr, "Server IP address: %s\n", inet_ntoa(*args.server_addr));

	UDPsocket socket = NULL;
	IPaddress* address_self = NULL;
	IPaddress address_server = { 0 };
	UDPpacket* packet_sending = NULL;
	UDPpacket* packet_receiving = NULL;

	/* open UDP socket */
	if (!(socket = SDLNet_UDP_Open(0))) {
		fprintf(stderr, "Error: could not open sending socket\n");
		goto listener_cleanup;
	}

	/* add socket to socket set */
	SDLNet_SocketSet socket_set = SDLNet_AllocSocketSet(1);
	if (SDLNet_UDP_AddSocket(socket_set, socket) < 0) {
		fprintf(stderr, "Error: could not add socket to socket set\n");
		goto listener_cleanup;
	}
	
	/* allocate sending packet */
	if (!(packet_sending = SDLNet_AllocPacket(PACKET_SIZE))) {
		fprintf(stderr, "Error: could not allocate sending packet (%d bytes)\n", PACKET_SIZE);
		goto listener_cleanup;
	}

	/* allocate receiving packet */
	if (!(packet_receiving = SDLNet_AllocPacket(PACKET_SIZE))) {
		fprintf(stderr, "Error: could not allocate receiving packet (%d bytes)\n", PACKET_SIZE);
		goto listener_cleanup;
	}

	SDLNet_ResolveHost(&address_server, inet_ntoa(*args.server_addr), PORT);

	/* sending initial protocol message */
	int data_len = snprintf((char* ) packet_sending->data, packet_sending->maxlen, "CONN[ID%d]", machine_id);
	if (data_len < 0 || data_len >= PACKET_SIZE)
		return NULL;
	packet_sending->len = data_len + 1;
	packet_sending->address = address_server;
	SDLNet_UDP_Send(socket, -1, packet_sending);

	/* parse initial message response */
	SDLNet_CheckSockets(socket_set, ~0);
	SDLNet_UDP_Recv(socket, packet_receiving);
	printf("got: %s\n", packet_receiving->data);

	printf("entering loop...\n");
	while (true) {
		SDLNet_CheckSockets(socket_set, ~0);
		if (SDLNet_SocketReady(socket)) {
			if (SDLNet_UDP_Recv(socket, packet_receiving)) {
				printf("received: %s\n", packet_receiving->data);
			}
		}
	}

listener_cleanup:

	if (socket)
		SDLNet_UDP_Close(socket);

	return NULL;

// 	IPaddress server_addr = { args.server_addr->s_addr, PORT };
// 	socket_channel = SDLNet_UDP_Bind(socket, -1, &server_addr);

// 	UDPpacket* packet;
// 	packet = SDLNet_AllocPacket(1024);
// 	packet->len = snprintf((char* ) packet->data, packet->maxlen, "CONN[ID%d]", machine_id) + 1;
// 	packet->channel = socket_channel;
// 	packet->address = server_addr;

// 	fprintf(stderr, "UDPSend: %d\n", SDLNet_UDP_Send(socket, socket_channel, packet));

// 	SDLNet_FreePacket(packet);

// listener_cleanup:
// 	if (socket_channel != -1)
// 		SDLNet_UDP_Unbind(socket, socket_channel);

// 	if (socket)
// 		SDLNet_UDP_Close(socket);

// 	return NULL;

	// /* establish connection */
	// int sockfd;
	// char buffer[BUFFER_SIZE];
	// struct sockaddr_in server_addr;

	// /* create socket file descriptor */
	// if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
	// 	printf("socket creation failed");
	// 	exit(EXIT_FAILURE);
	// }

	// memset(&server_addr, 0, sizeof(server_addr));
	// server_addr.sin_family = AF_INET;
	// server_addr.sin_port = htons(PORT);
	// server_addr.sin_addr.s_addr = args.server_addr->s_addr;
	
	// int conn_length = snprintf(buffer, BUFFER_SIZE, "CONN[ID%d]", machine_id);
	// if (conn_length < 0 || conn_length >= BUFFER_SIZE)
	// 	return NULL;

	// sendto(sockfd, buffer, conn_length, MSG_CONFIRM, (const struct sockaddr *) &server_addr, sizeof(server_addr));

	// int receive_count;
	// uint32_t len = sizeof(server_addr);
	// receive_count = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *) &server_addr, &len);
	// buffer[receive_count] = '\0';
	// printf("Response: %s\n", buffer);

	// close(sockfd);

	/////// plan:
	// init SDL_net
	// create connection
	// establish protocol connection
	// get response / player number
	// enter loop listening for state, after parsing send any inputs for state

	// while (true) {
	// 	usleep(16 * 1000);

	// 	/* update back state (TODO: get from server) */

	// 	state = args.state_mgr->back;

	// 	state->pos_x = state->pos_x + 0.25;
	// 	if (state->pos_x > 20.0)
	// 		state->pos_x = 0 - 1 + (20.0 - state->pos_x);

	// 	/* swap states */

	// 	pthread_mutex_lock(&args.state_mgr->mutex);

	// 	state = args.state_mgr->back;
	// 	args.state_mgr->back = args.state_mgr->front;
	// 	args.state_mgr->front = state;

	// 	pthread_mutex_unlock(&args.state_mgr->mutex);
	// }
}

int randrange(int min, int max)
{
	return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}
