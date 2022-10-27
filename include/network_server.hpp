#ifndef NETWORK_SERVER_HPP
#define NETWORK_SERVER_HPP

#include <iostream>
#include <memory>
#include <utility>
#include <glm/glm.hpp>
#include <exception>
#include <enet/enet.h>
#include <string>
#include <vector>
#include <array>
#include <random>
#include <thread>
#include <sstream>
#include <mutex>
#include <queue>

struct Player
{
	Player() :
		m_name(""),
		m_peer(nullptr)
	{}

	Player(std::string name, ENetPeer* peer) :
		m_name(name),
		m_peer(peer)
	{}

	std::string m_name;
	ENetPeer* m_peer;
};

struct ClientMessage
{
	ClientMessage() {}
	ClientMessage(std::shared_ptr<Player> player, std::string& message)
	{
		m_player = player;
		m_message = message;
	}

	std::shared_ptr<Player> m_player;
	std::string m_message;
};

class NetworkServer
{
public:

	NetworkServer();
	~NetworkServer();
	void run();
	void shutdown();
	void send_data(ENetPeer* peer, std::string data);

private:

	bool m_active;
	ENetAddress m_address;
	ENetHost* m_server;
	ENetEvent m_event;
	std::vector<std::shared_ptr<Player>> m_player;
};

inline std::queue<ClientMessage> g_message_queue;
inline std::mutex g_message_mutex;
inline std::mutex g_server_mutex;
void message_processing(int tid, bool& server_on, NetworkServer& server); // user defined

#endif