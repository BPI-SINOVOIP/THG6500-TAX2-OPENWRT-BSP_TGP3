// triductor dual image design
/*
 * Copyright (C) 2023 TGP3.0 design team
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/mtd/mtd.h>
#include <linux/slab.h>
#include <linux/mtd/partitions.h>

#include <linux/string.h>
#include <../drivers/mtd/triductor/tri_fmc.h>

#include "ofpart_triductor.h"

extern char *tri_get_flash_tag_info(void);
static int boot_flag(void)
{
	int i_upg_flag = 0;
	char *g_pc_flash_info = NULL;
	
	/*获取boot通过ATAG传递过来的upgflag，返回值为0/1*/
	g_pc_flash_info = tri_get_flash_tag_info();
	if (g_pc_flash_info != NULL){
		i_upg_flag = g_pc_flash_info[1];
		printk(KERN_NOTICE "i_upg_flag: %d\r\n", i_upg_flag);
	}
	else{
		i_upg_flag = 0;
	}
	if(1 == i_upg_flag){
		return 1;
	}
	else{
		return 0;
	}
}


int tr6560_partitions_post_parse(struct mtd_info *mtd, struct mtd_partition *parts, int nr_parts)
{
	int i;
	int upg_flag;

	upg_flag = boot_flag();

	if (0 == upg_flag){
		for (i = 0; i < nr_parts; i++) {
			if (!strcmp(parts[i].name, "kernelA")){
				parts[i].name = "kernel";
				printk(KERN_DEBUG "modify parts:%s to %s", "kernelA", parts[i].name);
			}
			else if (!strcmp(parts[i].name, "rootfsA")){
				parts[i].name = "ubi";
				printk(KERN_DEBUG "modify parts:%s to %s", "rootfsA", parts[i].name);
			}
			else if (!strcmp(parts[i].name, "firmwareA")){
				parts[i].name = "firmware";
				printk(KERN_DEBUG "modify parts:%s to %s", "firmwareA", parts[i].name);
			}
		}
	}
	else{
		for (i = 0; i < nr_parts; i++) {
			if (!strcmp(parts[i].name, "kernelB")){
				parts[i].name = "kernel";
				printk(KERN_DEBUG "modify parts:%s to %s", "kernelB", parts[i].name);
			}
			else if (!strcmp(parts[i].name, "rootfsB")){
				parts[i].name = "ubi";
				printk(KERN_DEBUG "modify parts:%s to %s", "rootfsB", parts[i].name);
			}
			else if (!strcmp(parts[i].name, "firmwareB")){
				parts[i].name = "firmware";
				printk(KERN_DEBUG "modify parts:%s to %s", "firmwareB", parts[i].name);
			}
		}
	}
	return 0;
}

