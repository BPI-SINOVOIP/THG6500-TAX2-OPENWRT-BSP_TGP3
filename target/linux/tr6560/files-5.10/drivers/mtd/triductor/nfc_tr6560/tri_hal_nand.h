/*
 */

#ifndef __TRI_HAL_NAND_H__
#define __TRI_HAL_NAND_H__

#include "tr6560/tri_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

tri_void tri_hal_nand_wp_enable(tri_void);
tri_void tri_hal_nand_wp_disable(tri_void);
tri_void  tri_hal_nand_transfer_reg(tri_void *dma, tri_void *bus);
tri_int32 tri_hal_nand_drv_rw(tri_void *host, tri_uint32 addr, tri_int32 op_type);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __TRI_HAL_NAND_H__ */
