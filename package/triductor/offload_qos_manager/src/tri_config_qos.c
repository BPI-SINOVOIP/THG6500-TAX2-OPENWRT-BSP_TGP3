/******************************************************************************

                  版权所有 (C), 2009-2029, 创耀通信科技股份有限公司

 ******************************************************************************
  文 件 名   : tri_config_qos.c
  版 本 号   : 初稿
  作    者   : 9527
  生成日期   : D2024.12.10
  最近修改   :

******************************************************************************/
#include <uci.h>
#include "tri_typedef.h"

extern pthread_mutex_t qos_mutex_lock;

//uci
struct uci_package *pkg = NULL;
struct uci_context *ctx = NULL;
char* qos_mode_value = NULL;	//保存qos_mode
static RULE_APP rule_app;	//保存单次获取的app配置信息
static RULE_HOST rule_host;	//保存单次获取的host配置信息
static PRIO_RATE prio_rate;		//保存每个优先级的上下行速及总速率
static unsigned int rule_sta = 0;	//每次触发配置后统计节点的个数，如：rule_app,rule_host个数
static StringArray  wifi_dev_name;	//保存无线device_name
static StringArray  dev_name;	//保存有线device_name

#define QOS_FILE_NAME "qos"	//qos配置文件名称，如tcqos
#define QOS_SECTION_TYPE "tcqos"	//配置qos_mode的节点属性，如tcqos
#define QOS_SECTION_NAME "qos"	//配置qos_mode的节点名称，如qos
#define QOS_MODE_OPTION_NAME "qos_mode"	//qos_mode option名称
#define RULE_APP_SECTION_TYPE "rule_app"	//rule_app节点类型
#define RULE_HOST_SECTION_TYPE "rule_host"	//rule_host 节点类型

#define FIREWALL_FILE_NAME "firewall"	//防火墙配置文件名称，用于查询offload是否开启
#define PRIO "prio"	//qos_mode的prio模式
#define HOST "host"	//qos_mode的host模式

//rate
#define WIFI_TOTAL_RATE 2000
//prio模式下配置总带款1G时，优先级调度不是很明显，此处默认配置950
#define PRIO_TOTAL_RATE 950
//host模式配置有线的上限1G
#define WIRED_HOST_TOTAL_RATE 1000

//防火墙
#define QOS_TABLE_NAME "qos-table"
#define FLUSH_NFT_TABLE "nft flush table bridge "QOS_TABLE_NAME " 2>/dev/null"
#define ADD_QOS_TABLE "nft add table bridge "QOS_TABLE_NAME
#define ADD_QOS_CHAIN "nft add chain bridge "QOS_TABLE_NAME " upload { type filter \
	hook prerouting priority 0 \\; policy accept \\;}"
#define ADD_QOS_RULE "nft add rule bridge "QOS_TABLE_NAME" upload "

static unsigned int device_num = 0;	//父类使用了1:1的id，编号从2开始	
//tc相关定义

//函数声明
int qos_info_change();


/*other func*/
int get_interface_mtu(char* dev)
{
	char path[256];
    FILE *fp;
    int mtu;

    snprintf(path, sizeof(path), "/sys/class/net/%s/mtu", dev);

    fp = fopen(path, "r");
    if (!fp) {
        perror("fopen");
        return -1;
    }

    if (fscanf(fp, "%d", &mtu) != 1) {
        perror("fscanf");
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return mtu;
}

//速率单位是Mbit/s
int calculate_r2q_value(int rate, int mtu)
{
	return (rate * 1000000) / (8 * mtu);
}

// 检查单个字符是否是合法的十六进制字符
bool is_hex_char(char c) {
    return isdigit(c) || (tolower(c) >= 'a' && tolower(c) <= 'f');
}

bool is_valid_mac(const char *mac) {
    if (mac == NULL) {
        return false;
    }

    // 标准MAC地址格式为 "XX:XX:XX:XX:XX:XX"
    if (strlen(mac) != 17) {
        return false;
    }

    for (int i = 0; i < 17; i++) {
        if ((i % 3 == 2 && mac[i] != ':') || (i % 3 != 2 && !is_hex_char(mac[i]))) {
            return false;
        }
    }

    return true;
}

void processing_buffer_info(char* buff, int flag, StringArray* device_name )
{
	char *line = strtok((char *)buff, "\n");
	char currentDevice[MAX_LENGTH] = "";

	if(flag == 0 || flag == 1){
		//保存wan interface
		while (line != NULL) {
			// 按空格分割
			if(strstr(line, "Link encap:")){
				sscanf(line, "%s", currentDevice);
			}
			// 将第一个部分保存到数组
			if(flag == 0){
				strncpy(device_name->wan_interface[device_name->wan_count], currentDevice, MAX_LENGTH);
				device_name->wan_count++;
			}else{
				strncpy(device_name->lan_interface[device_name->lan_count], currentDevice, MAX_LENGTH);
				device_name->lan_count++;
			}
			// 处理下一行
			line = strtok(NULL, "\n");
		}
	}else{
		//保存wifi的interface name
		while (line) {
			// 检查设备名称
			if (strstr(line, "ESSID:")) {
				sscanf(line, "%s", currentDevice);
			}
			// 检查Mode字段
			if (strstr(line, "Mode:")) {
				if (strstr(line, "Client")) {
					strncpy(device_name->wan_interface[device_name->wan_count], currentDevice, MAX_LENGTH);
					device_name->wan_count++;
				} else if (strstr(line, "Master")) {
					strncpy(device_name->lan_interface[device_name->lan_count], currentDevice, MAX_LENGTH);
					device_name->lan_count++;
				}
			}
			line = strtok(NULL, "\n");
		}
	}

	return;
}

/*flag:0 有线，1 无线*/
int get_interface_name(int flag)
{
	FILE *fp;
    char *buffer = NULL; // 字符串指针
    size_t size = 0;     // 分配的缓冲区大小
	char* command = NULL;

	if(flag == 0){
		command = "ifconfig -a | grep wan";
	}else if(flag == 1){
		command = "ifconfig -a | grep -E '^(lan[1-4])'";
	}else{
		command = "iwinfo";
	}

	// 使用 popen 执行 ifconfig 命令
    fp = popen(command, "r");
    if (fp == NULL) {
		syslog(LOG_ERR, "popen failed.");
        return 1;
    }

    // 动态读取命令输出
    size_t chunk_size = 1024; // 每次扩展的大小
    while (!feof(fp)) {
        // 为 buffer 分配或扩展内存
        buffer = realloc(buffer, size + chunk_size);
        if (buffer == NULL) {
			syslog(LOG_ERR, "realloc failed.");
            pclose(fp);
            return 1;
        }

        // 读取数据到 buffer 的当前位置
        if (fgets(buffer + size, chunk_size, fp) != NULL) {
            size += strlen(buffer + size); // 更新已读数据大小
        }
    }

	if(flag == 0 || flag == 1){
		processing_buffer_info(buffer, flag, &dev_name);
	}else{
		processing_buffer_info(buffer, flag, &wifi_dev_name);
	}

    // 输出结果
    if (buffer) {
        free(buffer); // 释放内存
    }

    // 关闭文件指针
    if (pclose(fp) == -1) {
		syslog(LOG_ERR, "pclose failed.");
        return 1;
    }

	return 0;
}

int get_network_interface_name()
{
	int ret = 0;

	//获取有线dev_name
	ret = get_interface_name(0);	//获取wan device name
	ret |= get_interface_name(1);	//获取lan device name
	//获取有线wifi_dev_name
	ret |= get_interface_name(2);	//获取wifi device name
	if(ret != 0){
		syslog(LOG_ERR, "Failed to obtain the network interface name..");
		return 1;
	}

	return 0;
}

/*tc func*/
int init_qos()
{
	int ret = 0;

	memset(&wifi_dev_name, 0, sizeof(StringArray));
	memset(&dev_name, 0, sizeof(StringArray));

	//获取当前存在的网络设备接口并保存到数组里
    ret = get_network_interface_name();
	if(ret != 0){
		return 1;
	}

	//软件初始化创建qos-table
	system(ADD_QOS_TABLE);
	system(ADD_QOS_CHAIN);

	//根据配置文件配置nft及tc
	ret = qos_info_change(); 
	if(ret != 0){
		syslog(LOG_ERR, "failed to handle the QoS information.");
		return 1;
	}
	
	return 0;
}

void del_tc_config()
{
	char *tc_cmd = "tc qdisc del dev %s root 2>/dev/null";
	char command[50];
	int i = 0;

	//清除tc配置
	for(i = 0; i < dev_name.wan_count; i++)
	{
		snprintf(command, sizeof(command), tc_cmd, dev_name.wan_interface[i]);
		system(command);
	}
	
	for(i = 0; i < dev_name.lan_count; i++)
	{
		snprintf(command, sizeof(command), tc_cmd, dev_name.lan_interface[i]);
		system(command);
	}

	for(i = 0; i < wifi_dev_name.wan_count; i++)
	{
		snprintf(command, sizeof(command), tc_cmd, wifi_dev_name.wan_interface[i]);
		system(command);
	}

	for(i = 0; i < wifi_dev_name.lan_count; i++)
	{
		snprintf(command, sizeof(command), tc_cmd, wifi_dev_name.lan_interface[i]);
		system(command);
	}
}

int clear_nft_and_tc_config()
{
	syslog(LOG_NOTICE, "clear firewall and tc configuration.");
	//清除nft配置
	system(FLUSH_NFT_TABLE);

	//清除tc配置
	del_tc_config();

	return 0;
}

/*rate:2000 含义是不进行总带宽限速*/
void config_prio_tc(char* dev_name, int rate)
{
	char tc_cmd[MAX_SIZE] = "";

	strcpy(tc_cmd, "");
	snprintf(tc_cmd + strlen(tc_cmd), MAX_SIZE-strlen(tc_cmd), \
			"tc qdisc add dev %s root handle 1: tbf rate %dmbit burst 500kb limit 1kb",\
			dev_name, rate);
	system(tc_cmd);

	strcpy(tc_cmd, "");
	snprintf(tc_cmd + strlen(tc_cmd), MAX_SIZE-strlen(tc_cmd), \
			"tc qdisc add dev %s parent 1:1 handle 10: prio bands 8 priomap 0 1 2 3 4 5 6 7", \
			dev_name);
	system(tc_cmd);

	for(int j = 1; j <= 8; j++){
		strcpy(tc_cmd, "");
		snprintf(tc_cmd, MAX_SIZE-strlen(tc_cmd), "tc qdisc add dev %s parent 10:%d handle %d00: pfifo",\
				dev_name, j, j);
		system(tc_cmd);

		//配置filter
		strcpy(tc_cmd, "");
		snprintf(tc_cmd + strlen(tc_cmd), MAX_SIZE-strlen(tc_cmd),\
				"tc filter add dev %s protocol ip parent 10: prio %d handle 0x111100%d%d fw flowid 10:%d", \
				dev_name, j, j, j, j);

		system(tc_cmd);
	}
}

/*flag:是否清除tc配置*/
int config_host_tc(char* dev_name, int total_rate, int rate, int flag)
{
	int mtu, r2q, ceil_rate;
	char tc_cmd[MAX_SIZE] = "";
	char *tc_del_cmd = "tc qdisc del dev %s root 2>/dev/null";

	mtu = get_interface_mtu(dev_name);
	if(mtu < 0){
		syslog(LOG_ERR, "the obtained MTU of the %s port is %d.", dev_name, mtu);
		return 1;
	}

	//第一个规则处理时需要配置默认tc队列
	if(rule_sta == 1){
		if(flag == 1){
			strcpy(tc_cmd, "");
			snprintf(tc_cmd, MAX_SIZE-strlen(tc_cmd), tc_del_cmd, dev_name);
			system(tc_cmd);
		}

		r2q = calculate_r2q_value(total_rate, mtu);

		strcpy(tc_cmd, "");
		snprintf(tc_cmd + strlen(tc_cmd), MAX_SIZE-strlen(tc_cmd), \
				"tc qdisc add dev %s root handle 1: htb default 300 r2q %d",\
				dev_name, r2q);
		system(tc_cmd);
		
		strcpy(tc_cmd, "");
		snprintf(tc_cmd + strlen(tc_cmd), MAX_SIZE-strlen(tc_cmd), \
				"tc class add dev %s parent 1: classid 1:1 htb rate %dmbit ceil %dmbit quantum %d",\
				dev_name, total_rate, total_rate, mtu);
		system(tc_cmd);
		
		strcpy(tc_cmd, "");
		snprintf(tc_cmd, MAX_SIZE-strlen(tc_cmd), \
				"tc class add dev %s parent 1:1 classid 1:300 htb rate %dmbit ceil %dmbit quantum %d", \
				dev_name, total_rate, total_rate, mtu);
		system(tc_cmd);
		
		strcpy(tc_cmd, "");
		snprintf(tc_cmd, MAX_SIZE-strlen(tc_cmd), "tc qdisc add dev %s parent 1:300 handle 300: sfq", \
				dev_name);
		system(tc_cmd);
	}

	ceil_rate = rate < total_rate ? rate : total_rate;
	//配置class,qdisc,filter
	strcpy(tc_cmd, "");
	snprintf(tc_cmd + strlen(tc_cmd), MAX_SIZE-strlen(tc_cmd),\
			"tc class add dev %s parent 1:1 classid 1:%02x htb rate %dmbit ceil %dmbit quantum %d", \
			dev_name, device_num ,rate, ceil_rate, mtu);
	system(tc_cmd);
	
	strcpy(tc_cmd, "");
	snprintf(tc_cmd + strlen(tc_cmd), MAX_SIZE-strlen(tc_cmd),\
			"tc qdisc add dev %s parent 1:%02x handle %02x: sfq", \
			dev_name, device_num, device_num);
	system(tc_cmd);
	
	strcpy(tc_cmd, "");
	snprintf(tc_cmd + strlen(tc_cmd), MAX_SIZE-strlen(tc_cmd),\
			"tc filter add dev %s protocol ip parent 1: prio 1 handle 0x111100%02x fw flowid 1:%02x", \
			dev_name, device_num, device_num);
	system(tc_cmd);

	return 0;
}

//flag:0,有线；1，无线
int config_prio_dev_tc(int flag, char* qos_prio)
{
	int i, total_rate;
	char tc_cmd[MAX_SIZE] = "";
	StringArray netdev_name;
	char *tc_del_cmd = "tc qdisc del dev %s root 2>/dev/null";
	
	if(flag == 0){
		netdev_name = dev_name;
		total_rate = PRIO_TOTAL_RATE;
	}else if(flag == 1){
		netdev_name = wifi_dev_name;
		total_rate = WIFI_TOTAL_RATE;
	}else{
		return 1;
	}
	
	//配置wan口
	for(i = 0; i < netdev_name.wan_count; i++ ){
		//第一个规则处理时需要配置默认tc队列
		if(rule_sta == 1){
			strcpy(tc_cmd, "");
			snprintf(tc_cmd, MAX_SIZE-strlen(tc_cmd), tc_del_cmd, netdev_name.wan_interface[i]);
			system(tc_cmd);

			//config_prio_tc(netdev_name.wan_interface[i], prio_rate.ulrate);
			config_prio_tc(netdev_name.wan_interface[i], total_rate);
		}
	}
	
	//配置lan口
	for(i = 0; i < netdev_name.lan_count; i++ ){
		//第一个规则处理时需要配置默认tc队列
		if(rule_sta == 1){
			strcpy(tc_cmd, "");
			snprintf(tc_cmd, MAX_SIZE-strlen(tc_cmd), tc_del_cmd, netdev_name.lan_interface[i]);
			system(tc_cmd);
	
			//config_prio_tc(netdev_name.lan_interface[i], prio_rate.dlrate);
			config_prio_tc(netdev_name.lan_interface[i], total_rate);
		}
	}

	return 0;
}

int config_host_dev_tc(int flag)
{
	int i, total_rate;
	StringArray netdev_name;
	//int k = atoi(qos_prio) - 1;

	//获取速率，单位mbit/s
	int ulrate = atoi(rule_host.ulrate);
	int dlrate = atoi(rule_host.dlrate);

	if(flag == 0){
		netdev_name = dev_name;
		total_rate = WIRED_HOST_TOTAL_RATE;
	}else if(flag == 1){
		netdev_name = wifi_dev_name;
		total_rate = WIFI_TOTAL_RATE;
	}else{
		return 1;
	}
	

	//配置wan口
	for(i = 0; i < netdev_name.wan_count; i++ ){
		//config_host_tc(netdev_name.wan_interface[i], prio_rate.ulrate, ulrate, 1);
		config_host_tc(netdev_name.wan_interface[i], total_rate, ulrate, 1);
	}
	
	//配置lan口
	for(i = 0; i < netdev_name.lan_count; i++ ){
		//config_host_tc(netdev_name.lan_interface[i], prio_rate.dlrate, dlrate, 1);
		config_host_tc(netdev_name.lan_interface[i], total_rate, dlrate, 1);
	}
	
	return 0;
}

//此接口为创建新的接口时会被触发
int add_dev_config_host_tc(char* dev_name, char* iftype)
{
	int total_rate;
	//int k = atoi(qos_prio) - 1;

	//获取速率，单位mbit/s
	int ulrate = atoi(rule_host.ulrate);
	int dlrate = atoi(rule_host.dlrate);

	if((strcmp(iftype, "STA") == 0) || (strcmp(iftype, "AP") == 0)){
		total_rate = WIFI_TOTAL_RATE;
	}else if(strcmp(iftype, "WIRED") == 0){
		total_rate = WIRED_HOST_TOTAL_RATE;
	}

	if(strcmp(iftype, "STA") == 0 || strcmp(iftype, "WIRED") == 0){
		//config_host_tc(dev_name, prio_rate.ulrate, ulrate, 0);
		config_host_tc(dev_name, total_rate, ulrate, 0);
	}else if(strcmp(iftype, "AP") == 0){
		//config_host_tc(dev_name, prio_rate.dlrate, dlrate, 0);
		config_host_tc(dev_name, total_rate, dlrate, 0);
	}
	return 0;
}

int add_dev_config_prio_tc(char* dev_name, char* iftype, char* qos_prio)
{
	int total_rate;

	if((strcmp(iftype, "STA") == 0) || (strcmp(iftype, "AP") == 0)){
		total_rate = WIFI_TOTAL_RATE;
	}else if(strcmp(iftype, "WIRED") == 0){
		total_rate = PRIO_TOTAL_RATE;
	}

	//配置wan口
	if(strcmp(iftype, "STA") == 0 || strcmp(iftype, "WIRED") == 0){
		//第一个规则处理时需要配置默认tc队列
		if(rule_sta == 1){
			//config_prio_tc(dev_name, prio_rate.ulrate);
			config_prio_tc(dev_name, total_rate);
		}
	}else if(strcmp(iftype, "AP") == 0){
		//第一个规则处理时需要配置默认tc队列
		if(rule_sta == 1){
			//config_prio_tc(dev_name, prio_rate.dlrate);
			config_prio_tc(dev_name, total_rate);
		}
	}

	return 0;
}

static int check_config_prio()
{
	if(strcmp(rule_app.target, "1") == 0 || strcmp(rule_app.target, "2") == 0 || \
			strcmp(rule_app.target, "3") == 0 || strcmp(rule_app.target, "4") == 0 || \
			strcmp(rule_app.target, "5") == 0 || strcmp(rule_app.target, "6") == 0 || \
			strcmp(rule_app.target, "7") == 0 || strcmp(rule_app.target, "8") == 0)
	{
		return 0;
	}
	
	//优先级配置1-8否则返回失败
	return 1;
}

int config_qos(char* dev_name, char* iftype)
{
	int ret = 0;
	char nft_ct_cmd[MAX_SIZE] = "";		//确保空间足够,默认256
	char nft_cmd[MAX_SIZE] = "";
	const char delimiter[] = "+";
	char *token = NULL;
	char *qos_prio = NULL;

	//创建接口不清除tc及防火墙
	if(rule_sta == 1 && dev_name == NULL){
		ret = clear_nft_and_tc_config();
		if(ret != 0)
			return 1;
	}

	//根据配置文件配置nft及tc
	if(strcmp(qos_mode_value, PRIO) == 0){
		if((rule_app.target == NULL) || ((rule_app.proto == NULL) && (rule_app.ports == NULL))){
			return 1;
		}

		syslog(LOG_NOTICE, "%s-target:%s, proto:%s, ports:%s",qos_mode_value,rule_app.target, rule_app.proto, rule_app.ports);

		ret = check_config_prio();
		if(ret != 0){
			syslog(LOG_ERR, "parsing priority error.");
			return 1;
		}
		
		qos_prio = rule_app.target;

		//创建接口调用不配置防火墙只配置tc
		if(dev_name == NULL){
			// 第一次调用strtok，需要传入要分割的字符串
			token = strtok(rule_app.proto, delimiter);
			while (token != NULL) {
				strcpy(nft_cmd, "");
				strcpy(nft_ct_cmd, "");
				
				snprintf(nft_cmd + strlen(nft_cmd),MAX_SIZE-strlen(nft_ct_cmd), "%s %s %s {%s}" \
						,ADD_QOS_RULE, token, "dport", rule_app.ports);
				
				snprintf(nft_ct_cmd + strlen(nft_ct_cmd), MAX_SIZE-strlen(nft_ct_cmd), \
						"%s ct mark set 0x111100%s%s counter", nft_cmd, qos_prio, qos_prio);
				snprintf(nft_cmd + strlen(nft_cmd), MAX_SIZE-strlen(nft_cmd)," meta mark set 0x111100%s%s counter" \
						, qos_prio,qos_prio);
			
				system(nft_ct_cmd);
				system(nft_cmd);
				
				// 继续分割剩余的字符串，strtok的参数为NULL
				token = strtok(NULL, delimiter);
			}
	
			//配置有线
			ret = config_prio_dev_tc(0, qos_prio);
			if(ret != 0){
				syslog(LOG_ERR, "prio:failed to configure wired tc.");
				return 1;	
			}

			//配置无线
			ret = config_prio_dev_tc(1, qos_prio);
			if(ret != 0){
				syslog(LOG_ERR, "prio:failed to configure wireless tc.");
				return 1;	
			}
		}else{
			ret = add_dev_config_prio_tc(dev_name, iftype, qos_prio);
			if(ret != 0){
				syslog(LOG_ERR, "prio:failed to configure %s tc.", dev_name);
				return 1;	
			}
		}
	}else if(strcmp(qos_mode_value, HOST) == 0){
		if(device_num  > 256 || device_num < 2){
			syslog(LOG_ERR, "the number of devices exceeds the limit.");
			return 1;
		}

		device_num = device_num&0xFF;
	
		int ulrate = atoi(rule_host.ulrate);
		int dlrate = atoi(rule_host.dlrate);

		//根据配置文件配置nft及tc
		if(ulrate <= 0 || ulrate > 2500 || dlrate <= 0 || dlrate > 2500){
			return 1;
		}
		
		if(!is_valid_mac(rule_host.host_mac)){
			syslog(LOG_ERR, "illegal MAC address:%s.",rule_host.host_mac);
			return 1;
		}

		syslog(LOG_NOTICE, "%s:ulrate:%s, dlrate:%s, host_mac:%s",qos_mode_value, rule_host.ulrate, \
				rule_host.dlrate, rule_host.host_mac);

		if(dev_name == NULL){	
			strcpy(nft_cmd, "");
			strcpy(nft_ct_cmd, "");
			
			snprintf(nft_cmd + strlen(nft_cmd),MAX_SIZE-strlen(nft_ct_cmd), "%s %s %s " \
					,ADD_QOS_RULE, "ether saddr", rule_host.host_mac);
			
			snprintf(nft_ct_cmd + strlen(nft_ct_cmd), MAX_SIZE-strlen(nft_ct_cmd), \
					"%s ct mark set 0x111100%02x counter", nft_cmd, device_num);
			snprintf(nft_cmd + strlen(nft_cmd), MAX_SIZE-strlen(nft_cmd)," meta mark set 0x111100%02x counter" \
					, device_num);
		
			system(nft_ct_cmd);
			system(nft_cmd);

			//配置有线
			ret = config_host_dev_tc(0);
			if(ret != 0){
				syslog(LOG_ERR, "host:failed to configure wired tc.");
				return 1;	
			}

			//配置无线
			ret = config_host_dev_tc(1);
			if(ret != 0){
				syslog(LOG_ERR, "host:failed to configure wireless tc.");
				return 1;	
			}
		}else{
			ret = add_dev_config_host_tc(dev_name, iftype);
			if(ret != 0){
				syslog(LOG_ERR, "host:failed to configure %s tc.", dev_name);
				return 1;	
			}
		}
		device_num++;
	}

	return 0;
}

/*uci func*/
void foreach_section_member(void *arg, const char *section_name) 
{
    struct uci_section *s = arg;
    struct uci_element *e;
	if(strcmp(qos_mode_value, PRIO) == 0){
		memset(&rule_app, 0, sizeof(rule_app));
	}else if(strcmp(qos_mode_value, HOST) == 0){
		memset(&rule_host, 0, sizeof(rule_host));
	}

    // 遍历section中的所有选项
    uci_foreach_element(&s->options, e) {
        struct uci_option *o = uci_to_option(e);
        if (o->type == UCI_TYPE_STRING) {
            // 输出选项名称和值
			if(strcmp(qos_mode_value, PRIO) == 0){
				if(strcmp(e->name,"target") == 0){
					rule_app.target = o->v.string;		
				}else if(strcmp(e->name, "proto") == 0){
					rule_app.proto = o->v.string;		
				}else if(strcmp(e->name, "ports") == 0){
					rule_app.ports = o->v.string;		
				}
			}else if(strcmp(qos_mode_value, HOST) == 0){
				if(strcmp(e->name,"ulrate") == 0){
					rule_host.ulrate = o->v.string;	
				}else if(strcmp(e->name,"dlrate") == 0){
					rule_host.dlrate = o->v.string;
				}else if(strcmp(e->name, "host_mac") == 0){
					strncpy(rule_host.host_mac, o->v.string, sizeof(rule_host.host_mac) - 1);
					rule_host.host_mac[sizeof(rule_host.host_mac) - 1] = '\0'; // 确保字符串以空字符结尾

				}
			}
        }
    }
}

char* get_option(const char* section_type, const char* section_name, const char* option_name)
{
	struct uci_section *s = NULL;
    struct uci_option *o = NULL;

	// 直接查找配置节
    s = uci_lookup_section(ctx, pkg, section_name);
    if (s) {
		if (strcmp(s->type, section_type) == 0) {
			// 获取配置选项
			o = uci_lookup_option(ctx, s, option_name);
			if (o) {
				if (o->type == UCI_TYPE_STRING) {
					// 输出选项名称和值
					return o->v.string;
				}
			} else {
				syslog(LOG_ERR, "option %s not found.", option_name);
			}
		}else{
			syslog(LOG_ERR, "%s's section type error.", section_type);
		} 
	}else {
		syslog(LOG_ERR, "section %s not found.",  section_name);
    }

	return NULL;
}

int check_qos_mode()
{
	//获取qos_mode option的值
	qos_mode_value = get_option(QOS_SECTION_TYPE, QOS_SECTION_NAME, QOS_MODE_OPTION_NAME);
	if(qos_mode_value == NULL){
		syslog(LOG_ERR, "Failed to retrieve the value of the option.");
		return 1;
	}
	//判断qos_mode option的值 是否合法
	if((strcmp(qos_mode_value, PRIO) != 0) && (strcmp(qos_mode_value, HOST) != 0)){
		syslog(LOG_ERR, "%s is error.", QOS_MODE_OPTION_NAME);
		return 1;
	}
	syslog(LOG_NOTICE, "qos_mode is %s.",qos_mode_value);

	return 0;
}

#if 0
int get_prio_rate()
{
	char* section_type = "class";
	char* option_value = NULL;
	char* Priority = "Priority";
	char* Express = "Express";
	char* Normal = "Normal";
	char* Bulk = "Bulk";
	char* uploadrate = "uploadrate";
	char* downloadrate = "downloadrate";

#if 0
	option_value = get_option(QOS_SECTION_TYPE, QOS_SECTION_NAME, "upload");
	if(option_value){
		prio_rate.ulrate = atoi(option_value);
	}else{
		goto do_error;
	}

	option_value = get_option(QOS_SECTION_TYPE, QOS_SECTION_NAME, "download");
	if(option_value){
		prio_rate.dlrate = atoi(option_value);
	}else{
		goto do_error;
	}

#endif
	option_value = get_option(section_type, Priority, uploadrate);
	if(option_value){
		prio_rate.rate[0].ulrate = atoi(option_value);
	}else{
		goto do_error;
	}

	option_value = get_option(section_type, Priority, downloadrate);
	if(option_value){
		prio_rate.rate[0].dlrate = atoi(option_value);
	}else{
		goto do_error;
	}

	option_value = get_option(section_type, Express, uploadrate);
	if(option_value){
		prio_rate.rate[1].ulrate = atoi(option_value);
	}else{
		goto do_error;
	}

	option_value = get_option(section_type, Express, downloadrate);
	if(option_value){
		prio_rate.rate[1].dlrate = atoi(option_value);
	}else{
		goto do_error;
	}

	option_value = get_option(section_type, Normal, uploadrate);
	if(option_value){
		prio_rate.rate[2].ulrate = atoi(option_value);
	}else{
		goto do_error;
	}

	option_value = get_option(section_type, Normal, downloadrate);
	if(option_value){
		prio_rate.rate[2].dlrate = atoi(option_value);
	}else{
		goto do_error;
	}

	option_value = get_option(section_type, Bulk, uploadrate);
	if(option_value){
		prio_rate.rate[3].ulrate = atoi(option_value);
	}else{
		goto do_error;
	}

	option_value = get_option(section_type, Bulk, downloadrate);
	if(option_value){
		prio_rate.rate[3].dlrate = atoi(option_value);
	}else{
		goto do_error;
	}

	return 0;
do_error:
	syslog(LOG_ERR, "get rate error.");
	return 1;
}
#endif

int parse_and_configure_qos_info(char* dev_name, char* iftype)
{
	struct uci_element *e = NULL;
	int ret = 0;
	
	ctx = NULL;
	pkg = NULL;
    
	ctx = uci_alloc_context();
    if (!ctx)
    {
		syslog(LOG_ERR, "No memory.");
        return 1;
    }

	//加载QoS配置包
    if (uci_load(ctx, QOS_FILE_NAME, &pkg) != UCI_OK) {
		syslog(LOG_ERR, "Failed to load qos package.");
		goto do_error;
    }

	//check qos_mode
	if(check_qos_mode() != 0){
		syslog(LOG_ERR, "check_qos_mode error.");
		goto do_error;
	}

#if 0
	//必须要先拿到上下行速率
	memset(&prio_rate, 0, sizeof(prio_rate));
	ret = get_prio_rate();
	if(ret != 0){
		syslog(LOG_ERR, "Failed to obtain the rates for different priorities.");
		goto do_error;
	}
#endif

	//遍历配置文件中的所有section
    uci_foreach_element(&pkg->sections, e) {
        struct uci_section *s = uci_to_section(e);
		if(strcmp(qos_mode_value, PRIO) == 0){
			if (strcmp(s->type, RULE_APP_SECTION_TYPE) == 0) {
				rule_sta++;
				// 对于每个rule_app section，保存配置信息
				foreach_section_member(s, s->e.name);
				//每个节点信息，配置对应的tc及防火墙
				if(config_qos(dev_name, iftype) != 0){
					rule_sta--;
					syslog(LOG_ERR, "config tc information error,qos_mode:%s.",qos_mode_value);
				}
			}
		}else if(strcmp(qos_mode_value, HOST) == 0){
			if (strcmp(s->type, RULE_HOST_SECTION_TYPE) == 0) {
				rule_sta++;
				// 对于每个rule_host section，保存配置信息
				foreach_section_member(s, s->e.name);
				//每个节点信息，配置对应的tc及防火墙
				if(config_qos(dev_name, iftype) != 0){
					rule_sta--;
					syslog(LOG_ERR, "config tc information error,qos_mode:%s.",qos_mode_value);
				}
			}
			
		}else{
			goto do_error;
		}
    }
	
	if(rule_sta <= 0){
		syslog(LOG_NOTICE, "Rule statistics is %d.",rule_sta);
		goto do_error;
	}

	return 0;

do_error:
    uci_free_context(ctx); //释放UCI上下文环境对象
	return 1;
}

int get_offload_switch_status()
{
	char* section_name = "defaults";
	char* option_name = "flow_offloading";
	struct uci_element *e = NULL;
	struct uci_section *sec = NULL;
	const char *value = NULL;
	
	ctx = NULL;
	pkg = NULL;

	ctx = uci_alloc_context(); 
    if (!ctx)
    {
		syslog(LOG_ERR, "No memory.");
        return 1;
    }


	//加载QoS配置包
    if (uci_load(ctx, FIREWALL_FILE_NAME, &pkg) != UCI_OK) {
		syslog(LOG_ERR, "Failed to load firewall package.");
        uci_free_context(ctx);
        return 1;
    }

    uci_foreach_element(&pkg->sections, e) {
        sec = uci_to_section(e);
        if (strcmp(sec->type, section_name) == 0) {
            value = uci_lookup_option_string(ctx, sec, option_name);
            if (value) {
				if(strcmp(value, "1") == 0){
					uci_free_context(ctx);
					return 0;
				}else{
					syslog(LOG_ERR, "Offload's value is incorrect.");
					goto do_error;
				}
            } else {
				syslog(LOG_ERR, "Offload is in the off state.");
				goto do_error;
            }
            break; // 找到目标 section 后可以退出
        }
    }

do_error:
    uci_free_context(ctx);
	return 1;
}

int qos_info_change()
{
	int ret = 0;

	//获取offload 开关状态
	ret = get_offload_switch_status();
	if(ret != 0){
		return 1;
	}

	pthread_mutex_lock(&qos_mutex_lock);
	rule_sta = 0;
	device_num = 2;
	
	ret = parse_and_configure_qos_info(NULL, NULL);
	if(ret != 0){
		syslog(LOG_ERR, "Failed to handle the QoS information.");
		return 1;
	}
	
	pthread_mutex_unlock(&qos_mutex_lock);

	syslog(LOG_NOTICE, "Configuration of QoS information completed.");

	return 0;
}

//处理netlink消息
int handle_netlink_message(char* dev_name, char* iftype)
{
	int ret = 0;
	if(dev_name == NULL || iftype == NULL){
		syslog(LOG_ERR, "dev_name or iftype pointer is null.");	
		return 1;
	}

	syslog(LOG_NOTICE, "dev_name:%s, iftype:%s.",dev_name, iftype);
	//获取offload 开关状态
	ret = get_offload_switch_status();
	if(ret != 0){
		return 1;
	}

	pthread_mutex_lock(&qos_mutex_lock);
	
	rule_sta = 0;
	device_num = 2;
	ret = parse_and_configure_qos_info(dev_name, iftype);
	if(ret != 0){
		syslog(LOG_ERR, "Failed to handle the QoS information.");
		return 1;
	}
	
	pthread_mutex_unlock(&qos_mutex_lock);
	return 0;
}
