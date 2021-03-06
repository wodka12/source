#include "stdafx.h"
#include "network.h"
#include "ClientManager.h"
#include "TcpSocket.h"
#include "Mysql.h"
#include "battle_mgr.h"

u_short g_port;
network* Singleton<network>::pInstance = NULL;

ClientManager* pClientManager = new ClientManager;
TcpSocket* pTcpSock = new TcpSocket;

/* 2016.05.24 */
CMysql* p_mysql = new CMysql;

battle_mgr* battle_manager = new battle_mgr;
battle_mgr* CSingletonT<battle_mgr>::s_pObject = NULL;

int main(int argc, char **argv)
{
	SYSTEM_INFO systemInfo;

	if (argc != 2) {
		printf("Usage : %s [port number]\n", argv[0]);
		return 1;
	}
	g_port = atoi(argv[1]);

	/* initialize mysql module */
	//p_mysql->initialize_mysql();
	//p_mysql->initialize_AMQ();//AMQ test 


	/* get system information */
	GetSystemInfo(&systemInfo);

	/* set thread counts */
	pTcpSock->Proactor_->Init(systemInfo.dwNumberOfProcessors);

	/* Initialize Client Manager */
	pClientManager->InitClientManager();

	/* initialize networking */
	Singleton<network>::getInstancePtr()->initialize_network();

	/* begin threads */
	pTcpSock->Proactor_->begin_threads();

	/* release to network instance */
	Singleton<network>::releaseInstance();


	return 0;
}


