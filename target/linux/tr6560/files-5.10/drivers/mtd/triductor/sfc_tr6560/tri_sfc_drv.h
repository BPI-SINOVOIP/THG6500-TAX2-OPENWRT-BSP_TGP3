/*
 */

#ifndef __TRI_SFC_DRV_H__
#define __TRI_SFC_DRV_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define SECTOR_SIZE_4K      0x1000
#define BLOCK_SIZE_64K      0x10000
#define BLOCK_SIZE_256K     0x40000
#define FLASH_SIZE_8M       0x800000
#define FLASH_SIZE_16M      0x1000000

#define TRI_SFC_MAX_READY_WAIT_JIFFIES   (40 * HZ)

#define TRI_SFC_ADDR_MASK     0x3FFFFFFF /* V300 - 29:0 */
#define TRI_SFC_REG_BUF_SIZE  256        /* V300 - 64databuffer*4 = 256B */

#define TRI_SPINOR_ADDR_MODE_3BYTE   (0x0 << 10)
#define TRI_SPINOR_ADDR_MODE_4BYTE   (0x1 << 10)

/* spiflash driver host structure */
struct tri_sfc_host {
	struct tri_sfc_chip *chip;
	struct mtd_info *mtd;
	struct device *dev;    /* kernel only */
	struct mutex lock;     /* kernel only */
};

/* BUS_FLASH_SIZE, bit */
enum tri_sfc_cmd_rw_e {
	TRI_SFC_CMD_WRITE_E = 0x0,
	TRI_SFC_CMD_READ_E = 0x1,
};

/* BUS_FLASH_SIZE, bit */
enum tri_sfc_bus_flash_size_e {
	TRI_FLASH_SIZE_0_E = 0x0,
	TRI_FLASH_SIZE_512K_E = 0x1,
	TRI_FLASH_SIZE_1MB_E = 0x2,
	TRI_FLASH_SIZE_2MB_E = 0x3,
	TRI_FLASH_SIZE_4MB_E = 0x4,
	TRI_FLASH_SIZE_8MB_E = 0x5,
	TRI_FLASH_SIZE_16MB_E = 0x6,
	TRI_FLASH_SIZE_32MB_E = 0x7,
	TRI_FLASH_SIZE_64MB_E = 0x8,
	TRI_FLASH_SIZE_128MB_E = 0x9,
	TRI_FLASH_SIZE_256MB_E = 0xa,
	TRI_FLASH_SIZE_512MB_E = 0xb,
};

tri_int32 tri_sfc_drv_pre_init(struct tri_sfc_host *host);
tri_void tri_sfc_drv_read_id(tri_uint32 tri_sel,
	tri_uchar8 *buf, tri_uchar8 id_len);
tri_int32 tri_sfc_drv_init(struct tri_sfc_host *host);
tri_int32 tri_sfc_drv_exit(struct tri_sfc_host *host);
tri_int32 tri_sfc_drv_read(struct tri_sfc_host *host, tri_uint32 tri_sel,
	tri_uchar8 *buf, tri_uint32 src_addr, tri_uint32 len);
tri_int32 tri_sfc_drv_write(struct tri_sfc_host *host, tri_uint32 tri_sel,
	const tri_uchar8 *buf, tri_uint32 dst_addr, tri_uint32 len);
tri_int32 tri_sfc_drv_erase_sector(struct tri_sfc_host *host,
	tri_uint32 tri_sel, tri_uint32 dst_addr, tri_uint32 erase_len);
tri_int32 tri_sfc_drv_set_status_reg(tri_uint32 tri_sel,
	tri_uchar8 reg_value[], tri_uint32 reg_len);
tri_uchar8 tri_sfc_drv_get_config_reg(tri_uint32 tri_sel);
tri_uchar8 tri_sfc_drv_get_status_reg(tri_uint32 tri_sel);

#ifdef FLASH_MAINTANCE_SUPPORT
void flash_maintenance_op_hook(struct mtd_info *mtd,
	int opcode, unsigned int offset, unsigned int len);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __TRI_NAND_DRV_H__ */
