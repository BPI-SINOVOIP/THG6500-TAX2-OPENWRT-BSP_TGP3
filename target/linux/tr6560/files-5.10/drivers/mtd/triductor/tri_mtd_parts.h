/*
 */

#ifndef __TRI_MTD_PARTS_H__
#define __TRI_MTD_PARTS_H__

#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

void tri_setup_mtd_partitions(struct mtd_info *mtd_info,
	const unsigned char *boot_param,
	struct mtd_partition **parts,
	unsigned int *mtd_num);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* __TRI_MTD_PARTS_H__ */
