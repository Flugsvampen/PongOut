#if _DEBUG
#pragma comment(lib, "sfml-system-d.lib")
#pragma comment(lib, "sfml-network-d.lib")
#else
#pragma comment(lib, "sfml-system.lib")
#pragma comment(lib, "sfml-network.lib")
#endif

#include <iostream>
#include <string>
#include "server.h"

sf::Packet foo() {
	std::cout << "foo was called!" << std::endl;
	return sf::Packet();
}

int main(int argc, char* argv[])
{
	std::cout << "This is the server!" << std::endl;

	Server server;
	server.Bind("foo", foo);
	server.Bind("foo", foo);

	while (true);

	return 0;
}