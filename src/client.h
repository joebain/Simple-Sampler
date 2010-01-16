#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "server.h"

class Client
{
public:
	Client(Server* server);

	virtual void start();
	virtual void stop();
	virtual void update();

protected:
	Server* server;

private:
	
};

#endif // _CLIENT_H_
