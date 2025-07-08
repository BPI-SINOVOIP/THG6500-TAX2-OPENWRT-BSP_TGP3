/*
 */

#ifndef __TRI_NAND_H__
#define __TRI_NAND_H__

#include "tr6560/tri_common.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define TRI_NAND_ID_MAX_LEN          8

/* supported max page size is 8KB */
#define TRI_NAND_MAX_PAGE_SIZE       0x2000

/*
 * when ECC0, supported max oob size is 512B
 * others, it's TRI_NFC_ECC_OOB_SIZE
 */
#define TRI_NAND_MAX_OOB_SIZE        0x200

/* NAND Flash Manufacturer ID Codes */
#define TRI_NAND_MFR_TOSHIBA         0x98
#define TRI_NAND_MFR_SAMSUNG         0xec
#define TRI_NAND_MFR_FUJITSU         0x04
#define TRI_NAND_MFR_NATIONAL        0x8f
#define TRI_NAND_MFR_RENESAS         0x07
#define TRI_NAND_MFR_STMICRO         0x20
#define TRI_NAND_MFR_HYNIX           0xad
#define TRI_NAND_MFR_MICRON          0x2c
#define TRI_NAND_MFR_AMD             0x01


/* standard nand flash commands */
#define TRI_NAND_CMD_READ0           0
#define TRI_NAND_CMD_READ1           1
#define TRI_NAND_CMD_RNDOUT          5
#define TRI_NAND_CMD_PAGEPROG        0x10
#define TRI_NAND_CMD_READOOB         0x50
#define TRI_NAND_CMD_ERASE1          0x60
#define TRI_NAND_CMD_STATUS          0x70
#define TRI_NAND_CMD_STATUS_MULTI    0x71
#define TRI_NAND_CMD_SEQIN           0x80
#define TRI_NAND_CMD_RNDIN           0x85
#define TRI_NAND_CMD_READID          0x90
#define TRI_NAND_CMD_ERASE2          0xd0
#define TRI_NAND_CMD_RESET           0xff

/* Extended commands for large page devices */
#define TRI_NAND_CMD_READSTART       0x30
#define TRI_NAND_CMD_RNDOUTSTART     0xE0
#define TRI_NAND_CMD_CACHEDPROG      0x15

/* Status bits */
#define TRI_NAND_STATUS_FAIL         0x01
#define TRI_NAND_STATUS_FAIL_N1      0x02
#define TRI_NAND_STATUS_TRUE_READY   0x20
#define TRI_NAND_STATUS_READY        0x40
#define TRI_NAND_STATUS_WP           0x80
#define TRI_NAND_ERASE_WRITE_RETRY_TIME 3

/* standard spi nand flash commands */
#define TRI_SPI_NAND_CMD_READ_ID            0x9F
#define TRI_SPI_NAND_CMD_READ_PAGE          0x13
#define TRI_SPI_NAND_CMD_WRITE_PAGE         0x10
#define TRI_SPI_NAND_CMD_READ_CACHE         0x03
#define TRI_SPI_NAND_CMD_WRITE_CACHE        0x02
#define TRI_SPI_NAND_CMD_WRITE_FEATURE      0x1F
#define TRI_SPI_NAND_CMD_READ_FEATURE       0x0F
#define TRI_SPI_NAND_CMD_ERASE              0xD8
#define TRI_SPI_NAND_CMD_RESET              0xff

#define TRI_SPI_NAND_STD_E                  0x0
#define TRI_SPI_NAND_DUALIN_DUALOUT_E       0x1
#define TRI_SPI_NAND_DUAL_IO_E              0x2
#define TRI_SPI_NAND_QUADIN_QUADOUT_E       0x3
#define TRI_SPI_NAND_QUAD_IO_E              0x4
#define TRI_SPI_NAND_STD_READ_DUMMY_NUM     0x1
#define TRI_SPI_NAND_QUAD_READ_DUMMY_NUM    0x2
#define TRI_SPI_NAND_WRITE_DUMMY_NUM        0x0

/* Read Data bytes */
#define TRI_SPI_NAND_CMD_READ               0x03
/* fast read dual output */
#define TRI_SPI_NAND_CMD_READ_DUAL          0x3B
/* fast read quad output */
#define TRI_SPI_NAND_CMD_READ_QUAD          0x6B
/* Dual I/O High Performance Read */
#define TRI_SPI_NAND_CMD_READ_DUAL_ADDR     0xBB
/* Quad I/O High Performance Read */
#define TRI_SPI_NAND_CMD_READ_QUAD_ADDR     0xEB
/* Page Programming */
#define TRI_SPI_NAND_CMD_PP                 0x02
/* fast program dual input */
#define TRI_SPI_NAND_CMD_WRITE_DUAL         0xA2
/* fast program quad input */
#define TRI_SPI_NAND_CMD_WRITE_QUAD         0x32

#define TRI_SPI_NAND_STATUS_BUSY            0x01
#define TRI_SPI_NAND_STATUS_WRITE_ENABLE    0x02
#define TRI_SPI_NAND_STATUS_ERASE_FAIL      0x04
#define TRI_SPI_NAND_STATUS_WRITE_FAIL      0x08

enum tri_nand_page_size_e {
	TRI_NAND_PAGE_SIZE_2K_E = 0x01,
	TRI_NAND_PAGE_SIZE_4K_E = 0x02,
	TRI_NAND_PAGE_SIZE_8K_E = 0x03,
};

enum tri_nand_block_size_e {
	TRI_NAND_BLOCK_SIZE_64_E  = 0x00,    /* 64 page */
	TRI_NAND_BLOCK_SIZE_128_E = 0x01,    /* 128 page */
	TRI_NAND_BLOCK_SIZE_256_E = 0x02,    /* 256 page */
};

/* nand_state_t - chip states  Enumeration for NAND flash chip state  */
enum tri_nand_state_e {
	TRI_FL_READY,
	TRI_FL_READING,
	TRI_FL_WRITING,
	TRI_FL_ERASING,
	TRI_FL_SYNCING,
	TRI_FL_CACHEDPRG,
	TRI_FL_PM_SUSPENDED,
};

enum tri_nand_op_e{
	TRI_NAND_READ_E      = 0x0,
	TRI_NAND_WRITE_E     = 0x1,
	TRI_NAND_READ_OOB_E  = 0x2,
	TRI_NAND_WRITE_OOB_E = 0x3,
};

/* Keep gcc happy */
struct tri_nand_chip;
struct tri_nand_host;

struct tri_nand_buffers {
	tri_uchar8 ecccalc[TRI_NAND_MAX_OOB_SIZE];
	tri_uchar8 ecccode[TRI_NAND_MAX_OOB_SIZE];
	tri_uchar8 databuf[TRI_NAND_MAX_PAGE_SIZE + TRI_NAND_MAX_OOB_SIZE];
	tri_uchar8 *blk_buf;
};

/* Constants for ECC_MODES  */
enum tri_nand_ecc_modes_t{
	TRI_NAND_ECC_NONE,
	TRI_NAND_ECC_SOFT,
	TRI_NAND_ECC_HW,
	TRI_NAND_ECC_HW_SYNDROME,
};

struct tri_nand_hw_control {
	spinlock_t lock;
	struct tri_nand_chip *active;
	wait_queue_head_t wq;
};

struct tri_spi_nand_rw_cfg_s {
	tri_uchar8 read_if_type;
	tri_uchar8 read_cmd;
	tri_uchar8 read_dummy_num;
	tri_uchar8 write_if_type;
	tri_uchar8 write_cmd;
	tri_uchar8 write_dummy_num;
	tri_uchar8 rsv[2]; /* reserved Byte */
};

/* nand chip basic parameter */
struct tri_nand_spec {
	tri_uint32 options;  /* maintain support for the standard Linux kernel */
	tri_uint32 tri_size; /* size unit Byte, supported max size is 4G Byte */
	tri_uint32 block_size;
	tri_uint32 page_size;
	tri_uint32 oob_size;
	tri_uint32 page_shift;
	tri_uint32 erase_shift;
	tri_uint32 bbt_erase_shift;
	tri_uint32 bbt_len;
	tri_uint32 ecc_type;   /* nand ecc type, support 1bit/4bit/8bit/24bit */
	tri_uchar8 read_addr_cycle;
	tri_uchar8 write_addr_cycle;
	tri_uchar8 erase_addr_cycle;
	tri_uchar8 rsv;
	struct tri_spi_nand_rw_cfg_s spi_nand_rw_cfg;
};

struct tri_nand_ops {
	tri_uint32 addr;
	tri_uint32 data_len;
	tri_uint32 check_bad; /* enable bad block handling to be skipped */
	enum tri_nand_op_e en_op_type;
	tri_uchar8 *data_buf;
	tri_uchar8 *oob_buf;
};

struct tri_nand_ecc_ctrl {
	enum tri_nand_ecc_modes_t mode;
	tri_int32 steps;
	tri_int32 size;
	tri_int32 bytes;
	tri_int32 total;
	tri_int32 prepad;
	tri_int32 postpad;
	struct mtd_oob_region *oobregion;

	tri_void(*hwctl)(struct mtd_info *mtd, tri_int32 mode);
	tri_int32(*calculate)(struct mtd_info *mtd, const tri_uchar8 *dat,
		tri_uchar8 *ecc_code);
	tri_int32(*correct)(struct mtd_info *mtd, tri_uchar8 *dat,
		tri_uchar8 *read_ecc, tri_uchar8 *calc_ecc);
	tri_int32(*read_page_raw)(struct mtd_info *mtd,
		struct tri_nand_chip *chip, tri_uchar8 *buf);
	tri_void(*write_page_raw)(struct mtd_info *mtd,
		struct tri_nand_chip *chip, const tri_uchar8 *buf);
	tri_int32(*read_page)(struct mtd_info *mtd, struct tri_nand_chip *chip,
		tri_uchar8 *buf);
	tri_int32(*read_subpage)(struct mtd_info *mtd, struct tri_nand_chip *chip,
		tri_uint32 offs, tri_uint32 len, tri_uchar8 *buf);
	tri_void(*write_page)(struct mtd_info *mtd, struct tri_nand_chip *chip,
		const tri_uchar8 *buf);
	tri_int32(*read_oob)(struct mtd_info *mtd, struct tri_nand_chip *chip,
		tri_int32 page, tri_int32 sndcmd);
	tri_int32(*write_oob)(struct mtd_info *mtd, struct tri_nand_chip *chip,
		tri_int32 page);
};

struct tri_nand_chip {
	struct tri_nand_spec spec;
	tri_char8 *name;
	tri_uchar8 *oob_poi;
	enum tri_nand_state_e en_state;
	struct tri_nand_hw_control *controller;
	struct tri_nand_hw_control hwcontrol;
	tri_int32 page_buf;
	struct tri_nand_ecc_ctrl ecc;
	struct mtd_oob_ops ops;
	struct tri_nand_buffers *buffers;
	tri_uchar8 *bbt;
	tri_void *priv;
};

/* extern from tri_nand.c */
struct tri_nand_func_s *tri_get_nand_drv_ops(tri_void);
tri_uint32 tri_nand_get_total_size(tri_void);
tri_uint32 tri_nand_get_block_size(tri_void);
tri_uint32 tri_nand_get_page_size(tri_void);
tri_uint32 tri_nand_get_oob_size(tri_void);
struct tri_nand_host *tri_nand_get_nand_host(tri_void);
tri_int32 tri_nand_block_isbad(tri_uint32 addr);
tri_int32 tri_nand_block_markbad(tri_uint32 addr);
tri_int32 tri_nand_oob_read(tri_uchar8 *buf, tri_uint32 offset);
tri_int32 tri_nand_oob_write(tri_uchar8 *buf, tri_uint32 offset);
tri_void tri_nand_dump_bbt(tri_void);

/* extern from tri_nand_bbt.c */
tri_int32 tri_nand_bbt_block_isbad(struct tri_nand_host *host,
	tri_uint32 block_addr);
tri_int32 tri_nand_bbt_block_markbad(struct tri_nand_host *host,
	tri_uint32 block_addr);

/* extern from tri_nand_mtd.c */
tri_int32 tri_nand_mtd_contact(struct mtd_info *mtd);

/* extern form tri_nand_util.c */
tri_int32 tri_nand_util_erase(struct tri_nand_host *host, tri_uint32 dst_addr,
	tri_uint32 len);
tri_int32 tri_nand_util_read(struct tri_nand_host *host, tri_uchar8 *dst_buf,
	tri_uint32 src_addr, tri_uint32 len);
tri_int32 tri_nand_util_read_page(struct tri_nand_host *host,
	tri_uchar8 *dst_buf, tri_uint32 src_addr);
tri_int32 tri_nand_util_write(struct tri_nand_host *host, tri_uchar8 *src_buf,
	tri_uint32 dst_addr, tri_uint32 len);
tri_int32 tri_nand_util_write_page(struct tri_nand_host *host,
	tri_uchar8 *src_buf, tri_uint32 dst_addr);
tri_int32 tri_nand_drv_check_erase(tri_uint32 dst_addr);
tri_int32 tri_nand_drv_check_write(const tri_uchar8 *buf,
	tri_uint32 page_addr);

/* for debug printf */
#ifdef TRI_NAND_DEBUG
#define tri_nand_printf(fmt, arg...) tri_printk(fmt, ##arg)
#else
#define tri_nand_printf(fmt, arg...)
#endif /* TRI_NAND_DEBUG */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __TRI_NAND_H__ */
