#include <iostream>
#include <ctime>
#include <string>
#include <functional>
#include <memory>
#include <exception>
#include <boost/asio.hpp>

std::string make_daytime_string() {
	char string[] = "1298371298";
	auto start = std::chrono::system_clock::now();

	std::time_t end_time = std::chrono::system_clock::to_time_t(start);
	
	std::cout << "finished computation at " << string << std::endl;

	std::string sstring(string);
	sstring.back() = '\0';
	return sstring;
}

using boost::asio::ip::udp;

//class Server {
//private:
//	boost::asio::io_context& io;
//	udp::socket socket;
//	std::vector<udp::endpoint> clients;
//	boost::asio::steady_timer broadcastTimer;
//	udp::endpoint client;
//
//public:
//	Server(boost::asio::io_context& _io) : io(_io), socket(_io, udp::endpoint(udp::v4(), 13)), broadcastTimer(io, boost::asio::chrono::milliseconds(100)) {
//		//seekClient1();
//		seekClient();
//		broadcastTimer.async_wait(std::bind(&Server::broadcast, this, boost::asio::placeholders::error));
//	}
//	
//	//void seekClient1() {
//	//	std::array<char, 1> callOut = { {0} };
//	//	//socket.async_receive_from(boost::asio::buffer(callOut), client, std::bind(&Server::manageNewClient, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
//	//	socket.receive_from(boost::asio::buffer(callOut), client);
//	//	manageNewClient1();
//	//}
//
//	//void manageNewClient1() {
//	//	/*if (std::find(clients.begin(), clients.end(), client) == clients.end()) {
//	//		clients.push_back(client);
//	//	}*/
//	//	socket.send_to(boost::asio::buffer(make_daytime_string()), client);
//	//	seekClient1();
//	//}
//
//	void seekClient() {
//		std::array<char, 1> callOut = { {0} };
//		udp::endpoint* client1 = new udp::endpoint();
//		socket.async_receive_from(boost::asio::buffer(callOut), *client1, std::bind(&Server::manageNewClient,
//			this, client1, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
//	}
//
//	void manageNewClient(udp::endpoint* client1, boost::system::error_code /*e*/, size_t /*bytes_transfered*/) {
//		//socket.send_to(boost::asio::buffer(make_daytime_string()), client);
//		if (std::find(clients.begin(), clients.end(), *client1) == clients.end()) {
//			clients.push_back(*client1);
//		}
//		seekClient();
//	}
//
//	void broadcast(boost::system::error_code /*e*/) {
//		std::string string = make_daytime_string();
//		for (udp::endpoint& client1 : clients) {
//			std::cout << "sending to " << client1.address().to_string() << "\n";
//			socket.send_to(boost::asio::buffer(string), client1);
//			delete &client1;
//		}
//
//		clients.clear();
//
//		broadcastTimer.expires_at(broadcastTimer.expiry() + boost::asio::chrono::seconds(5));
//		broadcastTimer.async_wait(std::bind(&Server::broadcast, this, boost::asio::placeholders::error));
//	}
//};

class Server {
private:
	boost::asio::io_context& io;
	udp::socket socket;
public:
	Server(boost::asio::io_context& _io) :io(_io), socket(_io, udp::endpoint(udp::v4(), 13)) {
		seekClient();
	}

	void seekClient() {
		std::array<char, 1> callOut = { {0} };
		udp::endpoint client;
		std::cout << "ashd";
		socket.receive_from(boost::asio::buffer(callOut), client);
		std::cout << "ashd";
		broadcast(client);
	}

	void broadcast(udp::endpoint client) {
		socket.send_to(boost::asio::buffer(make_daytime_string()), client);
		seekClient();
	}
};

int main() {
	try {
		/*
		using boost::asio::ip::udp;

		boost::asio::io_context io;
		udp::socket socket(io, udp::endpoint(udp::v4(), 13));

		while (1) {
			std::array<char, 1> recv_buf;
			udp::endpoint remote_endpoint;
			socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);

			boost::system::error_code ignored_error;
			socket.send_to(boost::asio::buffer(make_daytime_string()), remote_endpoint, 0, ignored_error);
		}
		*/
		std::cout << "ashd";
		boost::asio::io_context io;
		Server server(io);
		io.run();
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return 1;
	}
	return 0;
}