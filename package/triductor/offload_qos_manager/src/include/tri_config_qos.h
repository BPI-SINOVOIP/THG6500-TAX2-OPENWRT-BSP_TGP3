/******************************************************************************

                  版权所有 (C), 2009-2029, 创耀通信科技股份有限公司

 ******************************************************************************
  文 件 名   : tri_config_qos.h
  版 本 号   : 初稿
  作    者   : 9527
  生成日期   : D2024.12.05
  最近修改   :

******************************************************************************/


#ifndef __TRI_CONFIG_QOS_H__
#define __TRI_CONFIG_QOS_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define MAX_DEV_NUM 30
#define MAX_LENGTH 20
#define MAX_SIZE 256

typedef struct {
	char *target;
	char *proto;
	char *ports;

}RULE_APP;

typedef struct {
	int dlrate;
	int ulrate;
	char host_mac[18];

}RULE_HOST;

typedef struct{
	int ulrate;
	int dlrate;
}RATE;

/*dlrate,ulrate保存配置的上下行总带宽*/
typedef struct{
	int dlrate;
	int ulrate;
	//保存四个等级的速率:0,priority;1,express;2,normal;3bulk
	RATE rate[4];
	//RATE priority;
	//RATE express; 
	//RATE normal;
	//RATE bulk;
}PRIO_RATE;

typedef struct{
	char wan_interface[MAX_DEV_NUM][MAX_LENGTH];
	int wan_count;
	char lan_interface[MAX_DEV_NUM][MAX_LENGTH];
	int lan_count;
}StringArray;;



int init_qos();
void foreach_section_member(void *arg, const char *section_name);
char* get_option(const char* section_type, const char* section_name, const char* option_name);
int check_qos_mode();
int get_offload_switch_status();
int get_prio_rate();
int qos_info_change();
int handle_netlink_message(char* dev_name, char* iftype);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /*__TRI_CONFIG_QOS_H__*/

