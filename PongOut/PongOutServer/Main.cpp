#if _DEBUG
#pragma comment(lib, "sfml-system-d.lib")
#pragma comment(lib, "sfml-network-d.lib")
#pragma comment(lib, "sfml-graphics-d.lib")
#else
#pragma comment(lib, "sfml-system.lib")
#pragma comment(lib, "sfml-network.lib")
#pragma comment(lib, "sfml-graphics.lib")
#endif

#include <iostream>
#include <string>
#include "server.h"


int main(int argc, char* argv[])
{
	std::cout << "This is the server!" << std::endl;

	Server server;
	server.Receive();

	while (true);

	return 0;
}

