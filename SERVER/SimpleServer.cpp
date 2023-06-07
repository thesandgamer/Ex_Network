#include <SDL.h>
#include <SDL_net.h>
#include <iostream>

#include <vector>

using namespace std;

int main(int argc, char* argv[])
{

	IPaddress ip;
	if (SDLNet_Init() == -1)
	{
		cerr << "SDLNet_Init error: " << SDLNet_GetError() << endl;
		return 1;
	}


	if (SDLNet_ResolveHost(&ip, nullptr, 4242) == -1)
	{
		cerr << "Resolve Host error: " << SDLNet_GetError() << endl;
		SDLNet_Quit();
		return 1;
	}

	vector <TCPsocket> serverSockets;
	TCPsocket serverSocket = SDLNet_TCP_Open(&ip);
	serverSockets.push_back(serverSocket);
	if (!serverSocket)
	{
		cerr << "TCP Open error: " << SDLNet_GetError() << endl;
		SDLNet_Quit();
		return 1;
	}
	vector<TCPsocket> clientSockets;
	SDLNet_SocketSet clientSet = SDLNet_AllocSocketSet(10);


	while (true)
	{
		TCPsocket tempSocket = SDLNet_TCP_Accept(serverSocket);
		if (tempSocket != NULL)
		{
			string answer = "Connected to server \n";
			int bytesSent = SDLNet_TCP_Send(tempSocket, answer.c_str(), answer.length() + 1);
			if (bytesSent < answer.length() + 1) {
				cerr << "SDLNet TCP Send error: " << SDLNet_GetError() << endl;
			}
			clientSockets.push_back(tempSocket);
			SDLNet_TCP_AddSocket(clientSet, tempSocket);




			TCPsocket serverSocket = SDLNet_TCP_Open(&ip);
			serverSockets.push_back(serverSocket);

		}


		if (SDLNet_CheckSockets(clientSet, 0) > 0)//Check si il y a une activité
		{

			for (auto client : clientSockets)
			{
				if (client && SDLNet_SocketReady(client) > 0)
				{
					//cout << SDLNet_SocketReady(client) << endl;
					char nameBuffer[1024];
					int bytesRead = SDLNet_TCP_Recv(client, nameBuffer, sizeof(nameBuffer));

					if (bytesRead > 0)	//Si y'a un message
					{
						cout << nameBuffer << endl;

						
						for (auto clientToSend : clientSockets)
						{
							//if (clientToSend == client) continue;
							//----------Send a message to the client
							string answer = nameBuffer;
							int bytesSent = SDLNet_TCP_Send(clientToSend, answer.c_str(), answer.length() + 1);
							if (bytesSent < answer.length() + 1) {
								cerr << "SDLNet TCP Send error: " << SDLNet_GetError() << endl;
							}
						}

					}
				}
			}

		}


	}


	cout << "\n" << "Thank you for using ChArtFX !\n";
	return 0;
}



/*


if (clientSocket) {

	char nameBuffer[1024];
	int bytesRead = SDLNet_TCP_Recv(clientSocket, nameBuffer, sizeof(nameBuffer));
	if (bytesRead > 0) {
		cout << nameBuffer << " reached the server!" << endl;

	}



	char buffer[1024];
	 bytesRead = SDLNet_TCP_Recv(clientSocket, buffer, sizeof(buffer));
	if (bytesRead > 0) {
		cout << "Incoming message: " << buffer << endl;
		string answer = "Message received 5/5, client!";
		int bytesSent = SDLNet_TCP_Send(clientSocket, answer.c_str(), answer.length() + 1);
		if (bytesSent < answer.length() + 1) {
			cerr << "SDLNet TCP Send error: " << SDLNet_GetError() << endl;
			//break;
		}

	}
}
*/