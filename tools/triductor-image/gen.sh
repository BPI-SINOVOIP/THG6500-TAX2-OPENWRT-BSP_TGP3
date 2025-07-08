#!/bin/sh

echo $1 $2 $3 $4 $5 $6 $7 $8 $9
cd $1

BOOT_LEN=512K
FIRST_BOOT_LEN=128K
KERNEL_LEN=2560K
FS_LEN=11136K
ROOTFS_DATA_LEN=$9
shift
ZERO_LEN=$9


dd if=/dev/zero ibs=1k count=$ZERO_LEN | tr "\000" "\377" > fileFF.tmp

cat TR6560-startbootimage.bin fileFF.tmp | head --byte=$FIRST_BOOT_LEN > fboot_base.tmp;\
cat fboot_base.tmp TR6560-bootimage.bin fileFF.tmp | head --bytes=$BOOT_LEN > boot_base.tmp;\
cat $2 fileFF.tmp | head --bytes=$KERNEL_LEN > kernel.tmp;\
cat $3 fileFF.tmp | head --bytes=$FS_LEN > rootfs.tmp;\
cat $7 fileFF.tmp | head --bytes=$ROOTFS_DATA_LEN > rootfs_data.tmp;\
cat boot_base.tmp kernel.tmp rootfs.tmp rootfs_data.tmp > $4-factory.tmp

cat $4-factory.tmp fileFF.tmp | head --byte=${ZERO_LEN}K > $4-$5-$6-BurningImage.bin
mv $4-$5-$6-BurningImage.bin $1/

rm -fr *.tmp

cd -
