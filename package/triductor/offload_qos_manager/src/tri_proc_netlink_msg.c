/******************************************************************************

                  版权所有 (C), 2009-2029, 创耀通信科技股份有限公司

 ******************************************************************************
  文 件 名   : tri_proc_netlink_msg.c 
  版 本 号   : 初稿
  作    者   : 9527
  生成日期   : D2024.12.05
  最近修改   :

******************************************************************************/
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <linux/netlink.h>

#include "tri_typedef.h"

#define UEVENT_BUFFER_SIZE 2048

void process_string(const char *str) {
    const char *action_key = "ACTION=";
    const char *interface_key = "INTERFACE=";
	const char *iftype_key = "IFTYPE=";
    char action_value[32] = {0};
    char interface_value[32] = {0};
    char iftype_value[32] = {0};
	int ret = 0;

    // 查找 ACTION 和 INTERFACE 的位置
    const char *action_pos = strstr(str, action_key);
    const char *interface_pos = strstr(str, interface_key);
    const char *iftype_pos = strstr(str, iftype_key);

    if (action_pos) {
        // 获取 ACTION 的值
        action_pos += strlen(action_key); // 跳过 "ACTION="
        sscanf(action_pos, "%31[^,]", action_value); // 读取到下一个逗号为止

        if (strcmp(action_value, "Register") == 0 && interface_pos && iftype_pos) {
			iftype_pos += strlen(iftype_key);
			sscanf(iftype_pos, "%31[^,]", iftype_value);
			
			// 获取 INTERFACE 的值
			interface_pos += strlen(interface_key); // 跳过 "INTERFACE="
			sscanf(interface_pos, "%31[^,]", interface_value); // 读取到下一个逗号为止
			if(strcmp(iftype_value, "WIRED") == 0)
			{
				if (strstr(interface_value, "wan") != NULL){
					ret |= handle_netlink_message(interface_value, iftype_value);
				}	
			}else if((strcmp(iftype_value, "AP") == 0) || (strcmp(iftype_value, "STA") == 0)){
				ret |= handle_netlink_message(interface_value, iftype_value);
			}
			if(ret != 0){
				syslog(LOG_NOTICE, "handling netlink message errors,ret=%d.",ret);
			}
        }
    }
}

void* netlink_qos_init(void* arg)
{
	int sockfd;
    struct sockaddr_nl addr;
    char buffer[UEVENT_BUFFER_SIZE];

    // 创建 netlink 套接字
    sockfd = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 配置 sockaddr_nl 结构体
    memset(&addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_groups = 1; // 订阅组 1，接收广播消息

    // 绑定套接字
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Listening for uevent messages...\n");

    // 循环接收 uevent 消息
    while (1) {
        ssize_t len = recv(sockfd, buffer, sizeof(buffer), 0);
        if (len < 0) {
            perror("recv");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        buffer[len] = '\0'; // 确保缓冲区以 NULL 结尾
		process_string(buffer);
    }

    close(sockfd);

	return (void*)1;
}

