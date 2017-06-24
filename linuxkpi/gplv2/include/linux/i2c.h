#ifndef _LINUX_I2C_H_
#define _LINUX_I2C_H_

#include <linux/mod_devicetable.h>
#include <linux/device.h>
#include <linux/mutex.h>
#include <linux/completion.h>
#include <linux/sched.h>
#include <linux/of.h>

struct i2c_adapter;
struct i2c_msg;



struct i2c_msg {
	__u16 addr;	/* slave address			*/
	__u16 flags;
#define I2C_M_TEN		0x0010	/* this is a ten bit chip address */
#define I2C_M_RD		0x0001	/* read data, from slave to master */
#define I2C_M_STOP		0x8000	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_NOSTART		0x4000	/* if I2C_FUNC_NOSTART */
#define I2C_M_REV_DIR_ADDR	0x2000	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_IGNORE_NAK	0x1000	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_NO_RD_ACK		0x0800	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_RECV_LEN		0x0400	/* length will be first received byte */
	__u16 len;		/* msg length				*/
	__u8 *buf;		/* pointer to msg data			*/
};

#define I2C_FUNC_I2C			0x00000001
#define I2C_FUNC_10BIT_ADDR		0x00000002
#define I2C_FUNC_PROTOCOL_MANGLING	0x00000004
#define I2C_FUNC_SMBUS_PEC		0x00000008
#define I2C_FUNC_NOSTART		0x00000010
#define I2C_FUNC_SMBUS_BLOCK_PROC_CALL	0x00008000
#define I2C_FUNC_SMBUS_QUICK		0x00010000
#define I2C_FUNC_SMBUS_READ_BYTE	0x00020000
#define I2C_FUNC_SMBUS_WRITE_BYTE	0x00040000
#define I2C_FUNC_SMBUS_READ_BYTE_DATA	0x00080000
#define I2C_FUNC_SMBUS_WRITE_BYTE_DATA	0x00100000
#define I2C_FUNC_SMBUS_READ_WORD_DATA	0x00200000
#define I2C_FUNC_SMBUS_WRITE_WORD_DATA	0x00400000
#define I2C_FUNC_SMBUS_PROC_CALL	0x00800000
#define I2C_FUNC_SMBUS_READ_BLOCK_DATA	0x01000000
#define I2C_FUNC_SMBUS_WRITE_BLOCK_DATA 0x02000000
#define I2C_FUNC_SMBUS_READ_I2C_BLOCK	0x04000000
#define I2C_FUNC_SMBUS_WRITE_I2C_BLOCK	0x08000000

#define I2C_FUNC_SMBUS_BYTE		(I2C_FUNC_SMBUS_READ_BYTE | \
					 I2C_FUNC_SMBUS_WRITE_BYTE)
#define I2C_FUNC_SMBUS_BYTE_DATA	(I2C_FUNC_SMBUS_READ_BYTE_DATA | \
					 I2C_FUNC_SMBUS_WRITE_BYTE_DATA)
#define I2C_FUNC_SMBUS_WORD_DATA	(I2C_FUNC_SMBUS_READ_WORD_DATA | \
					 I2C_FUNC_SMBUS_WRITE_WORD_DATA)
#define I2C_FUNC_SMBUS_BLOCK_DATA	(I2C_FUNC_SMBUS_READ_BLOCK_DATA | \
					 I2C_FUNC_SMBUS_WRITE_BLOCK_DATA)
#define I2C_FUNC_SMBUS_I2C_BLOCK	(I2C_FUNC_SMBUS_READ_I2C_BLOCK | \
					 I2C_FUNC_SMBUS_WRITE_I2C_BLOCK)

#define I2C_FUNC_SMBUS_EMUL		(I2C_FUNC_SMBUS_QUICK | \
					 I2C_FUNC_SMBUS_BYTE | \
					 I2C_FUNC_SMBUS_BYTE_DATA | \
					 I2C_FUNC_SMBUS_WORD_DATA | \
					 I2C_FUNC_SMBUS_PROC_CALL | \
					 I2C_FUNC_SMBUS_WRITE_BLOCK_DATA | \
					 I2C_FUNC_SMBUS_I2C_BLOCK | \
					 I2C_FUNC_SMBUS_PEC)


/* i2c adapter classes (bitmask) */
#define I2C_CLASS_HWMON		(1<<0)	/* lm_sensors, ... */
#define I2C_CLASS_DDC		(1<<3)	/* DDC bus on graphics adapters */
#define I2C_CLASS_SPD		(1<<7)	/* Memory modules */


#define I2C_CLIENT_TEN		0x10	/* we have a ten bit chip address */
					/* Must equal I2C_M_TEN below */
#define I2C_CLIENT_SLAVE	0x20
/* Internal numbers to terminate lists */
#define I2C_CLIENT_END		0xfffeU

/*
 * Data for SMBus Messages
 */
#define I2C_SMBUS_BLOCK_MAX	32	/* As specified in SMBus standard */
union i2c_smbus_data {
	__u8 byte;
	__u16 word;
	__u8 block[I2C_SMBUS_BLOCK_MAX + 2]; /* block[0] is used for length */
			       /* and one more for user-space compatibility */
};

/* i2c_smbus_xfer read or write markers */
#define I2C_SMBUS_READ	1
#define I2C_SMBUS_WRITE	0

struct i2c_algorithm {
	/* If an adapter algorithm can't do I2C-level access, set master_xfer
	   to NULL. If an adapter algorithm can do SMBus access, set
	   smbus_xfer. If set to NULL, the SMBus protocol is simulated
	   using common I2C messages */
	/* master_xfer should return the number of messages successfully
	   processed, or a negative value on error */
	int (*master_xfer)(struct i2c_adapter *adap, struct i2c_msg *msgs,
			   int num);
	int (*smbus_xfer) (struct i2c_adapter *adap, u16 addr,
			   unsigned short flags, char read_write,
			   u8 command, int size, union i2c_smbus_data *data);

	/* To determine what the adapter supports */
	u32 (*functionality) (struct i2c_adapter *);
};

/**
 * struct i2c_lock_operations - represent I2C locking operations
 * @lock_bus: Get exclusive access to an I2C bus segment
 * @trylock_bus: Try to get exclusive access to an I2C bus segment
 * @unlock_bus: Release exclusive access to an I2C bus segment
 *
 * The main operations are wrapped by i2c_lock_bus and i2c_unlock_bus.
 */
struct i2c_lock_operations {
	void (*lock_bus)(struct i2c_adapter *, unsigned int flags);
	int (*trylock_bus)(struct i2c_adapter *, unsigned int flags);
	void (*unlock_bus)(struct i2c_adapter *, unsigned int flags);
};

struct i2c_adapter {
	struct module *owner;
	unsigned int class;		  /* classes to allow probing for */
	const struct i2c_algorithm *algo; /* the algorithm to access the bus */
	void *algo_data;

	/* data fields that are valid for all devices	*/
	const struct i2c_lock_operations *lock_ops;
	struct mutex bus_lock;
	int timeout;			/* in jiffies */
	int retries;
	struct device dev;		/* the adapter device */

	int nr;
	char name[48];
	struct completion dev_released;

};

struct i2c_board_info {
	char		type[I2C_NAME_SIZE];
	unsigned short	flags;
	unsigned short	addr;
	void		*platform_data;
	struct dev_archdata	*archdata;
	struct device_node *of_node;
	struct fwnode_handle *fwnode;
	int		irq;
};

struct i2c_client {
	unsigned short flags;		/* div., see below		*/
	unsigned short addr;		/* chip address - NOTE: 7bit	*/
					/* addresses are stored in the	*/
					/* _LOWER_ 7 bits		*/
	char name[I2C_NAME_SIZE];
	struct i2c_adapter *adapter;	/* the adapter we sit on	*/
	struct device dev;		/* the device structure		*/
	int irq;			/* irq issued by device		*/
	struct list_head detected;
#if IS_ENABLED(CONFIG_I2C_SLAVE)
	i2c_slave_cb_t slave_cb;	/* callback for slave mode	*/
#endif
};

struct i2c_driver {
	unsigned int class;

	/* Notifies the driver that a new bus has appeared. You should avoid
	 * using this, it will be removed in a near future.
	 */
	int (*attach_adapter)(struct i2c_adapter *) __deprecated;

	/* Standard driver model interfaces */
	int (*probe)(struct i2c_client *, const struct i2c_device_id *);
	int (*remove)(struct i2c_client *);

	/* driver model interfaces that don't relate to enumeration  */
	void (*shutdown)(struct i2c_client *);

	/* Alert callback, for example for the SMBus alert protocol.
	 * The format and meaning of the data value depends on the protocol.
	 * For the SMBus alert protocol, there is a single bit of data passed
	 * as the alert response's low bit ("event flag").
	 */
	void (*alert)(struct i2c_client *, unsigned int data);

	/* a ioctl like command that can be used to perform specific functions
	 * with the device.
	 */
	int (*command)(struct i2c_client *client, unsigned int cmd, void *arg);

	struct device_driver driver;
	const struct i2c_device_id *id_table;

	/* Device detection callback for automatic device creation */
	int (*detect)(struct i2c_client *, struct i2c_board_info *);
	const unsigned short *address_list;
	struct list_head clients;
};
#define to_i2c_driver(d) container_of(d, struct i2c_driver, driver)


static inline int
i2c_adapter_id(struct i2c_adapter *adap)
{
	return adap->nr;
}

extern struct i2c_client *
i2c_new_device(struct i2c_adapter *adap, struct i2c_board_info const *info);
extern int i2c_add_adapter(struct i2c_adapter *adapter);

extern int i2c_del_adapter(struct i2c_adapter *);

extern int i2c_register_driver(struct module *, struct i2c_driver *);
extern void i2c_del_driver(struct i2c_driver *);


extern int i2c_transfer(struct i2c_adapter *adap, struct i2c_msg *msgs,
                        int num);
static inline void
i2c_unregister_device(struct i2c_client *client)
{
	device_unregister(&client->dev);
}

static inline void *
i2c_get_adapdata(const struct i2c_adapter *dev)
{
	return dev_get_drvdata(&dev->dev);
}

static inline void
i2c_set_adapdata(struct i2c_adapter *dev, void *data)
{
	dev_set_drvdata(&dev->dev, data);
}

/**
 * i2c_lock_bus - Get exclusive access to an I2C bus segment
 * @adapter: Target I2C bus segment
 * @flags: I2C_LOCK_ROOT_ADAPTER locks the root i2c adapter, I2C_LOCK_SEGMENT
 *	locks only this branch in the adapter tree
 */
static inline void
i2c_lock_bus(struct i2c_adapter *adapter, unsigned int flags)
{
	adapter->lock_ops->lock_bus(adapter, flags);
}

/**
 * i2c_trylock_bus - Try to get exclusive access to an I2C bus segment
 * @adapter: Target I2C bus segment
 * @flags: I2C_LOCK_ROOT_ADAPTER tries to locks the root i2c adapter,
 *	I2C_LOCK_SEGMENT tries to lock only this branch in the adapter tree
 *
 * Return: true if the I2C bus segment is locked, false otherwise
 */
static inline int
i2c_trylock_bus(struct i2c_adapter *adapter, unsigned int flags)
{
	return adapter->lock_ops->trylock_bus(adapter, flags);
}

/**
 * i2c_unlock_bus - Release exclusive access to an I2C bus segment
 * @adapter: Target I2C bus segment
 * @flags: I2C_LOCK_ROOT_ADAPTER unlocks the root i2c adapter, I2C_LOCK_SEGMENT
 *	unlocks only this branch in the adapter tree
 */
static inline void
i2c_unlock_bus(struct i2c_adapter *adapter, unsigned int flags)
{
	adapter->lock_ops->unlock_bus(adapter, flags);
}

#endif
