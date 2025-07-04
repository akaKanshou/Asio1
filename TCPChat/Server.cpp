#include "TCPServer.h"
#include <exception>

int main() {
	try {
		boost::asio::io_context io;
		TCPServer Server(io);
		io.run();
	}
	catch (std::exception& e) {
		std::cout << "ERROR: " << e.what() << "\n";
		return 1;
	}
	return 0;
}