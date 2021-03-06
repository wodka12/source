﻿#pragma once

#include "Croom.h"

#define MAX_ROOM_CNT 512

class Croom_manager : Croom
{
public:
	unordered_map<int, Croom*> map_room;

public:
	Croom_manager();
	~Croom_manager();

	/* 2016.05.23*/
	int scan_empty_room(void);
	/* 2016.05.23*/

	bool create_room(int user_fd, int* room_num);
	void delete_room(void);

	int allotate_room(int room_num, int user_fd);
	int allotate_host(void);

	/* 2016.05.24 */
	int exit_room(int room_num, int user_fd);
	int broadcast_room(ObjectUser* info, CStreamSP* pStreamSP, int flag);
	//flag:0 = join , 1 = exit 현재 테스트버젼 좀더 세분화 시킬필요가 있음 예로 룸별로 브로드캐스팅이냐 로비 전체냐 패킷종류는 무엇이냐.등등등..
	
	/* 2016.05.24 */

	/* 2016.05.31 */
	CStreamSP* pStreamSP = new CStreamSP;
	SOCKETINFO *pc_socket_info;
	int enter_game_req(int user_fd, SOCKETINFO* p_socket_info, CStreamSP* pStreamSP);

	int get_user_cnt_in_room(int room_num);
	/* 2016.05.31 */

};
