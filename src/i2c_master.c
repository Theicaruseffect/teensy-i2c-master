#include "i2c_master.h"

void init_i2c(void)
{
        //Start clock for the i2c module
        SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;
        //Configure pins for SDA and SCL
        PORTB_PCR2 = PORT_PCR_MUX(2) | PORT_PCR_ODE_MASK | PORT_PCR_SRE_MASK | PORT_PCR_DSE_MASK;
        PORTB_PCR3 = PORT_PCR_MUX(2) | PORT_PCR_ODE_MASK | PORT_PCR_SRE_MASK | PORT_PCR_DSE_MASK;
        I2C0_F = 0x27;
        I2C0_FLT = 4; //Glitch filter. I think I need this?
}

void master_tx_init(const char address)
{
        //The lower bit must be zero, to indicate tx transfer
        masterTxData[0] = address << 1;
        masterTxSetup = 1;
        masterTxBufIdx = 1;
}

int master_tx_write(const char data)
{
        if (masterTxSetup == 0) {
                return -1; //Tx hasn't been setup
        }
        
        if (masterTxBufIdx >= MAX_TX_DATA_BUF) {
                return -2; //Buffer full
        } else {
                masterTxData[masterTxBufIdx] = data;
                masterTxBufIdx++;
        }
        
        return 1;
}

int master_tx_start()
{
        if (masterTxSetup == 0) {
                return -1; //Tx hasn't been setup
        }
        
        //Wait until the bus is free.
        while (I2C0_S & I2C_S_BUSY_MASK);
        //Bus free now, so become the bus master
        I2C0_C1 = I2C_C1_IICEN_MASK | I2C_C1_MST_MASK | I2C_C1_TX_MASK;
        
        //Wait for a start condition to issued as we're now the master
        while (!(I2C0_S & I2C_S_BUSY_MASK));
        
        for (int i = 0; i < masterTxBufIdx; i++) {
                I2C0_D = masterTxData[i];
                //The bus is now busy, due to the transfer
                while (!(I2C0_S & I2C_S_BUSY_MASK));
                //Wait until an interrupt is pending
                while (!(I2C0_S & I2C_S_IICIF_MASK));
                //Clear pending interrupt
                I2C0_S = I2C_S_IICIF_MASK;
        }

        masterTxBufIdx = 1; //Finished

        //Force the stop condition
        I2C0_C1 &= ~I2C_C1_MST_MASK;

        return 1;
}
