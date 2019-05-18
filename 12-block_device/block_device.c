#include <linux/module.h>

#include <linux/blkdev.h>

#define SIMP_BLKDEV_DEVICEMAJOR	COMPAQ_SMART2_MAJOR

#define SIMP_BLKDEV_DISKNAME	"simp_blkdev"

#define SIMP_BLKDEV_BYTES	(16*1024*1024)

#define SECTOR_SIZE_SHIFT    9

 

static struct request_queue *simp_blkdev_queue;

static struct gendisk *simp_blkdev_disk;

 

unsigned char simp_blkdev_data[SIMP_BLKDEV_BYTES];

 

static void simp_blkdev_do_request(struct request_queue *q)

{

        struct request *req;

        while ((req = blk_fetch_request(q)) != NULL) {

                if (((blk_rq_pos(req) << SECTOR_SIZE_SHIFT) + blk_rq_bytes(req))

                        > SIMP_BLKDEV_BYTES) {

                        printk(KERN_ERR SIMP_BLKDEV_DISKNAME ": bad request: block=%llu, count=%u\n", (unsigned long long)blk_rq_pos(req),blk_rq_sectors(req));

                        __blk_end_request_cur(req, 0);

                        continue;

                }

 

                switch (rq_data_dir(req)) {

                case READ:

                        do {

                                memcpy(bio_data(req-> bio), simp_blkdev_data + 

                                        (blk_rq_pos(req) << SECTOR_SIZE_SHIFT), 

                                        blk_rq_cur_bytes(req));

				 printk(KERN_INFO SIMP_BLKDEV_DISKNAME ": Read one sector! ");

                        } while (__blk_end_request_cur(req, 0)); 

                        break;

                case WRITE:

                        do {

                                /* 

                                * NOTE: the data buffer for the whole request might not be contiguous,

                                * so we should better handle the bios one by one. The req->buffer just

                                * point to the current bio's data buffer.

                                */

                                memcpy((char *)simp_blkdev_data + 

                                        (blk_rq_pos(req) << SECTOR_SIZE_SHIFT),

                                        bio_data(req-> bio), blk_rq_cur_bytes(req));

					printk(KERN_INFO SIMP_BLKDEV_DISKNAME ": Write one sector! ");

                        } while (__blk_end_request_cur(req, 0)); 

 

                        break;

                default:

                        printk(KERN_ERR "unknown requests\n");

                        /* No default because rq_data_dir(req) is 1 bit */

                        break;

                }

        }

}   

 

struct block_device_operations simp_blkdev_fops = {

	.owner		= THIS_MODULE,

};

 

static int __init simp_blkdev_init(void)

{

	int ret;

 

	simp_blkdev_queue = blk_init_queue(simp_blkdev_do_request, NULL);

	if (!simp_blkdev_queue)
	{

		ret = -ENOMEM;

		goto err_init_queue;
	}

	 printk(KERN_INFO SIMP_BLKDEV_DISKNAME ": inital the queue! ");

	

	simp_blkdev_disk = alloc_disk(1);

	if (!simp_blkdev_disk) 
	{
		ret = -ENOMEM;

		goto err_alloc_disk;
	}

	printk(KERN_INFO SIMP_BLKDEV_DISKNAME ": alloc_disk! ");

	

	strcpy(simp_blkdev_disk->disk_name, SIMP_BLKDEV_DISKNAME);

	simp_blkdev_disk->major = SIMP_BLKDEV_DEVICEMAJOR;

	simp_blkdev_disk->first_minor = 0;

	simp_blkdev_disk->fops = &simp_blkdev_fops;

	simp_blkdev_disk->queue = simp_blkdev_queue;

	set_capacity(simp_blkdev_disk, SIMP_BLKDEV_BYTES>>9);

	add_disk(simp_blkdev_disk);

	printk(KERN_INFO SIMP_BLKDEV_DISKNAME ": add_disk! ");

	

	return 0;

 

err_alloc_disk:

	blk_cleanup_queue(simp_blkdev_queue);

err_init_queue:

	return ret;

}

 

static void __exit simp_blkdev_exit(void)

{

	del_gendisk(simp_blkdev_disk);

	put_disk(simp_blkdev_disk);

	blk_cleanup_queue(simp_blkdev_queue);

}

 

module_init(simp_blkdev_init);

module_exit(simp_blkdev_exit);

 

MODULE_LICENSE("GPL");
