 /*
 *  This file is written using examples provided by Spectrum Digital 
 */


#include "stdio.h"
#include<string.h>
#include "usbstk5515.h"
#include "usbstk5515_gpio.h"
#include "usbstk5515_i2c.h"


#define AIC3204_I2C_ADDR 0x18
#define Rcv 0x08
#define Xmit 0x20

#ifndef TRUE
#define TRUE 1
#endif

#define freq_6_857 0xF1
#define freq_8 0xE1
#define freq_9_6 0xD1
#define freq_12 0xC1
#define freq_16 0xB1
#define freq_24 0xA1
#define freq_48 0x91
Int16 LEFT_DATA,RIGHT_DATA;
Int16 data_to_send[16] = {0,0,0,1,1,0,1,0,0,0,0,0,0,0,0,0};//00011010 is preamble, the next 8 bits are data bits  
Int16 data_count = 0;
Int16 values_sent = 0;
Int16 xmit_seq = 0; 

Int16 data_zero[96] = {
        0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
        0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
        0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
        0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
        0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
        0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000         
    }; 


Int16 data_one[192] = {
        0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff,
        0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff,
        0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff,
        0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff,
        0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff,
        0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff,
        0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff,
        0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff,
        0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff,
        0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff,
        0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff,
        0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,         
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000         
    }; 
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  Function to transmit bit 1                                              *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 send_one()
{
    Int16 ones_sent = 0;
    Int16 z = 0; 
    
    while(TRUE)
    {
    LEFT_DATA = data_one[z]; 
  	RIGHT_DATA = data_one[z];     
        
    while((Xmit & I2S0_IR) == 0);
	    I2S0_W0_MSW_W = LEFT_DATA;  // 16 bit left channel transmit audio data
	    I2S0_W1_MSW_W = RIGHT_DATA; // 16 bit right channel transmit audio data
    
    z = z + 1;
    if( z == 192 )
    {
    	z = 0; 
    	ones_sent = ones_sent + 1;
    	if(ones_sent == 6)
    	{
    		break;
    	}
    }
    }    
    return ones_sent;  
}
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  Function to transmit bit 0                                              *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 send_zero()
{
    Int16 zeroes_sent = 0;
    Int16 z = 0; 
    
    while(TRUE)
    {
    LEFT_DATA = data_zero[z]; 
  	RIGHT_DATA = data_zero[z];     
        
    while((Xmit & I2S0_IR) == 0);
	    I2S0_W0_MSW_W = LEFT_DATA;  // 16 bit left channel transmit audio data
	    I2S0_W1_MSW_W = RIGHT_DATA; // 16 bit right channel transmit audio data
    
    z = z + 1;
    if( z == 96 )
    {
    	z = 0; 
    	zeroes_sent = zeroes_sent + 1;
    	if(zeroes_sent == 6)
    	{
    		break;
    	}
    }
    }    
    return zeroes_sent;  
}
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  Function to convert the int to ascii for sending                        *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 * convert_to_ascii(Int16 int_data_to_convert) 
{
	static Int16 bin_data[8] = {0,0,0,0,0,0,0,0};
	Int16 quotient =  int_data_to_convert;
	Int16 i = 7;
	Int16 j = 0;
	
	for( j = 0; j < 8; j++)
	{
		bin_data[j] = 0;
	}		 	
	while(quotient >= 2)
	{
		bin_data[i] = (int)(quotient%2);
		quotient = (int)(quotient / 2);
		i = i-1;
	} 	
	bin_data[i] = quotient;
	
	/*
	for(j = 0; j<8; j++)
	    { 	
		printf("%d", bin_data[j]);
		}	
	*/
	return bin_data;
}	
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  Function to transmit 16 bits of data stored in data_to_send             *
 *                                                                          *
 * ------------------------------------------------------------------------ */
void transmit()
{
	Int16 i=0;
	while(data_count<16)
	{
	if( data_to_send[data_count] == 1 )
	    {
	    	values_sent = send_one();
	    	//printf("%d",data_to_send[data_count]);
	    	      for(i = 0; i<1000; i++)
    				{
	    				LEFT_DATA = xmit_seq; 
	    				RIGHT_DATA = xmit_seq;  
	    	
	    				while((Xmit & I2S0_IR) == 0);
	      				 I2S0_W0_MSW_W = LEFT_DATA;  // 16 bit left channel transmit audio data
	      				 I2S0_W1_MSW_W = RIGHT_DATA; // 16 bit right channel transmit audio data
				 }
	    	data_count = data_count + 1; 
	    	
	    }		
	    
	 if( data_to_send[data_count] == 0 )
	    {
	    	values_sent = send_zero();
	    	//printf("%d",data_to_send[data_count]);
	    	for(i = 0; i<1000; i++)
    				{
	    				LEFT_DATA = xmit_seq; 
	    				RIGHT_DATA = xmit_seq;  
	    	
	    				while((Xmit & I2S0_IR) == 0);
	      			    	 I2S0_W0_MSW_W = LEFT_DATA;  // 16 bit left channel transmit audio data
	      				 I2S0_W1_MSW_W = RIGHT_DATA; // 16 bit right channel transmit audio data
				 }
	    	data_count = data_count + 1; 
	    	
	    }	
	}
	
	data_count = 0;
}
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  Initialization functions                                                *
 *                                                                          *
 * ------------------------------------------------------------------------ */

Int16 AIC3204_rget(  Uint16 regnum, Uint16* regval )
{
    Int16 retcode = 0;
    Uint8 cmd[2];

    cmd[0] = regnum & 0x007F;       // 7-bit Register Address
    cmd[1] = 0;

    retcode |= USBSTK5515_I2C_write( AIC3204_I2C_ADDR, cmd, 1 );
    retcode |= USBSTK5515_I2C_read( AIC3204_I2C_ADDR, cmd, 1 );

    *regval = cmd[0];
    USBSTK5515_wait( 10 );
    return retcode;
}

Int16 AIC3204_rset( Uint16 regnum, Uint16 regval )
{
    Uint8 cmd[2];
    cmd[0] = regnum & 0x007F;       // 7-bit Register Address
    cmd[1] = regval;                // 8-bit Register Data

    return USBSTK5515_I2C_write( AIC3204_I2C_ADDR, cmd, 2 );
} 

void AIC3204_config(Uint8 sampling_freq)
{
	/* Configure AIC3204 */
     AIC3204_rset( 0, 0 );          // Select page 0
    AIC3204_rset( 1, 1 );          // Reset codec
    AIC3204_rset( 0, 1 );          // Select page 1
    AIC3204_rset( 1, 8 );          // Disable crude AVDD generation from DVDD
    AIC3204_rset( 2, 1 );          // Enable Analog Blocks, use LDO power
    AIC3204_rset( 0, 0 );          // Select page 0
    /* PLL and Clocks config and Power Up  */
    AIC3204_rset( 27, 0x0d );      // BCLK and WCLK is set as o/p to AIC3204(Master)
    AIC3204_rset( 28, 0x00 );      // Data ofset = 0
    AIC3204_rset( 4, 3 );          // PLL setting: PLLCLK <- MCLK, CODEC_CLKIN <-PLL CLK
    AIC3204_rset( 6, 7 );          // PLL setting: J=7
    AIC3204_rset( 7, 0x06 );       // PLL setting: HI_BYTE(D=1680)
    AIC3204_rset( 8, 0x90 );       // PLL setting: LO_BYTE(D=1680)
    AIC3204_rset( 30, 0x88 );      // For 32 bit clocks per frame in Master mode ONLY
                                   // BCLK=DAC_CLK/N =(12288000/8) = 1.536MHz = 32*fs
    AIC3204_rset( 5, sampling_freq);       // PLL setting: Power up PLL, P=1 and R=1
    AIC3204_rset( 13, 0 );         // Hi_Byte(DOSR) for DOSR = 128 decimal or 0x0080 DAC oversamppling
    AIC3204_rset( 14, 0x80 );      // Lo_Byte(DOSR) for DOSR = 128 decimal or 0x0080
    AIC3204_rset( 20, 0x80 );      // AOSR for AOSR = 128 decimal or 0x0080 for decimation filters 1 to 6
    AIC3204_rset( 11, 0x82 );      // Power up NDAC and set NDAC value to 2
    AIC3204_rset( 12, 0x87 );      // Power up MDAC and set MDAC value to 7
    AIC3204_rset( 18, 0x87 );      // Power up NADC and set NADC value to 7
    AIC3204_rset( 19, 0x82 );      // Power up MADC and set MADC value to 2
    /* DAC ROUTING and Power Up */
    AIC3204_rset(  0, 0x01 );      // Select page 1
    AIC3204_rset( 12, 0x08 );      // LDAC AFIR routed to HPL
    AIC3204_rset( 13, 0x08 );      // RDAC AFIR routed to HPR
    AIC3204_rset(  0, 0x00 );      // Select page 0
    AIC3204_rset( 64, 0x02 );      // Left vol=right vol
    AIC3204_rset( 65, 0x00 );      // Left DAC gain to 0dB VOL; Right tracks Left
    AIC3204_rset( 63, 0xd4 );      // Power up left,right data paths and set channel
    AIC3204_rset(  0, 0x01 );      // Select page 1
    AIC3204_rset( 16, 0x00 );      // Unmute HPL , 0dB gain
    AIC3204_rset( 17, 0x00 );      // Unmute HPR , 0dB gain
    AIC3204_rset(  9, 0x30 );      // Power up HPL,HPR
    AIC3204_rset(  0, 0x00 );      // Select page 0
    USBSTK5515_wait( 500 );        // Wait
    
    /* ADC ROUTING and Power Up */
    AIC3204_rset( 0, 1 );          // Select page 1
    AIC3204_rset( 0x34, 0x30 );    // STEREO 1 Jack
		                           // IN2_L to LADC_P through 40 kohm
    AIC3204_rset( 0x37, 0x30 );    // IN2_R to RADC_P through 40 kohmm
    AIC3204_rset( 0x36, 3 );       // CM_1 (common mode) to LADC_M through 40 kohm
    AIC3204_rset( 0x39, 0xc0 );    // CM_1 (common mode) to RADC_M through 40 kohm
    AIC3204_rset( 0x3b, 0 );       // MIC_PGA_L unmute
    AIC3204_rset( 0x3c, 0 );       // MIC_PGA_R unmute
    AIC3204_rset( 0, 0 );          // Select page 0
    AIC3204_rset( 0x51, 0xc0 );    // Powerup Left and Right ADC
    AIC3204_rset( 0x52, 0 );       // Unmute Left and Right ADC
    
    AIC3204_rset( 0, 0 );    
    USBSTK5515_wait( 200 );        // Wait
    /* I2S settings */
    I2S0_SRGR = 0x0;
    I2S0_CR = 0x8010;    // 16-bit word, slave, enable I2C
    I2S0_ICMR = 0x3f;    // Enable interrupts
    
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  main( )                                                                 *
 *                                                                          *
 * ------------------------------------------------------------------------ */
 main( void )
{
    Int16 i=0;
    Int16 j=0;
    Int16 c = 1, d = 1;		
    //char data[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char data[] = "abcdefghijklmnopqrstuvwxyz"; //give your message here
    //char data[] = "a";	
    int data_int_of_ascii = 0;	
    Int16 *bin_data_pointer;	

    /* Initialize BSL */      
    SYS_EXBUSSEL = 0x6100; 
    USBSTK5515_init( );
    AIC3204_config(freq_48);
    
    for(i = 0; i<10; i++)
    {
	    	LEFT_DATA = xmit_seq; 
	    	RIGHT_DATA = xmit_seq;  
	    	
	    	while((Xmit & I2S0_IR) == 0);
	        I2S0_W0_MSW_W = LEFT_DATA;  // 16 bit left channel transmit audio data
	        I2S0_W1_MSW_W = RIGHT_DATA; // 16 bit right channel transmit audio data
    }
	 
    while(TRUE)
    {	    	
	for( i = 0; i<strlen(data); i++)
	{
		data_int_of_ascii = (int)data[i]; 
		bin_data_pointer = convert_to_ascii(data_int_of_ascii);

		/* 
		(checkpoint)
		Use the bin_data_pointer to transmit the data that you want to send.
		You can use the transmit() function for this.
		*/
		
		//delay after each character transmitted to allow receiver to properly decode each character
		for ( c = 1 ; c <= 1500 ; c++ )
       			for ( d = 1 ; d <= 1500 ; d++ )
       			{}			
	}			
    }   
    /* Disble I2S */
    I2S0_CR = 0x00;
    return 0;
}
