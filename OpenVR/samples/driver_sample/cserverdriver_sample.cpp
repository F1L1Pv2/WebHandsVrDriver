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
void ReceiveThread(CSampleControllerDriver *m_pController, CSampleControllerDriver *m_pController2)
{
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in serverHint;
	serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(UDP_RECV_PORT);
	char buffer[UDP_RECV_BYTES];

	if (bind(s, (sockaddr *)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
	{
		printf("Could not bind socket, error %d", WSAGetLastError());
		return;
	}

	sockaddr_in client;
	int clientLength = sizeof(client);
	ZeroMemory(&client, clientLength);

	// Main loop of socket
	while (isListening)
	{
		ZeroMemory(buffer, UDP_RECV_BYTES);

		int bytesIn = recvfrom(s, buffer, UDP_RECV_BYTES, 0, (sockaddr *)&client, &clientLength);
		if (bytesIn == SOCKET_ERROR)
		{
			printf("Error in recvfrom, error %d", WSAGetLastError());
			continue;
		}

		char clientIp[256];
		ZeroMemory(clientIp, 256);

		inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

		printf("Message received from %s: %s", clientIp, buffer);
		vr::VRDriverLog()->Log(buffer);

		// message format: "PosX1|PosY1|PosZ1|RotX1|RotY1|RotZ1|PosX2|PosY2|PosZ2|RotX2|RotY2|RotZ2|A|B|X|Y|Joy1X|Joy1Y|Joy2X|Joy2Y|Trig1|Trig2|Grip1|Grip2|AppMenu1|AppMenu2|Joy1Click|Joy2Click|System1|System2"
		// without strtok

		double posX1, posY1, posZ1;
		double rotW1, rotX1, rotY1, rotZ1;
		double posX2, posY2, posZ2;
		double rotW2, rotX2, rotY2, rotZ2;

		bool btnA1, btnB1, btnA2, btnB2, joy1Click, joy2Click;
		float trig1, trig2, grip1, grip2, joy1X, joy1Y, joy2X, joy2Y;

// split string by "|"
#pragma warning disable C4996
		char *pch = strtok(buffer, "|");
		posX1 = atof(pch);
		pch = strtok(NULL, "|");
		posY1 = atof(pch);
		pch = strtok(NULL, "|");
		posZ1 = atof(pch);
		pch = strtok(NULL, "|");

		rotW1 = atof(pch);
		pch = strtok(NULL, "|");
		rotX1 = atof(pch);
		pch = strtok(NULL, "|");
		rotY1 = atof(pch);
		pch = strtok(NULL, "|");
		rotZ1 = atof(pch);
		pch = strtok(NULL, "|");

		posX2 = atof(pch);
		pch = strtok(NULL, "|");
		posY2 = atof(pch);
		pch = strtok(NULL, "|");
		posZ2 = atof(pch);
		pch = strtok(NULL, "|");

		rotW2 = atof(pch);
		pch = strtok(NULL, "|");
		rotX2 = atof(pch);
		pch = strtok(NULL, "|");
		rotY2 = atof(pch);
		pch = strtok(NULL, "|");
		rotZ2 = atof(pch);

		pch = strtok(NULL, "|");
		btnA1 = atoi(pch);
		pch = strtok(NULL, "|");
		btnB1 = atoi(pch);
		pch = strtok(NULL, "|");
		btnA2 = atoi(pch);
		pch = strtok(NULL, "|");
		btnB2 = atoi(pch);
		pch = strtok(NULL, "|");
		joy1X = atof(pch);
		pch = strtok(NULL, "|");
		joy1Y = atof(pch);
		pch = strtok(NULL, "|");
		joy2X = atof(pch);
		pch = strtok(NULL, "|");
		joy2Y = atof(pch);
		pch = strtok(NULL, "|");
		trig1 = atof(pch);
		pch = strtok(NULL, "|");
		trig2 = atof(pch);
		pch = strtok(NULL, "|");
		grip1 = atof(pch);
		pch = strtok(NULL, "|");
		grip2 = atof(pch);
		pch = strtok(NULL, "|");
		joy1Click = atoi(pch);
		pch = strtok(NULL, "|");
		joy2Click = atoi(pch);

		m_pController->UpdatePosition(1, posX1, posY1, posZ1);
		m_pController->UpdateRotation(1, rotW1, rotX1, rotY1, rotZ1);
		m_pController2->UpdatePosition(2, posX2, posY2, posZ2);
		m_pController2->UpdateRotation(2, rotW2, rotX2, rotY2, rotZ2);

		vr::VRDriverInput()->UpdateBooleanComponent(m_pController->HButtons[0], btnB1, 0);
		vr::VRDriverInput()->UpdateBooleanComponent(m_pController2->HButtons[0], btnA1, 0);
		vr::VRDriverInput()->UpdateBooleanComponent(m_pController->HButtons[1], btnB2, 0);
		vr::VRDriverInput()->UpdateBooleanComponent(m_pController2->HButtons[1], btnA2, 0);
		vr::VRDriverInput()->UpdateBooleanComponent(m_pController->HButtons[2], joy1Click, 0);
		vr::VRDriverInput()->UpdateBooleanComponent(m_pController2->HButtons[2], joy2Click, 0);

		vr::VRDriverInput()->UpdateScalarComponent(m_pController->HAnalog[0], joy1X, 0);
		vr::VRDriverInput()->UpdateScalarComponent(m_pController->HAnalog[1], joy1Y, 0);
		vr::VRDriverInput()->UpdateScalarComponent(m_pController2->HAnalog[0], joy2X, 0);
		vr::VRDriverInput()->UpdateScalarComponent(m_pController2->HAnalog[1], joy2Y, 0);
		vr::VRDriverInput()->UpdateScalarComponent(m_pController->HAnalog[2], trig1, 0);
		vr::VRDriverInput()->UpdateScalarComponent(m_pController2->HAnalog[2], trig2, 0);
		vr::VRDriverInput()->UpdateScalarComponent(m_pController->HAnalog[2], grip1, 0);
		vr::VRDriverInput()->UpdateScalarComponent(m_pController2->HAnalog[2], grip2, 0);
	}

	closesocket(s);
}

EVRInitError CServerDriver_Sample::Init(vr::IVRDriverContext *pDriverContext)
{
	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0)
	{
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
	if (m_pController)
	{
		m_pController->RunFrame();
	}
	if (m_pController2)
	{
		m_pController2->RunFrame();
	}

#if 0
	vr::VREvent_t vrEvent;
	while (vr::VRServerDriverHost()->PollNextEvent(&vrEvent, sizeof(vrEvent)))
	{
		if (m_pController)
		{
			m_pController->ProcessEvent(vrEvent);
		}
		if (m_pController2)
		{
			m_pController2->ProcessEvent(vrEvent);
		}
	}
#endif
}
