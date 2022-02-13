#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

#include <SDL_net.h>

#include "input.h"
#include "log.h"
#include "state.h"
#include "parse.h"

#include "listener.h"

#define PORT 6969
#define PACKET_SIZE 8192
#define INPUT_SEND_FREQ 60

struct ListenerState* listener_init(struct StateManager* state_mgr, struct InputManager* input_mgr, struct in_addr server_addr);
void listener_destroy(struct ListenerState* listener);
void* listener(void* args);
int randrange(int min, int max);

struct ListenerState {
	pthread_t thread_id;
	struct ListenerArgs {
		struct StateManager* state_mgr;
		struct InputManager* input_mgr;
		struct in_addr* server_addr;
	} args;
};

struct ListenerState* listener_init(struct StateManager* state_mgr, struct InputManager* input_mgr, struct in_addr server_addr)
{
	struct ListenerState* listener_state = NULL;
	struct in_addr* server_addr_mem;

	if (!(listener_state = malloc(sizeof(*listener_state))))
		goto listener_init_cleanup;

	if (!(server_addr_mem = malloc(sizeof(*server_addr_mem))))
		goto listener_init_cleanup;

	*server_addr_mem = server_addr;

	listener_state->args.state_mgr = state_mgr;
	listener_state->args.input_mgr = input_mgr;
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
	unsigned int machine_id = randrange(1, 8192);

	srand(time(NULL));
	print_info("Server IP address: %s", inet_ntoa(*args.server_addr));

	UDPsocket socket = NULL;
	IPaddress* address_self = NULL;
	IPaddress address_server = { 0 };
	UDPpacket* packet_sending = NULL;
	UDPpacket* packet_receiving = NULL;

	/* open UDP socket */
	if (!(socket = SDLNet_UDP_Open(0))) {
		print_err("Could not open sending socket");
		goto listener_cleanup;
	}

	/* add socket to socket set */
	SDLNet_SocketSet socket_set = SDLNet_AllocSocketSet(1);
	if (SDLNet_UDP_AddSocket(socket_set, socket) < 0) {
		print_err("Could not add socket to socket set");
		goto listener_cleanup;
	}
	
	/* allocate sending packet */
	if (!(packet_sending = SDLNet_AllocPacket(PACKET_SIZE))) {
		print_err("Could not allocate sending packet (%d bytes)", PACKET_SIZE);
		goto listener_cleanup;
	}

	/* allocate receiving packet */
	if (!(packet_receiving = SDLNet_AllocPacket(PACKET_SIZE))) {
		print_err("Could could not allocate receiving packet (%d bytes)", PACKET_SIZE);
		goto listener_cleanup;
	}

	SDLNet_ResolveHost(&address_server, inet_ntoa(*args.server_addr), PORT);

	/* sending initial protocol message */
	int data_len = snprintf((char* ) packet_sending->data, packet_sending->maxlen, "CONN(ID%d)", machine_id);
	if (data_len < 0 || data_len >= PACKET_SIZE)
		return NULL;
	packet_sending->len = data_len + 1;
	packet_sending->address = address_server;
	SDLNet_UDP_Send(socket, -1, packet_sending);

	/* parse initial message response (UGLY) */
	SDLNet_CheckSockets(socket_set, ~0);
	SDLNet_UDP_Recv(socket, packet_receiving);
	int player_number = parse_conn(args.state_mgr->back, (char*) packet_receiving->data, packet_receiving->len);

	if (player_number <= 0)
		goto listener_cleanup;

	args.state_mgr->back->player_number_self = player_number;

	while (true) {
		SDLNet_CheckSockets(socket_set, (int) ((1.0 / INPUT_SEND_FREQ) * 1000.0));

		if (SDLNet_SocketReady(socket)) {
			if (SDLNet_UDP_Recv(socket, packet_receiving)) {

				struct State* state = args.state_mgr->back;

				parse_msg(state, (char* ) packet_receiving->data, packet_receiving->len);

				/* swap states */
				pthread_mutex_lock(&args.state_mgr->mutex);
				state = args.state_mgr->back;
				args.state_mgr->back = args.state_mgr->front;
				args.state_mgr->front = state;
				pthread_mutex_unlock(&args.state_mgr->mutex);
			}
		}

		struct InputManager* input_mgr = args.input_mgr;
		pthread_mutex_lock(&input_mgr->mutex);
		bool i_l = input_mgr->left, i_r = input_mgr->right, i_j = input_mgr->jump, i_s = input_mgr->shoot;
		pthread_mutex_unlock(&input_mgr->mutex);

		int data_len = snprintf((char* ) packet_sending->data, packet_sending->maxlen, "INPT(ID%d_L%d_R%d_J%d_S%d)", machine_id, i_l, i_r, i_j, i_s);
		if (data_len < 0 || data_len >= PACKET_SIZE)
			return NULL;
		packet_sending->len = data_len + 1;
		packet_sending->address = address_server;
		SDLNet_UDP_Send(socket, -1, packet_sending);
	}

listener_cleanup:

	if (socket)
		SDLNet_UDP_Close(socket);

	return NULL;
}

int randrange(int min, int max)
{
	return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}
