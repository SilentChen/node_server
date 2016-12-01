/*
*	������ȫ�ֱ���������
*	���ߣ�������
*	ʱ�䣺2016/11/22
*/

require('enum.js');
require('message.js');
require('struct.js');
require('config.js');
require('util.js');
require('timer.js');
require('public_server/guild.js');
require('public_server/rank.js');

//ȫ�ֱ�����
var global = function () {};

//���ù�����
global.config = new Config();
global.config.init();
//��ʱ��
global.timer = new Timer();

///////////////////center_server����////////////////////////
//sid idx
global.sid_idx = 0;
//sid set
global.sid_set = new Set();
//node_id--node_info
global.node_map = new Map();
//gate�б�
global.gate_list = new Array();
//game�б�
global.game_list = new Array();
//account--Token_Info
global.account_token_map = new Map();

///////////////////master_server����////////////////////////
//node_id--node_status
global.node_status_map = new Map();

///////////////////gate_server����////////////////////////
//gate���̽ڵ���Ϣ
global.gate_node_info = null;
//game_node_id--game_cid
global.game_nid_cid_map = new Map();
//client_cid--session
global.cid_session_map = new Map();
//sid--session
global.sid_session_map = new Map();
//account--session
global.account_session_map = new Map();

///////////////////game_server����////////////////////////
//game���̽ڵ���Ϣ
global.game_node_info = null;
//sid--gate_endpoint_id
global.sid_gate_eid_map = new Map();
//sid--Create_Role_Info
global.sid_create_role_map = new Map();
//sid--logout_time
global.logout_map = new Map();
//sid----game_player
global.sid_game_player_map = new Map();
//role_id---game_player
global.role_id_game_player_map = new Map();
//role_name---game_player
global.role_name_game_player_map = new Map();

///////////////////public_server����////////////////////////
//sid----public_player
global.sid_public_player_map = new Map();
//role_id---public_player
global.role_id_public_player_map = new Map();
//���������
global.guild_manager = new Guild_Manager();
//���а������
global.rank_manager = new Rank_Manager();