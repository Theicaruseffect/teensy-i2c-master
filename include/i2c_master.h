/* 
 * File:   i2c_master.h
 * Author: photek
 *
 * Created on 25 October 2015, 20:01
 */

#ifndef I2C_MASTER_H
#define	I2C_MASTER_H

#include "common.h"
#include "MK20D7.h"
#include "arm_cm4.h"


#ifdef	__cplusplus
extern "C" {
#endif

#define MAX_TX_DATA_BUF 255

static char masterTxData[MAX_TX_DATA_BUF];
static short masterTxSetup = 0;
static int masterTxBufIdx = 0;

/**
 * This initialises the i2c module. Should only be called once
 */
void init_i2c(void);
/**
 * This setups the transfer request for where the queued data will be sent to
 * @param address The address of the slave device communicating to
 */
void master_tx_init(const char address);
/**
 * This queues up the data to be transmitted to the addressed slave device
 * 
 * @param data
 * @return 1 on success. Error codes -1 if master_tx_init() hasn't been called
 * . -2 if the buffer (MAX_TX_DATA_BUF) is full
 */
int master_tx_write(const char data);

/**
 * This goes through the queued data and writes the data out to the slave device.
 * 
 * TODO : More error checking is needed. However this is only for a single master
 * device (so arbitration errors shouldn't happen). However, incorrect pull ups
 * might cause errors (tested using 4.7K resistors)
 * 
 * @return -1 on success, -1 if master_tx_init() hasn't been called
 */
int master_tx_start();


#ifdef	__cplusplus
}
#endif

#endif	/* I2C_MASTER_H */

