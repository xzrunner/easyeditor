#include "Socket.h"

#include <wx/socket.h>

namespace ee
{

SINGLETON_DEFINITION(Socket)

Socket::Socket()
	: m_connected(false)
{
	// Create the socket
	m_sock = new wxSocketClient();

	// Setup the event handler and subscribe to most events
	//	m_sock->SetEventHandler(*this, SOCKET_ID);
	m_sock->SetNotify(wxSOCKET_CONNECTION_FLAG |
		              wxSOCKET_INPUT_FLAG |
					  wxSOCKET_LOST_FLAG);
	m_sock->Notify(true);
}

Socket::~Socket()
{
	// No delayed deletion here, as the frame is dying anyway
	delete m_sock;
}

void Socket::ConnDefault()
{
	wxIPV4address addr;
	addr.Hostname("localhost");
	addr.Service(3141);
	m_sock->Connect(addr, false);

	m_connected = true;
}

void Socket::ConnFromUser(const std::string& hostname)
{
	if (hostname.empty()) {
		return;
	}

	wxIPV4address addr;
	addr.Hostname(hostname);
	addr.Service(3141);
	m_sock->Connect(addr, false);

	m_connected = true;
}

void Socket::Send(const std::string& msg)
{
	m_sock->Write(msg.c_str(), msg.size() + 1);
}

}