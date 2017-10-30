#ifndef _EASYEDITOR_SOCKET_H_
#define _EASYEDITOR_SOCKET_H_

#include <cu/cu_macro.h>

#include <string>

class wxSocketClient;

namespace ee
{

class Socket
{
public:
	void ConnDefault();
	void ConnFromUser(const std::string& hostname);

	void Send(const std::string& msg);

	bool IsConnected() const { return m_connected; }

private:
	wxSocketClient* m_sock;

	bool m_connected;

	CU_SINGLETON_DECLARATION(Socket)

}; // Socket

}

#endif // _EASYEDITOR_SOCKET_H_