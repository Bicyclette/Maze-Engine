#include "network_server.hpp"

NetworkServer::NetworkServer() :
	m_active(true),
	m_server(nullptr)
{
	if (enet_initialize() != 0)
	{
		throw std::runtime_error("An error occured while initializing ENet !");
	}
	atexit(enet_deinitialize);

	m_address.host = ENET_HOST_ANY;
	m_address.port = 7777;

	m_server = enet_host_create(&m_address, 2, 2, 0, 0);
	if (m_server == nullptr)
	{
		enet_deinitialize();
		throw std::runtime_error("Error while trying to create the network server !");
	}
}

NetworkServer::~NetworkServer()
{
	enet_host_destroy(m_server);
	for (auto& p : m_player)
	{
		enet_peer_disconnect(p->m_peer, 0);
	}
}

void NetworkServer::run()
{
	while (m_active)
	{
		while (enet_host_service(m_server, &m_event, 0) > 0)
		{
			if (m_event.type == ENET_EVENT_TYPE_CONNECT)
			{
				m_player.emplace_back(std::make_shared<Player>("", m_event.peer));
			}
			else if (m_event.type == ENET_EVENT_TYPE_RECEIVE)
			{
				std::string message(reinterpret_cast<char*>(m_event.packet->data));
				for (int i{ 0 }; i < m_player.size(); ++i)
				{
					auto& p{ m_player[i] };
					ENetAddress player_address = p->m_peer->address;
					ENetAddress peer_address = m_event.peer->address;
					if (player_address.host == peer_address.host && player_address.port == peer_address.port)
					{
						g_message_queue.emplace(p, message);
					}
				}
			}
			else if (m_event.type == ENET_EVENT_TYPE_DISCONNECT)
			{
				for (int i{ 0 }; i < m_player.size(); ++i)
				{
					auto& p{ m_player[i] };
					ENetAddress player_address = p->m_peer->address;
					ENetAddress peer_address = m_event.peer->address;
					if (player_address.host == peer_address.host && player_address.port == peer_address.port)
					{
						m_player.erase(m_player.begin() + i);
						break;
					}
				}
			}
		}
	}
}

void NetworkServer::shutdown()
{
	m_active = false;
}

void NetworkServer::send_data(ENetPeer* peer, std::string data)
{
	ENetPacket* packet = enet_packet_create(data.c_str(), data.size() + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet);
}