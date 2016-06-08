/*
 * i2cdev.h
 *
 * Copyright (C) 2014 William Markezana <william.markezana@me.com>
 *
 */

#pragma once

#include <iostream>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <linux/types.h>

using namespace std;

/*
 * public defines
 *
 */

/*
 * public types
 *
 */

/*
 * public class
 *
 */
class i2cdev
{
private:
    //uint8_t m_address;
    bool set_slave_addr(char addr);
    bool read(uint8_t addr, uint8_t * data,int len, bool repeated);
    bool write(uint8_t addr, uint8_t * data, int len, bool repeated);
    bool m_repeated;
    struct i2c_rdwr_ioctl_data m_work_queue;
    struct i2c_msg m_msg[2];
    uint8_t temp_data[6];

protected:
    const string m_device = "/dev/i2c-1";
    bool m_init_successfull;
    bool m_running;
    int m_handle;
    int m_mode;

public:
    i2cdev();
    void configure();

    bool write_command(uint8_t address, uint8_t command);
    bool write_byte_reg(uint8_t address, uint8_t reg, uint8_t data);
    bool write_dword_reg(uint8_t address, uint8_t reg, uint8_t data[4]);
    bool write_block_reg(uint8_t address, uint8_t reg, uint8_t *data, uint16_t length);
    bool write_block_reg_restart(uint8_t address, uint8_t reg, uint8_t *data, uint16_t length);
    bool read_byte_reg(uint8_t address, uint8_t reg, uint8_t *data);
    bool read_word_reg(uint8_t address, uint8_t reg, uint16_t *data);
    bool read_word_reg_swapped(uint8_t address, uint8_t reg, uint16_t *data);
    bool read_block_reg(uint8_t address, uint8_t reg, uint8_t *data, uint16_t length,uint32_t delay_ms = 0);
    bool read_block(uint8_t address,uint8_t *data, uint16_t length);
    bool read_block_restart(uint8_t address, uint8_t *data, uint16_t length);
};
