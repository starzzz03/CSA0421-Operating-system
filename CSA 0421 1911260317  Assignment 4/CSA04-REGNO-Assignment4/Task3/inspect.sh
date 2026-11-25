#!/usr/bin/env bash
# Task 3.4: inspect a real file system with df -i, stat, ls -i, tune2fs/dumpe2fs
d=$(mktemp -d); cd "$d"
echo "=== df -iT / ==="; df -iT / | head -3
echo; echo "=== df -hT / ==="; df -hT / | head -3
f=$d/data.bin; dd if=/dev/zero of=$f bs=1M count=10 status=none; ln $f $f.hard; ln -s $f $f.sym
echo; echo "=== ls -li (inode numbers) ==="; ls -li $d
echo; echo "=== stat (file, hard link, symlink) ==="; stat $f | head -4; stat $f.hard | sed -n '3,4p'; stat $f.sym | head -2
echo; echo "=== stat -f / (block size, free blocks) ==="; stat -f / | head -4
img=$d/demo_ext4.img; truncate -s 64M $img; mke2fs -q -t ext4 -F $img 2>&1
echo; echo "=== tune2fs -l on a 64 MB ext4 image ==="; tune2fs -l $img 2>&1 | grep -E "features|Inode count|Block count|Free blocks|Free inodes|Block size|Inode size|Journal inode|Inodes per group"
echo; echo "=== dumpe2fs -h (journal) ==="; dumpe2fs -h $img 2>&1 | grep -iE "Total journal|Journal features|Reserved block count"
echo; echo "=== mount options of / (journal mode) ==="; grep " / " /proc/mounts
echo; echo "=== Task 4.4: I/O scheduler of each disk ==="
for s in /sys/block/*/queue/scheduler; do echo "$s: $(cat $s)  rotational=$(cat $(dirname $s)/rotational)"; done
rm -rf "$d"
