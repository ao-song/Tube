#include "InetAddress.h"

using namespace Tube;

bool
InetAddress::init(
	const char* ipStr)
{
	skip_spaces(ipStr);

	const char* startOfAddress = ipStr;
}

bool
InetAddress::init(
    const struct sockaddr_storage* address,
	socklen_t                      addressLength,
	unsigned short&                port)
{
	
}