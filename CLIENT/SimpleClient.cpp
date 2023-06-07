#include <SDL.h>
#include <SDL_net.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
using namespace std;


/*
	Socket : prise, permet de se connecter

*/

TCPsocket clientSocket{};
IPaddress ip;

string 	clientName = "ClientName";


int InitClient()
{
	//Init SDL
	if (SDLNet_Init() == -1) {
		cerr << "SDLNet_Init error: " << SDLNet_GetError() << endl;
		return 1;
	}

	//Set Ip to connect
	string ipToConnect = "localhost";	//Remove localhost if you want to connect to another

	if (ipToConnect != "localhost")
	{
		cout << "IP to connect: ";
		getline(std::cin, ipToConnect);
	}

	//Connect to Ip with port(4242)
	if (SDLNet_ResolveHost(&ip, ipToConnect.c_str(), 4242) == -1)
	{
		cerr << "Resolve Host error: " << SDLNet_GetError() << endl;
		SDLNet_Quit();
		return 1;
	}

	//Create socket for client
	clientSocket = SDLNet_TCP_Open(&ip);	//Ouvre soket client
	if (!clientSocket) {
		cerr << "TCP Open error: " << SDLNet_GetError() << endl;
		SDLNet_Quit();
		return 1;
	}

}

void SendMessage(string message)
{
	string finalMsg = clientName + " : " + message;
	int bytesSent = SDLNet_TCP_Send(clientSocket, finalMsg.c_str(), finalMsg.length() + 1);
	if (bytesSent < finalMsg.length() + 1) {
		cerr << "SDLNet TCP Send error: " << SDLNet_GetError() << endl;
	}
}

void ListenReceiveMessage()
{
	//---------Reçoit message
	while (true) {
		char buffer[1024];
		int bytesRead = SDLNet_TCP_Recv(clientSocket, buffer, sizeof(buffer));
		if (bytesRead <= 0)
		{
			cerr << "SDLNet TCP Recv error: " << SDLNet_GetError() << endl;

		}
		else {
			cout << buffer << endl;
		}
	}
}

void ConnectToServer()
{

	cout << "To Connect please enter your name: ";
	getline(std::cin, clientName);

	string finalMsg = clientName + " connected to server";
	int bytesSent = SDLNet_TCP_Send(clientSocket, finalMsg.c_str(), finalMsg.length() + 1);
	if (bytesSent < finalMsg.length() + 1) {
		cerr << "SDLNet TCP Send error: " << SDLNet_GetError() << endl;
	}
}


void WriteMessage()
{
	while (true) {
		string message;
		getline(std::cin, message);
		cout << "\b \b \b \b \b \b \b";

		SendMessage(message);
	}
}


void InitOnline()
{
	InitClient();
	//------------Send name of client
	ConnectToServer();


	vector <std::thread> threads;

	threads.emplace_back(	//On ajoute la fonction dans la liste de threads
		([=]() {		//On passe le tout par copie,
			ListenReceiveMessage();//
			})
	);
	threads.emplace_back(	//On ajoute la fonction dans la liste de threads
		([=]() {		//On passe le tout par copie,
			WriteMessage();//
			})
	);

	for (size_t i = 0; i < threads.size(); i++)//On rejoins tout les threads
	{
		threads[i].join();
	}
}


int main(int argc, char* argv[]) 
{
	InitOnline();
		
	SDLNet_TCP_Close(clientSocket);
	SDLNet_Quit();



	//cout << "\n" << "Thank you for using ChArtFX !\n";
	return 0;
}
