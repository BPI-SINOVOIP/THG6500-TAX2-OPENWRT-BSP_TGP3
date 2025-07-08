
.PHONY: all clean

TARGET := offload_qos_manager
PWD := $(shell pwd)
CC = $(CROSS_COMPILE)gcc -O0 -g -Wall -save-temps
#SAVE_TEMPS := -save-temps

# the C objects file should be compile according to the C source file
SUB_OBJS = tri_offload_qos_manager.o tri_proc_ubus_msg.o tri_proc_netlink_msg.o tri_config_qos.o


# add the special GCC compile flags
# such as local macros, debug marcos and so on

# add the special link
# such as local include path, local libs and so on
TRI_LDFLAGS += -lpthread -lubus -lubox -lblobmsg_json -ljson-c -luci
INC_DIR := -I$(PWD)/include
TRI_CFLAGS += $(INC_DIR) -fPIC


%.o: %.c 
	$(CC) -c $< -o $@ $(TRI_CFLAGS)


all: $(SUB_OBJS)
	$(CC) $^ -o $(TARGET) $(TRI_LDFLAGS)

clean:
	@rm -rf *.o
	@rm -rf $(TARGET)

