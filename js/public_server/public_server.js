/*
*	描述：public_server脚本
*	作者：张亚磊
*	时间：2016/02/24
*/

require("js/public_server/public_player.js");

function init(node_info) {
	log_info('public_server init, node_type:',node_info.node_type,' node_id:',node_info.node_id,' node_name:',node_info.node_name);
	global.node_info = node_info;
	global.timer.init();

    for(var i = 0; i < node_info.endpoint_list.length; ++i) {
    	if(node_info.endpoint_list[i].endpoint_type == Endpoint_Type.CONNECTOR
    		&& node_info.endpoint_list[i].endpoint_name != "public_data_connector"
    		&& node_info.endpoint_list[i].endpoint_name != "public_log_connector") {
    	    util.sync_node_info(node_info.endpoint_list[i].endpoint_id);
    	}
    }
    
	//加载公共数据
	load_public_data();
}

function on_hotupdate(file_path) { }

function on_drop_eid(eid) {
    for (var i = 0; i < global.node_info.endpoint_list.length; ++i) {
        if (global.node_info.endpoint_list[i].endpoint_id == eid
    		&& global.node_info.endpoint_list[i].endpoint_name != "public_data_connector"
            && global.node_info.endpoint_list[i].endpoint_name != "public_log_connector") {
            util.sync_node_info(eid);
            break;
        }
    }
}

function on_drop_cid(cid) { }

function on_msg(msg) {
	if (msg.msg_type == Msg_Type.NODE_C2S) {
		process_public_client_msg(msg);
	} else if (msg.msg_type == Msg_Type.NODE_MSG) {
		process_public_node_msg(msg);
	}
}

function on_tick(timer_id) {
    var timer_handler = global.timer.get_timer_handler(timer_id);
	if (timer_handler != null) {
		timer_handler();
	}
}

function send_msg_to_db(msg_id, sid, msg) {
	send_msg(Endpoint.PUBLIC_DATA_CONNECTOR, 0, msg_id, Msg_Type.NODE_MSG, sid, msg);
}

function send_public_msg(cid, msg_id, sid, msg) {
	send_msg(Endpoint.PUBLIC_SERVER, cid, msg_id, Msg_Type.NODE_MSG, sid, msg);
}

function load_public_data() {
	var msg = new Object();
	msg.db_id = DB_Id.GAME;
	msg.key_index = 0;
	msg.struct_name = "Guild_Info";
	msg.data_type = DB_Data_Type.GUILD;
	send_msg_to_db(Msg.SYNC_LOAD_DB_DATA, 0, msg);
	msg.struct_name = "Rank_Info";
	msg.data_type = DB_Data_Type.RANK;
	send_msg_to_db(Msg.SYNC_LOAD_DB_DATA, 0, msg);
}

function process_public_client_msg(msg) {
    var public_player = global.sid_public_player_map.get(msg.sid);
	if (!public_player) {
		return log_error('process_public_client_msg, public_player not exist, game_cid:', msg.cid, ' sid:', msg.sid, ' msg_id:', msg.msg_id);
	}
	
	switch(msg.msg_id) {
	    case Msg.REQ_CREATE_GUILD:
	        global.guild_manager.create_guild(public_player, msg);
		    break;
	    case Msg.REQ_DISSOVE_GUILD:
	        global.guild_manager.dissove_guild(public_player, msg);
		    break;
	    default:
		    log_error('process_public_client_msg, msg_id not exist:', msg.msg_id);
		    break;
	}
}

function process_public_node_msg(msg) {
    switch (msg.msg_id) {
    	case Msg.SYNC_NODE_INFO:
            break;
        case Msg.SYNC_HOT_UPDATE:
	  		hot_update(msg);
	  		break;
        case Msg.SYNC_SEND_MAIL: {
        	var public_player = global.role_name_public_player_map.get(msg.role_name);
        	if(public_player) {
        		public_player.send_msg(msg.msg_id, msg);
        	}
        	break;
        }
        case Msg.SYNC_DB_RET_CODE:
            process_db_ret_code(msg);
            break;
        case Msg.SYNC_RES_SELECT_DB_DATA:
            res_select_db_data(msg);
		    break;
        case Msg.SYNC_RES_GENERATE_ID:
            res_generate_id(msg);
		    break;
	    case Msg.SYNC_SAVE_DB_DATA: {
		    switch (msg.data_type) {
		        case DB_Data_Type.GUILD:
		            global.guild_manager.load_data(msg);
			        break;
		        case DB_Data_Type.RANK:
		            global.rank_manager.load_data(msg);
			        break;
		        default :
			        break;
		    }
		    break;
	    }
	    case Msg.SYNC_GAME_PUBLIC_LOGIN_LOGOUT: {
		    //game通知public玩家上线下线
		    if (msg.login) {
		        var public_player = global.sid_public_player_map.get(msg.sid);
			    if (public_player == null) {
				    public_player = new Public_Player();
			    }
			    public_player.login(msg.cid, msg.sid, msg.role_info);	
		    } 
		    else {
		        var public_player = global.sid_public_player_map.get(msg.sid);
			    if (public_player) {
				    public_player.logout();
			    }
		    }
		    break;
	    }
	    default:
		    log_error('proceess_public_node_msg, msg_id not exist:', msg.msg_id);
		    break;
	}
}

//后台手动热更新
function hot_update(msg) {

}

function process_db_ret_code(msg) {
    switch (msg.opt_msg_id) {
        case Msg.SYNC_SELECT_DB_DATA: {
            if (msg.ret_code == DB_Ret_Code.DATA_NOT_EXIST && msg.query_name == "guild_id") {
                var msg_res = new Object();
                msg_res.type = "guild_id";
                send_msg_to_db(Msg.SYNC_GENERATE_ID, msg.sid, msg_res);
            }
            break;
        }
        default:
            break;
    }
}

function res_select_db_data(msg) {
    switch (msg.data_type) {
        case Select_Data_Type.INT64: {
            if (msg.query_name == "guild_id" && msg.query_value > 0) {
                var player = global.sid_public_player_map.get(msg.sid);
                if (player) {
                    player.send_error_msg(Error_Code.GUILD_HAS_EXIST);
                }
            }
            break;
        }
    }
}

function res_generate_id(msg) {
    var player = global.sid_public_player_map.get(msg.sid);
	if (!player) {
		return log_error('generate guild id, player not exist, sid:',msg.sid);
	}
		
	if (msg.id <= 0) {
		player.send_error_msg(Error_Code.GENERATE_ID_ERROR);
	} else {
		//创建公会时候，既保存到缓存，又保存到db
		var guild_info = new Guild_Info();
		guild_info.guild_id = msg.id;
		guild_info.guild_name = player.role_info.guild_name;
		guild_info.chief_id = player.role_info.role_id;
		guild_info.create_time = util.now_sec();
		
		var msg_res = new Object();
		msg_res.save_type = Save_Type.SAVE_DB_AND_CACHE;
		msg_res.vector_data = true;
		msg_res.db_id = DB_Id.GAME;
		msg_res.struct_name = "Guild_Info";
		msg_res.data_type = DB_Data_Type.GUILD;
		msg_res.guild_list = new Array();
		msg_res.guild_list.push(guild_info);
		send_msg_to_db(Msg.SYNC_SAVE_DB_DATA, this.sid, msg_res);
		
		global.guild_manager.db_create_guild(player, guild_info);
	}
}