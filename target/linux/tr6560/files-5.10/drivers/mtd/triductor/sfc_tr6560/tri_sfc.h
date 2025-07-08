/*
 */

#ifndef __TRI_SFC_H__
#define __TRI_SFC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* 最大SPI ID长度 */
#define TRI_SPI_ID_MAX_LEN           8

/* NONE spi flash commands */
#define TRI_SPI_CMD_WREN             0x06    /* Write Enable */
#define TRI_SPI_CMD_WSTATUS          0x01    /* Write status */
#define TRI_SPI_CMD_SE_4K            0x20    /* 4KB  sector Erase */
#define TRI_SPI_CMD_SE_32K           0x52    /* 32KB sector Erase */
#define TRI_SPI_CMD_SE               0xD8    /* 64KB Sector Erase */
#define TRI_SPI_CMD_4SE              0xDC    /* followed by a 4-byte address */
#define TRI_SPI_CMD_BE               0xC7    /* chip erase */
#define TRI_SPI_CMD_RDSR             0x05    /* Read Status Register */
#define TRI_SPI_CMD_WRSR             0x01    /* Write Status Register */
#define TRI_SPI_CMD_RDCF_MXIC        0x15    /* Read mxic config Register */
#define TRI_SPI_CMD_RDCF             0x35    /* Read Config Register */
#define TRI_SPI_CMD_RDSR2            0x35    /* Read Status Register-2 */
#define TRI_SPI_CMD_RDID             0x9F    /* Read Identification */
#define TRI_SPI_CMD_PP               0x02    /* Page Programming */

/* Page Programming followed by a 4-byte address */
#define TRI_SPI_CMD_4PP              0x12
#define TRI_SPI_CMD_WRITE_DUAL       0xA2    /* fast program dual input */
#define TRI_SPI_CMD_WRITE_QUAD       0x32    /* fast program quad input */

/* fast program quad input followed by a 4-byte address */
#define TRI_SPI_CMD_4WRITE_QUAD      0x34

/* Read Data bytes */
#define TRI_SPI_CMD_READ             0x03

/* Read Data bytes followed by a 4-byte address */
#define TRI_SPI_CMD_4READ            0x13

/* Read Data Bytes at Higher Speed */
#define TRI_SPI_CMD_FAST_READ        0x0B

/* Read Data Bytes at Higher Speed followed by a 4-byte address */
#define TRI_SPI_CMD_FAST_4READ       0x0C
#define TRI_SPI_CMD_READ_DUAL        0x3B    /* fast read dual output */

/* fast read dual output followed by a 4-byte address */
#define TRI_SPI_CMD_4READ_DUAL       0x3C
#define TRI_SPI_CMD_READ_QUAD        0x6B    /* fast read quad output */

/* fast read quad output followed by a 4-byte address */
#define TRI_SPI_CMD_4READ_QUAD       0x6C
#define TRI_SPI_CMD_IO_READ_QUAD     0xEB    /* Quad I/O Read */

/* Quad I/O Read followed by a 4-byte address */
#define TRI_SPI_CMD_IO_4READ_QUAD    0xEC

/* SPI FLASH interface enum type */
enum tri_sfc_if_e {
	TRI_SFC_IF_STD_E = 0x0,
	TRI_SFC_IF_DUALIN_DUALOUT_E = 0x1,
	TRI_SFC_IF_DUAL_IO_E = 0x2,
	TRI_SFC_IF_QUADIN_DUALOUT_E = 0x3,
	TRI_SFC_IF_QUAD_IO_E = 0x4,
};

/* spi read write opration parameter */
struct tri_sfc_ops {
	enum tri_sfc_if_e mem_if_type;
	tri_uchar8 dummy_bytes;
	tri_uchar8 cmd;
};

/* spi chip device parameter */
struct tri_sfc_spec {
	tri_uint32 tri_size;
	tri_uint32 erase_size;
	tri_uint32 page_size;
	struct tri_sfc_ops wr_ops;
	struct tri_sfc_ops rd_ops;
	tri_uchar8 erase_cmd;
};

/* spi chip basic info */
struct tri_sfc_chip {
	struct tri_sfc_spec spec;
	tri_uint32 total_size;    /* the sum size of all chip selected */
	tri_void *priv;
};

/* spi chip device list attribute */
struct tri_sfc_ids_s {
	tri_char8 *name;
	tri_uchar8 id[TRI_SPI_ID_MAX_LEN];
	tri_uint32 id_len;
	tri_uint32 tri_size;
	tri_uint32 erase_size;
	tri_uint32 page_size;
	struct tri_sfc_ops wr_ops;
	struct tri_sfc_ops rd_ops;
	tri_uchar8 erase_cmd;
};

/* from tri_sfc_mtd.c */
tri_int32 tri_sfc_mtd_contact(struct mtd_info *mtd);
tri_int32 tri_sfc_mtd_erase(struct mtd_info *mtd,
	struct erase_info *erase_info);
tri_int32 tri_sfc_mtd_write(struct mtd_info *mtd, loff_t dst_addr,
	tri_uint32 len, tri_uint32 *retlen, const tri_uchar8 *buf);
tri_int32 tri_sfc_mtd_read(struct mtd_info *mtd, loff_t src_addr,
	tri_uint32 len, tri_uint32 *retlen, tri_uchar8 *buf);

/* from tri_sfc.c */
tri_int32 tri_sfc_erase(tri_uint32 offset, tri_uint32 len);
tri_int32 tri_sfc_read(tri_uchar8 *buf, tri_uint32 offset, tri_uint32 len);
tri_int32 tri_sfc_write(tri_uchar8 *buf, tri_uint32 offset, tri_uint32 len);
tri_uint32 tri_sfc_get_block_size(tri_void);
tri_uint32 tri_sfc_get_total_size(tri_void);

#ifdef TRI_SFC_DEBUG
#define tri_sfc_printf(fmt, arg...) tri_printk(fmt, ##arg)
#else
#define tri_sfc_printf(fmt, arg...)
#endif /* TRI_SFC_DEBUG */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __TRI_SFC_H__ */