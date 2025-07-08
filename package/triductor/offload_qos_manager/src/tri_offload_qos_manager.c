/******************************************************************************

                  版权所有 (C), 2009-2029, 创耀通信科技股份有限公司

 ******************************************************************************
  文 件 名   : tri_offload_qos_manager.c
  版 本 号   : 初稿
  作    者   : 9527
  生成日期   : D2024.12.05
  最近修改   :

******************************************************************************/
#include <pthread.h>

#include "tri_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

pthread_mutex_t qos_mutex_lock;

int main()
{
	pthread_t thread_ubus, thread_netlink;
	int ret = 0;

	openlog("offload_qos_manager", LOG_CONS, LOG_USER);
	printf("The offload_qos_manager program is initializing.\n");

    pthread_mutex_init(&qos_mutex_lock, NULL);

	//软件启动初始化qos
	ret = init_qos();
	if(ret != 0 ){
		syslog(LOG_ERR, "QoS initialization failed.");
		//return 1;
	}
    
	// 创建ubus线程
    if (pthread_create(&thread_ubus, NULL, ubus_qos_init, NULL) != 0) {
		syslog(LOG_ERR, "Failed to create thread ubus.");
        return 1;
    }

    // 创建第二个线程
    if (pthread_create(&thread_netlink, NULL, netlink_qos_init, NULL) != 0) {
		syslog(LOG_ERR, "Failed to create thread netlink.");
        return 1;
    }

	printf("Offload_qos_manager program initialization completed.\n");
    
	// 等待第一个线程结束
    if (pthread_join(thread_ubus, NULL) != 0) {
		syslog(LOG_ERR, "Failed to join thread ubus");
        return 1;
    }

    // 等待第二个线程结束
    if (pthread_join(thread_netlink, NULL) != 0) {
		syslog(LOG_ERR, "Failed to join thread netlink.");
        return 1;
    }

	syslog(LOG_NOTICE, "Both threads finished execution.");

    return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

