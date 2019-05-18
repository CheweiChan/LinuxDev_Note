 
 *     insmod simp_blkdev.ko
 *     mkfs.ext3 /dev/simp_blkdev
 *     mount /dev/simp_blkdev /mnt/blk/
 
       do smoething test:
       mkdir /mnt/blk/test
       rmdir test
       vi test.txt
       rm test.txt
          
 *     umount /mnt/blk/
 *     rmmod simp_blkdev.ko


