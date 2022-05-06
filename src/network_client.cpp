#include "network_client.hpp"

NetworkClient::NetworkClient() :
	m_client(nullptr),
	m_peer(nullptr)
{
	if (enet_initialize() != 0)
	{
		throw std::exception("An error occured while initializing ENet !");
	}
	atexit(enet_deinitialize);

	// create client
	m_client = enet_host_create(nullptr, 1, 2, 0, 0);
	if (m_client == nullptr)
	{
		enet_deinitialize();
		throw std::exception("Error while trying to create the network client !");
	}
}

NetworkClient::~NetworkClient()
{

}

bool NetworkClient::connect(std::string server_ip, int port)
{
	enet_address_set_host(&m_address, server_ip.c_str());
	m_address.port = port;

	m_peer = enet_host_connect(m_client, &m_address, 2, 0);
	if (m_peer == nullptr)
	{
		enet_host_destroy(m_client);
		enet_deinitialize();
		throw std::exception("Error occured while creating the server peer connection !");
	}

	// check if the server has contacted us back
	if (enet_host_service(m_client, &m_event, 5000) > 0 &&
		m_event.type == ENET_EVENT_TYPE_CONNECT)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool NetworkClient::disconnect()
{
	enet_peer_disconnect(m_peer, 0);
	while (enet_host_service(m_client, &m_event, 5000) > 0)
	{
		switch (m_event.type)
		{
		case ENET_EVENT_TYPE_RECEIVE:
			enet_packet_destroy(m_event.packet);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			return true;
		}
	}
	return false;
}


bool NetworkClient::has_received_data()
{
	if (enet_host_service(m_client, &m_event, 0) > 0 && m_event.type == ENET_EVENT_TYPE_RECEIVE)
	{
		return true;
	}
	return false;
}

void NetworkClient::print_data()
{
	std::string data = reinterpret_cast<char*>(m_event.packet->data);
	std::cout << data << std::endl;
}