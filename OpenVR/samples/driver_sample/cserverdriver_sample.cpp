#include "cserverdriver_sample.h"
#include <WinSock2.h>
#include <windows.h>
#include <WS2tcpip.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <thread>
#pragma comment(lib, "ws2_32.lib")

#define UDP_RECV_PORT 49152
#define UDP_RECV_BYTES 1024

bool isListening = true;

using namespace vr;
void ReceiveThread(CSampleControllerDriver* m_pController, CSampleControllerDriver* m_pController2) {
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in serverHint;
	serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(UDP_RECV_PORT);
	char buffer[UDP_RECV_BYTES];

	if (bind(s, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR) {
		printf("Could not bind socket, error %d", WSAGetLastError());
		return;
	}

	sockaddr_in client;
	int clientLength = sizeof(client);
	ZeroMemory(&client, clientLength);

	// Main loop of socket
	while (isListening) {
		ZeroMemory(buffer, UDP_RECV_BYTES);

		int bytesIn = recvfrom(s, buffer, UDP_RECV_BYTES, 0, (sockaddr*)&client, &clientLength);
		if (bytesIn == SOCKET_ERROR) {
			printf("Error in recvfrom, error %d", WSAGetLastError());
			continue;
		}

		char clientIp[256];
		ZeroMemory(clientIp, 256);

		inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

		printf("Message received from %s: %s", clientIp, buffer);
		vr::VRDriverLog()->Log(buffer);

		// message format: "controllerIndex|PosX|PosY|PosZ|RotX|RotY|RotZ"
		//without strtok
		
		char controllerIndex[1024];
		double posX, posY, posZ;
		double rotX, rotY, rotZ;

		#pragma warning(disable : 4996)
		sscanf(buffer, "%s|%lf|%lf|%lf|%lf|%lf|%lf", controllerIndex, &posX, &posY, &posZ, &rotX, &rotY, &rotZ);

		if (strcmp(controllerIndex, "1") == 0) {
			m_pController->UpdatePosition(1,posX, posY, posZ);
			m_pController->UpdateRotation(1,rotX, rotY, rotZ);
		}
		else if (strcmp(controllerIndex, "2") == 0) {
			m_pController2->UpdatePosition(2,posX, posY, posZ);
			m_pController2->UpdateRotation(2,rotX, rotY, rotZ);
		}


	}
	
	closesocket(s);
}

EVRInitError CServerDriver_Sample::Init(vr::IVRDriverContext *pDriverContext)
{
	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0) {
		printf("Could not intialize WSA, error %d", err);
		return VRInitError_Driver_Failed;
	}
	printf("Initialized WSA");
	

	VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);

	m_pController = new CSampleControllerDriver();
	m_pController->SetControllerIndex(1);
	vr::VRServerDriverHost()->TrackedDeviceAdded(m_pController->GetSerialNumber().c_str(), vr::TrackedDeviceClass_Controller, m_pController);

	m_pController2 = new CSampleControllerDriver();
	m_pController2->SetControllerIndex(2);
	vr::VRServerDriverHost()->TrackedDeviceAdded(m_pController2->GetSerialNumber().c_str(), vr::TrackedDeviceClass_Controller, m_pController2);

	std::thread UDPTask(ReceiveThread, m_pController, m_pController2);
	UDPTask.detach();

	return VRInitError_None;
}

void CServerDriver_Sample::Cleanup()
{
	delete m_pController;
	m_pController = NULL;
	delete m_pController2;
	m_pController2 = NULL;

	isListening = false;

	WSACleanup();
}

void CServerDriver_Sample::RunFrame()
{
	if (m_pController) {
		m_pController->RunFrame();
	}
	if (m_pController2) {
		m_pController2->RunFrame();
	}

#if 0
	vr::VREvent_t vrEvent;
	while ( vr::VRServerDriverHost()->PollNextEvent( &vrEvent, sizeof( vrEvent ) ) )
	{
		if ( m_pController )
		{
			m_pController->ProcessEvent(vrEvent);
		}
		if ( m_pController2)
		{
			m_pController2->ProcessEvent(vrEvent);
		}
	}
#endif
}
