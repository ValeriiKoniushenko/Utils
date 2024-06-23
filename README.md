# FunSocket
I have wrote this library for simple & fun using of the sockets. It just wrapper around ```WinSock```.

## TCP Client\Server

### Client

```c++
#include "ClientSocket.h"
#include "Wsa.h"

#include <iostream>

using namespace std;

int main()
{
	try
	{
		Wsa::instance().initialize(1, 1);

		TCPClientSocket client;
		client.open(AddressFamily::Inet);
		client.connectTo({"127.0.0.1", 8088});

		cout << "Response: " << client.receiveAsString(6) << endl;
		client.send("World");

		system("pause");
	}
	catch (std::runtime_error& error)
	{
		std::cout << "Error: " << error.what() << std::endl;
	}

	return 0;
}
```

### Server

```c++
#include "ClientSocket.h"
#include "ServerSocket.h"
#include "Wsa.h"

#include <iostream>

using namespace std;

int main()
{
	try
	{
		Wsa::instance().initialize(1, 1);

		TCPServerSocket server;
		server.open(AddressFamily::Inet);
		server.bind(SocketAddress("127.0.0.1", 8088));
		server.listen();
		auto client = server.accept();

		client.send("Hello");
		cout << client.receiveAsString(6) << endl;

		system("pause");
	}
	catch (std::runtime_error& error)
	{
		std::cout << "Error: " << error.what() << std::endl;
	}

	return 0;
}
```

## UPD Client\Server

### Client

```c++
#include "ClientSocket.h"
#include "Wsa.h"

#include <iostream>

using namespace std;

int main()
{
	try
	{
		Wsa::instance().initialize(1, 1);
		UDPClientSocket client(AddressFamily::Inet);

		client.sendTo("Hello", {"127.0.0.1", 8088});
		cout << client.getOwnAddress().getAddress() << ":" << client.getOwnAddress().getPort() << endl;

		auto result = client.receive(5);

		string str(result.data.begin(), result.data.end());
		cout << str << endl;
		cout << result.server.getAddress() << ":" << result.server.getPort() << endl;

		system("pause");
	}
	catch (std::runtime_error& error)
	{
		std::cout << "Error: " << error.what() << std::endl;
	}

	return 0;
}
```

### Server

```c++
#include "ClientSocket.h"
#include "ServerSocket.h"
#include "Wsa.h"

#include <iostream>

using namespace std;

int main()
{
	try
	{
		Wsa::instance().initialize(1, 1);
		UDPServerSocket server;
		server.open(AddressFamily::Inet);
		server.bind({"127.0.0.1", 8088});

		auto result = server.receive(6);
		std::string str(result.data.begin(), result.data.end());
		cout << str << "\nFrom: " << result.client.getAddress() << ":" << result.client.getPort() << endl;

		server.sendTo("World", result.client);

		system("pause");
	}
	catch (std::runtime_error& error)
	{
		std::cout << "Error: " << error.what() << std::endl;
	}

	return 0;
}
```

---

## 📞 Feedback & Contacts

You can send me e-mail: Valerii.Koniushenko@gmail.com or try to find me in telegram: @markmoran24