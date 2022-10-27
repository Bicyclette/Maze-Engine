#ifndef NETWORK_CLIENT_HPP
#define NETWORK_CLIENT_HPP

#include <iostream>
#include <string>
#include <exception>
#include <enet/enet.h>

class NetworkClient
{
public:
	NetworkClient();
	~NetworkClient();
	bool connect(std::string server_ip, int port);
	bool disconnect();
	bool has_received_data();
	void print_data();
	void send_data(std::string data);
	int service();

private:
	ENetHost* m_client;
	ENetAddress m_address; // IP & port of the server the client will connect to
	ENetPeer* m_peer; // the server the client is connected to
	ENetEvent m_event; // event received from the server
};

#endif