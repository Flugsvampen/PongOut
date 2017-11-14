#pragma comment(lib, "rpc.lib")

#include <rpc/server.h>

#include <iostream>
#include <string>


std::string foo() {
	std::cout << "foo was called!" << std::endl;
	return "You called foo!";
}

int main(int argc, char* argv[])
{
	std::cout << "This is the server!" << std::endl;
	rpc::server* server = new rpc::server(8081);

	server->bind("foo", &foo);

	server->run();

	return 0;
}