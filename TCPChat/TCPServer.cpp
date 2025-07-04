#include "TCPServer.h"

TCPServer::TCPServer(boost::asio::io_context& _io) : 
	io(_io), socket(_io), acceptor(_io, tcp::endpoint(tcp::v4(), 7777)), inactivityTimeoutTimer(_io), broadcastTimer(_io) /**/ {

	inactivityTimeoutTimer.expires_from_now(boost::posix_time::seconds(20));
	inactivityTimeoutTimer.async_wait(std::bind(&TCPServer::inactivityTimeout, this, boost::asio::placeholders::error));
	/*
	*/

	seekClient();
	/*TCPServer::safeTimer timer = TCPServer::getTimer(io);
	timer->expires_after(std::chrono::milliseconds(100));
	timer->async_wait(std::bind(&TCPServer::broadcastString, this, timer, boost::asio::placeholders::error));*/

	broadcastTimer.expires_at(std::chrono::steady_clock::now() + std::chrono::seconds(3));
	broadcastTimer.async_wait(std::bind(&TCPServer::broadcastString, this, boost::asio::placeholders::error));
}

TCPServer::~TCPServer() {
	std::cout << "LOG: Server_Shutdown\n";
}

void TCPServer::inactivityTimeout(boost::system::error_code err) {
	if (!err) {
		acceptor.cancel();
		broadcastTimer.cancel();
		std::cout << "LOG: Server_Shutdown[Inactivity]\n";	
	}
	else if (err == boost::asio::error::operation_aborted) {
		std::cout << "LOG: InactivityTimer_Restarted\n";
		inactivityTimeoutTimer.expires_from_now(boost::posix_time::seconds(20));
		inactivityTimeoutTimer.async_wait(std::bind(&TCPServer::inactivityTimeout, this, boost::asio::placeholders::error));
	}
	else {
		std::cout << "ERROR: " << err.what() << "\n";
	}
}

void TCPServer::seekClient() {
	Connection::pointer newConnection = Connection::create(io);
	acceptor.async_accept(newConnection->getSocket(), 
		std::bind(&TCPServer::manageClient, this, newConnection, boost::asio::placeholders::error));
}

void TCPServer::manageClient(Connection::pointer newConnection, boost::system::error_code err) {
	if (err) {
		std::cout << "ERROR: " << err.what() << "\n";
		return;
	}

	inactivityTimeoutTimer.expires_from_now(boost::posix_time::seconds(20));

	std::cout << "LOG: Client_Recieved\n";
	clients.push_back(newConnection);

	seekClient();
} 

void TCPServer::broadcastString(boost::system::error_code err) {
	if (err) {
		std::cout << "ERROR: " << err.what() << "\n";
		return;
	}

	std::cout << "LOG: Broadcast_Called\n";

	std::time_t timeT= std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::string time(std::ctime(&timeT));

	auto it = clients.begin();
	std::vector<decltype(it)> stack;

	for (auto& client : clients) {
		inactivityTimeoutTimer.expires_from_now(boost::posix_time::seconds(20));

		boost::system::error_code err;
		boost::asio::write(client->getSocket(), boost::asio::buffer(time), err);

		if (err == boost::asio::error::connection_reset || err == boost::asio::error::broken_pipe) {
			std::cout << "LOG: Connection_Closed_Forcibly\n";
			stack.push_back(it);
		}

		else if (err) {
			std::cout << "ERROR: " << err.what() << "\n";
		}

		it++;
	}

	while (!stack.empty()) {
		clients.erase(stack.back());
		stack.pop_back();
	}

	/*TCPServer::safeTimer timer = TCPServer::getTimer(io);
	timer->expires_after(std::chrono::seconds(5));
	timer->async_wait(std::bind(&TCPServer::broadcastString, this, timer, boost::asio::placeholders::error));*/
	broadcastTimer.expires_at(std::chrono::steady_clock::now() + std::chrono::seconds(3));
	broadcastTimer.async_wait(std::bind(&TCPServer::broadcastString, this, boost::asio::placeholders::error));
}

void TCPServer::recieveString() {
	//TO BE IMPLEMENTED
}

TCPServer::safeTimer TCPServer::getTimer(boost::asio::io_context& io) {
	return TCPServer::safeTimer(new boost::asio::steady_timer(io));
}




Connection::Connection(boost::asio::io_context& _io) : io(_io), socket(_io) {}

Connection::~Connection() {
	std::cout << "LOG: Connection_Closed.\n";
}


Connection::pointer Connection::create(boost::asio::io_context& _io) {
	return Connection::pointer(new Connection(_io));
}

tcp::socket& Connection::getSocket() {
	return this->socket;
}