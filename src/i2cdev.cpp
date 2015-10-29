/*
 * i2cdev.cpp
 *
 * Copyright (C) 2014 William Markezana <william.markezana@meas-spec.com>
 *
 */

#include "i2cdev.h"
#include "bcm2835.h"

    
/*
 * private defines
 *
 */

/* Max cycles approximately to wait on RXDREADY and TXDREADY event, 
 * This is optimized way instead of using timers, this is not power aware. */
#define TIMEOUT_MAX             (0x40)        /**< MAX while loops to wait for RXD/TXD event */


/*
 * private functions
 *
 */

/*
 * public functions
 *
 */
i2cdev::i2cdev()
{
	configure();
}

void i2cdev::configure()
{  
	m_running = false;
	m_init_successfull = false;
	m_repeated = false;

	m_handle = open(m_device.c_str(), O_RDWR);
	if (m_handle < 0)
	{
		cerr << "can't open device " << m_device << endl;
	}

	if (!bcm2835_init()) return;
	    bcm2835_i2c_begin();
	    bcm2835_i2c_set_baudrate(100000);

	m_init_successfull = true;
}

bool i2cdev::set_slave_addr(char addr)
{
	if (ioctl(m_handle, I2C_SLAVE, addr) < 0)
	{
	    //printf("Failed to acquire bus access and/or talk to slave.\n");
	    /* ERROR HANDLING; you can check errno to see what went wrong */
	    return 0;
	}
	return 1;
}

bool i2cdev::read(uint8_t addr, uint8_t * data, int len, bool repeated)
{
	/*if(!set_slave_addr(addr))
		return 0;*/
	if(!repeated)
	{
		bcm2835_i2c_begin();

		struct i2c_msg msg[1];
		if(m_repeated)
		{
			m_repeated = false;
			m_work_queue.msgs[1].addr = addr;
			m_work_queue.msgs[1].len = len;
			m_work_queue.msgs[1].flags = I2C_M_RD;
			m_work_queue.msgs[1].buf = data;

			bcm2835_i2c_setSlaveAddress(m_work_queue.msgs[0].addr);
			bcm2835_i2c_write_read_rs((char *)m_work_queue.msgs[0].buf, m_work_queue.msgs[0].len, (char *)m_work_queue.msgs[1].buf, m_work_queue.msgs[1].len);
		}
		else
		{
			m_work_queue.nmsgs = 1;
			m_work_queue.msgs = msg;
			m_work_queue.msgs[0].addr = addr;
			m_work_queue.msgs[0].len = len;
			m_work_queue.msgs[0].flags = I2C_M_RD;
			m_work_queue.msgs[0].buf = data;

			bcm2835_i2c_setSlaveAddress(m_work_queue.msgs[0].addr);
			bcm2835_i2c_read((char *)m_work_queue.msgs[0].buf, m_work_queue.msgs[0].len);
		}

		/*if (ioctl(m_handle,I2C_RDWR, &m_work_queue) < 0)
		{
			 //ERROR HANDLING: i2c transaction failed
			//printf("Failed to read from the i2c bus.\n");
			return 0;
	    }*/
		return 0;
	}
	else
	{
		m_repeated = true;
		m_work_queue.nmsgs = 2;
		m_work_queue.msgs = m_msg;
		m_work_queue.msgs[0].addr = addr;
		m_work_queue.msgs[0].len = len;
		m_work_queue.msgs[0].flags = I2C_M_RD;
		m_work_queue.msgs[0].buf = temp_data;
		for(uint8_t i = 0; i < len; i++)
			temp_data[i] = data[i];
		return 0;
	}
}

bool i2cdev::write(uint8_t addr, uint8_t * data, int len, bool repeated)
{
	/*if(!set_slave_addr(addr))
			return 0;*/
	if(!repeated)
	{
		bcm2835_i2c_begin();
		struct i2c_msg msg[1];
		if(m_repeated)
		{
			m_repeated = false;
			m_work_queue.msgs[1].addr = addr;
			m_work_queue.msgs[1].len = len;
			m_work_queue.msgs[1].flags = 0;
			m_work_queue.msgs[1].buf = data;
			bcm2835_i2c_setSlaveAddress(m_work_queue.msgs[0].addr);
			bcm2835_i2c_write((char *)m_work_queue.msgs[0].buf, m_work_queue.msgs[0].len);
		}
		else
		{
			m_work_queue.nmsgs = 1;
			m_work_queue.msgs = msg;
			m_work_queue.msgs[0].addr = addr;
			m_work_queue.msgs[0].len = len;
			m_work_queue.msgs[0].flags = 0;
			m_work_queue.msgs[0].buf = data;
			bcm2835_i2c_setSlaveAddress(m_work_queue.msgs[0].addr);
			bcm2835_i2c_write((char *)m_work_queue.msgs[0].buf, m_work_queue.msgs[0].len);
		}

		/*if(ioctl(m_handle,I2C_RDWR, &m_work_queue) < 0)
		{
			//ERROR HANDLING: i2c transaction failed
			//printf("Failed to write to the i2c bus.\n");
			return 0;
		}*/
		return 0;
	}
	else
	{
		m_repeated = true;
		m_work_queue.nmsgs = 2;
		m_work_queue.msgs = m_msg;
		m_work_queue.msgs[0].addr = addr;
		m_work_queue.msgs[0].len = len;
		m_work_queue.msgs[0].flags = 0;
		m_work_queue.msgs[0].buf = temp_data;
		for(uint8_t i = 0; i < len; i++)
			temp_data[i] = data[i];
		return 0;
	}
}

bool i2cdev::write_command(uint8_t address, uint8_t command)
{
    return write(address /*<< 1*/, &command, 1, false) == 0 ? true : false;
}

bool i2cdev::write_byte_reg(uint8_t address, uint8_t reg, uint8_t data)
{
	uint8_t buf[] = {reg, data};
    return write(address/* << 1*/, buf, 2, false) == 0 ? true : false;
}

bool i2cdev::write_dword_reg(uint8_t address, uint8_t reg, uint8_t data[4])
{
	uint8_t buf[] = {reg, data[0], data[1], data[2], data[3]};
    return write(address /*<< 1*/, buf, 5, false) == 0 ? true : false;
}

bool i2cdev::write_block_reg(uint8_t address, uint8_t reg, uint8_t *data, uint16_t length)
{
	uint8_t buf[16];
    buf[0] = reg;
    for(uint8_t i = 0; i < length; i++)
        buf[i + 1] = data[i];
    return write(address /*<< 1*/, buf, length+1, false) == 0 ? true : false;
}

bool i2cdev::write_block_reg_restart(uint8_t address, uint8_t reg, uint8_t *data, uint16_t length)
{
	uint8_t buf[16];
    buf[0] = reg;
    for(uint8_t i = 0; i < length; i++)
        buf[i + 1] = data[i];
    return write(address /*<< 1*/, buf, length+1, true) == 0 ? true : false;
}

bool i2cdev::read_byte_reg(uint8_t address, uint8_t reg, uint8_t *data)
{
    return read_block_reg(address, reg, data, 1);
}

bool i2cdev::read_word_reg(uint8_t address, uint8_t reg, uint16_t *data)
{
    return read_block_reg(address, reg, (uint8_t*)data, 2);
}

bool i2cdev::read_word_reg_swapped(uint8_t address, uint8_t reg, uint16_t *data)
{
    bool error = read_block_reg(address, reg, (uint8_t*)data, 2);
    *data = ((*data & 0xff00) >> 8) | ((*data & 0x00ff) << 8);
    return error;
}

bool i2cdev::read_block_reg(uint8_t address, uint8_t reg, uint8_t *data, uint16_t length, uint32_t delay_ms)
{
    if (write_command(address, reg))
    {
        //if (delay_ms)
            //wait_ms(delay_ms);
        return read_block(address, data, length);
    }
    return false;
}

bool i2cdev::read_block(uint8_t address, uint8_t *data, uint16_t length)
{
    return read(address /*<< 1*/, data, length, false) == 0 ? true : false;
}

bool i2cdev::read_block_restart(uint8_t address, uint8_t *data, uint16_t length)
{
    return read(address /*<< 1*/, data, length, true) == 0 ? true : false;
}
