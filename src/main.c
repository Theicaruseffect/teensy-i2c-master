#include "i2c_master.h"

#define LED_ON  GPIOC_PSOR = (1 << 5)
#define LED_OFF GPIOC_PCOR = (1 << 5)


/**
 * This simple application is to test the Teensy's i2c master, by sending
 * data to the mcp23017. The mcp23017 port expander is configured to have its
 * address set to 0x20 (all A lines on the expander are grounded.
 * 
 * @return 0 If this happens, then something horrible has made this happen. 
 * Maybe a black hole has caused this?...
 */
int main(void)
{
        PORTC_PCR5 = PORT_PCR_MUX(0x1); // LED is on PC5 (pin 13), config as GPIO (alt = 1)
        GPIOC_PDDR = (1<<5);   	// make this an output pin pin 13

        init_i2c();
        
        //ADDRESS 32 PORT A
        master_tx_init(0x20);    
        master_tx_write(0x00);
        master_tx_write(0x00);
        master_tx_start();

        //ADDRESS 32 PORT B
        master_tx_init(0x20);    
        master_tx_write(0x01);
        master_tx_write(0x00);
        master_tx_start();

        char portAData = 0x01;
        char portBData = 0x01;

        //This simply cycles through the MCP23017's ports
        while (1)
        {
                //Address port A 
                master_tx_init(0x20);    
                master_tx_write(0x12);
                master_tx_write(portAData);		
                master_tx_start();
                portAData = portAData << 1;
                //Address port B
                master_tx_init(0x20);    
                master_tx_write(0x13);
                master_tx_write(portBData);		
                master_tx_start();
                portBData = portBData << 1;
                
                //Reached the end? restart
                if (portAData & 0x80) {
                        portAData = 0x01;
                }
                
                if (portBData & 0x80) {
                        portBData = 0x01;
                }
                //Give some delay
                for (int i =0; i < 1000000; i++);
        }
        return  0;
}
