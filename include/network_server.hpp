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
#include <random>
#include <thread>
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

inline std::queue<ClientMessage> g_message_queue;
inline std::mutex g_message_mutex;
void message_processing(int tid, bool& server_on); // user defined

class NetworkServer
{
public:

	NetworkServer();
	~NetworkServer();
	void run();
	void shutdown();

private:

	bool m_active;
	const unsigned int m_num_threads;
	std::vector<std::thread> m_thread_pool;
	ENetAddress m_address;
	ENetHost* m_server;
	ENetEvent m_event;
	std::vector<std::shared_ptr<Player>> m_player;
};

#endif