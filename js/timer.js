/*
*	��������ʱ��ϵͳ
*	���ߣ�������
*	ʱ�䣺2016/10/24
*/

function Timer() {
	var timer_map = new Map();
	var timer_id = 1;
	
	this.init = function(node_type) {
		switch(node_type) {
		case Node_Type.CENTER_SERVER: {
			//ע�����token��ʱ����ʱ����10s
			this.register_timer(10000, 0, this.center_token_handler);
			break;
		}
		case Node_Type.GAME_SERVER: {
			//ע����Ҷ�ʱ����ʱ����500ms
			this.register_timer(500, 0, this.game_player_handler);
			break;
		}
		case Node_Type.PUBLIC_SERVER: {
			//ע����Ҷ�ʱ����ʱ����500ms��ÿ�ε��ڱ���������ҽ��д���
			this.register_timer(500, 0, this.public_player_handler);
			//ע�ṫ����Ϣ���涨ʱ����ʱ����30s
			this.register_timer(30000, 0, this.public_save_handler);
			break;
		}
		default:
			break;
		}
	}
	
	//ע�ᶨʱ��
	this.register_timer = function(interval, next_tick, handler) {
		register_timer(timer_id, interval, next_tick);
		timer_map.set(timer_id, handler);
		timer_id++;
	}

	//��ȡ��ʱ��
	this.get_timer_handler = function(timer_id) {
		return timer_map.get(timer_id);
	}
	
	/////////////////////////////////��ʱ��������//////////////////////////////////
	this.center_token_handler = function() {
		var now = util.now_sec();
		account_token_map.forEach(function(value, key, map) {
			if (now - value.token_time >= 10) {
				close_session(Endpoint.CENTER_CLIENT_SERVER, value.cid, Error_Code.TOKEN_TIMEOUT);
				account_token_map.delete(key);	
			}
		});
	}
	
	this.game_player_handler = function() {
		var now = util.now_sec();
		for (var value of role_id_game_player_map.values()) {
  			value.tick(now);
		}
	}
	
	this.public_player_handler = function() {
		var now = util.now_sec();
		for (var value of role_id_public_player_map.values()) {
  			value.tick(now);
		}
	}

	this.public_save_handler = function() {
		guild_manager.save_data_handler();
		rank_manager.save_data();
	}
}
