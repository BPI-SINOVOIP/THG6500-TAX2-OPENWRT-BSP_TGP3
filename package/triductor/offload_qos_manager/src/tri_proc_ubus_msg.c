/******************************************************************************

                  版权所有 (C), 2009-2029, 创耀通信科技股份有限公司

 ******************************************************************************
  文 件 名   : tri_proc_ubus_msg.c 
  版 本 号   : 初稿
  作    者   : 9527
  生成日期   : D2024.12.05
  最近修改   :

******************************************************************************/

#include <libubox/blobmsg_json.h>
#include <libubus.h>
#include <json-c/json.h>

#include "tri_typedef.h"

static struct ubus_context *ctx;
static struct ubus_subscriber config_change_event;
static uint32_t obj_id;



static int handle_rule_app_event(struct ubus_context *ctx, struct ubus_object *obj,
                                struct ubus_request_data *req, const char *method,
                                struct blob_attr *msg)
{
	int ret = 0;

    json_object *js_tmp = NULL, *js_obj = NULL;
    char *str = NULL, *stringMsg = NULL;

    stringMsg = blobmsg_format_json(msg, true);
	syslog(LOG_NOTICE, "blobmsg_format_json:%s.", stringMsg);

    js_obj = json_tokener_parse(stringMsg);

    js_tmp = json_object_object_get(js_obj, "config");
    if (js_tmp != NULL) {
      str = (char *)json_object_get_string(js_tmp);
      if (strcmp(str, "qos") == 0) {
		ret = qos_info_change();
		if(ret != 0){
			syslog(LOG_NOTICE, "Quality of Service message processing failed.");
		}
      }
    }
    
	return true;
}

// 注册订阅
static int register_subscriber(struct ubus_context *ctx)
{
	int ret;
    memset(&config_change_event, 0, sizeof(config_change_event));
    config_change_event.cb = handle_rule_app_event;

    // 订阅事件
	ret = ubus_register_subscriber(ctx, &config_change_event);
    if(ret) {
		syslog(LOG_ERR, "Failed to register event handler.");
        return ret;
    }

	ret = ubus_lookup_id(ctx, "service", &obj_id);
	if(ret){
		syslog(LOG_ERR, "Failed to look up service.");
        return ret;
    }

	ret = ubus_subscribe(ctx, &config_change_event, obj_id);
	if(ret){
		syslog(LOG_ERR, "Failed to subscriber.");
        return ret;
    }
	syslog(LOG_NOTICE, "Event handler registered for 'service'.");
    
	return 0;
}

void* ubus_qos_init(void* arg)
{
    const char *ubus_socket = NULL;

    uloop_init();
    // 初始化 UBUS
    ctx = ubus_connect(ubus_socket);
    if (!ctx) {
		syslog(LOG_ERR, "Failed to connect to ubus.");
        return NULL;
    }
	syslog(LOG_NOTICE, "Connected to ubus.");
    
	//主循环监听事件
	ubus_add_uloop(ctx);

    // 注册订阅
    if (register_subscriber(ctx)) {
        ubus_free(ctx);
        return NULL;
    }
    
	uloop_run();

    // 释放资源
    ubus_free(ctx);
    uloop_done();
	
	return (void*)1;
}

