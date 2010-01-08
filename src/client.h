#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "server.h"

class Client
{
public:
	Client(Server* server);

	void start();
	void stop();
	void update();
protected:

private:
	Server* server;
};

#endif // _CLIENT_H_
