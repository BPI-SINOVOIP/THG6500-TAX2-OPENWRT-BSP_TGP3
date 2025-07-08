/*
 */

#ifndef __TRI_FMC_H__
#define __TRI_FMC_H__

#include "tr6560/tri_common.h"



#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* dma_err | err_alarm | err_inval */
#define TRI_FMC_ECCERR_INTMASK          (0x7 << 3)

#define TRI_OP_MODE_BOOT                 0
#define TRI_OP_MODE_NORMAL               1
#define TRI_FMC_REG_ADDRNUM_NBYTE        (0x5 << 4)
#define TRI_FMC_REG_ADDRNUM_3BYTE        (0x3 << 4)
#define TRI_FMC_REG_ADDRNUM_1BYTE        (0x1 << 4)

/* fmc_reg_op_ctrl */
#define TRI_FMC_REG_DMA_OP_READY_SET     0x1
#define TRI_FMC_REG_DMA_OP_READ          (0x0 << 1)
#define TRI_FMC_REG_DMA_OP_WRITE         (0x1 << 1)
#define TRI_FMC_REG_RD_OP_PAGE           (0x0 << 4)
#define TRI_FMC_REG_RD_OP_OOB            (0x1 << 4)
#define TRI_READ_IFTYPE_MASK             0x7
#define TRI_READ_IFTYPE_SHIFTBIT         7
#define TRI_READ_ADDR_CYCLE_MASK         0x7
#define TRI_READ_ADDR_CYCLE_SHIFTBIT     4
#define TRI_WRITE_ADDR_CYCLE_MASK        0x7
#define TRI_WRITE_ADDR_CYCLE_SHIFTBIT    4
#define TRI_READ_DUMMY_MASK              0xf
#define TRI_FMC_DMA_OP_READY             1
#define TRI_RD_OP_SEL_MASK               0x3
#define TRI_READ_OPCODE_MASK             0xff
#define TRI_WRITE_OPCODE_MASK            0xff
#define TRI_R_DATA_SEL_SHIFTBIT          4
#define TRI_READ_CMD_OPCODE_BIT          16
#define TRI_WRITE_CMD_OPCODE_BIT         8
#define TRI_FLASH_ERASED_VAL             0xFF

/* fmc_reg_op */
#define TRI_FMC_OP_DUMMY_EN              (1 << 8)
#define TRI_FMC_OP_CMD1_EN               (1 << 7)
#define TRI_FMC_OP_ADDR_EN               (1 << 6)
#define TRI_FMC_OP_WRITE_DATA_EN         (1 << 5)
#define TRI_FMC_OP_CMD2_EN               (1 << 4)
#define TRI_FMC_OP_WAIT_READY_EN         (1 << 3)
#define TRI_FMC_OP_READ_DATA_EN          (1 << 2)
#define TRI_FMC_OP_START_EN              (1 << 0)

#define TRI_FMC_REG_NAND_TYPE            0x4
#define TRI_FMC_REG_ADDR_SHIFT           16
#define TRI_ECC_TYPE_0_BIT               0x0
#define TRI_ECC_TYPE_8_BIT               0x1
#define TRI_ECC_TYPE_16_BIT              0x2
#define TRI_ECC_TYPE_24_BIT              0x3
#define TRI_ECC_TYPE_28_BIT              0x4
#define TRI_ECC_TYPE_40_BIT              0x5
#define TRI_ECC_TYPE_64_BIT              0x6

enum tri_nand_ecc_type_e {
	TRI_NAND_ECC_0BIT_E  = 0x0,
	TRI_NAND_ECC_8BIT_E  = 0x1,
	TRI_NAND_ECC_16BIT_E = 0x2,
	TRI_NAND_ECC_24BIT_E = 0x3,
	TRI_NAND_ECC_28BIT_E = 0x4,
	TRI_NAND_ECC_40BIT_E = 0x5,
	TRI_NAND_ECC_64BIT_E = 0x6,
};

#define TRI_IMAGE_HEADER_MAGIC_NUM   0x44414548
#define TRI_NAME_LEN                 16
#define TRI_MAX_MTD_NR               32

struct tri_mtd_s {
	tri_char8  mtd_name[TRI_NAME_LEN];
	tri_uint32 mtd_addr;
	tri_uint32 mtd_size;
};

struct tri_fmc_reg_s {
	volatile tri_uint32 fmc_reg_cfg;              /* (0x00) */
	volatile tri_uint32 fmc_reg_global_cfg;       /* (0x04) */
	volatile tri_uint32 fmc_reg_timint_spi_cfg;   /* (0x08) */
	volatile tri_uint32 fmc_reg_pnd_pwidth_cfg;   /* (0x0c) */
	volatile tri_uint32 fmc_reg_pnd_opidle_cfg;   /* (0x10) */
	volatile tri_uint32 fmc_reg_reserved1;        /* (0x14) */
	volatile tri_uint32 fmc_reg_fmc_int;          /* (0x18) */
	volatile tri_uint32 fmc_reg_fmc_int_en;       /* (0x1c) */
	volatile tri_uint32 fmc_reg_fmc_int_clr;      /* (0x20) */
	volatile tri_uint32 fmc_reg_cmd;              /* (0x24) */
	volatile tri_uint32 fmc_reg_addrh;            /* (0x28) */
	volatile tri_uint32 fmc_reg_addrl;            /* (0x2c) */
	volatile tri_uint32 fmc_reg_op_cfg;           /* (0x30) */
	volatile tri_uint32 fmc_reg_reserved2[1];     /* (0x34) */
	volatile tri_uint32 fmc_reg_data_num;         /* (0x38) */
	volatile tri_uint32 fmc_reg_op;               /* (0x3c) */
	volatile tri_uint32 fmc_reg_dma_len;          /* (0x40) */
	volatile tri_uint32 fmc_reg_reserved3[1];     /* (0x44) */
	volatile tri_uint32 fmc_reg_reserved4[1];     /* (0x48) */
	volatile tri_uint32 fmc_reg_saddr_d0;         /* (0x4c) */
	volatile tri_uint32 fmc_reg_saddr_d1;         /* (0x50) */
	volatile tri_uint32 fmc_reg_reserved5[2];     /* (0x54~0x58) */
	volatile tri_uint32 fmc_reg_saddr_oob;        /* (0x5c) */
	volatile tri_uint32 fmc_reg_reserved6[2];     /* (0x60~0x64) */
	volatile tri_uint32 fmc_reg_op_ctrl;          /* (0x68) */
	volatile tri_uint32 fmc_reg_reserved7[16];    /* (0x6c~0xa8) */
	volatile tri_uint32 fmc_reg_flash_info;       /* (0xac) */
};

tri_int32 tri_fmc_status_check(tri_uint32 *reg);
tri_int32 tri_fmc_write_reg_en(void);
tri_void  tri_fmc_opmode_set(tri_uint32 opmode);
tri_uint32 tri_fmc_ecctype_get(tri_void);
tri_void tri_fmc_ecctype_set(tri_uint32 ecc_value);
tri_void tri_fmc_wp_en_set(tri_uint32 en);
tri_uint32 tri_fmc_wp_en_check(void);
tri_int32 tri_fmc_is_nand_flash_type(void);
tri_int32 tri_fmc_init(void);
tri_int32 tri_fmc_atags_parse(tri_void);
tri_char8 *tri_get_flash_tag_info(tri_void);
tri_char8 * tri_get_flash_spec_info(tri_void);
struct tri_fmc_reg_s *tri_get_fmc_reg(tri_void);
tri_uchar8 *tri_get_fmc_buf(tri_void);
tri_uint32 tri_get_fmc_buf_size(tri_void);
tri_uint32 tri_get_fmc_cs(tri_void);
tri_uint32 tri_fmc_int_status_get(tri_uint32 int_mask);
tri_void tri_fmc_int_en_set(tri_uint32 int_mask, tri_uint32 enable);
tri_void tri_fmc_int_clr_set(tri_uint32 int_mask);
tri_void tri_fmc_reg_dump(tri_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __TRI_FMC_H__ */
