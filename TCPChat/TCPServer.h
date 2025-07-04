#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <ctime>
#include <chrono>
#include <thread>
#include <functional>
#include <memory>
#include <vector>

using namespace boost::asio::ip;

class Connection {
private:
	boost::asio::io_context& io;
	tcp::socket socket;

public:
	typedef std::shared_ptr<Connection> pointer;

	Connection(boost::asio::io_context& _io);
	~Connection();

	static pointer create(boost::asio::io_context& _io);

	tcp::socket& getSocket();
};

class TCPServer {
private:
	boost::asio::io_context& io;
	tcp::socket socket;
	tcp::acceptor acceptor;
	std::vector<Connection::pointer> clients;
	boost::asio::deadline_timer inactivityTimeoutTimer;
	boost::asio::steady_timer broadcastTimer;

	void seekClient();

	void manageClient(Connection::pointer newConnection, boost::system::error_code error);

	void broadcastString(boost::system::error_code err);

	void recieveString();

	void inactivityTimeout(boost::system::error_code err);

public:
	TCPServer(boost::asio::io_context& _io);
	~TCPServer();

	tcp::socket& getSocket();
};


