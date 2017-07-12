 /*
 *  This file is written using examples provided by Spectrum Digital
 */


//#include "stdio.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
Int16 data_received[8]={0};
Int16 header[8] = {0,0,0,1,1,0,1,0}; //header content of data packet
Int16 data_to_write_int = 0;
int data_cap_pointer =0;
int ascii_pointer =0;
Int16 data_count = 0;
Int16 data_to_send[16] = {0,0,0,1,1,0,1,0,0,0,0,0,0,0,0,0};
Int16 values_sent = 0;
Int16 xmit_seq = 0;

Int16 data_captured[1024];
Int16 data_ascii[1024];

Int16 data_next=0;
Int16 no_match_count=1;

char single;

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
 *  Testing Function                                                        *
 *                                                                          *
 * ------------------------------------------------------------------------ */
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
 *  Function to detect bit 1                                                *
 *                                                                          *
 * ------------------------------------------------------------------------ */

 Int16 detect_one()
{
    Int16 count_of_one = 0;
    Int16 data_count = 1;

    while(TRUE)
    {
    while((Rcv & I2S0_IR) == 0);  // Wait for interrupt pending flag
    LEFT_DATA = I2S0_W0_MSW_R;  // 16 bit left channel received audio data
    RIGHT_DATA = I2S0_W1_MSW_R;  // 16 bit right channel received audio data

    data_count = data_count + 1;


    if( data_count == 24 || data_count == 120 || data_count == 216 || data_count == 312 || data_count == 408 || data_count == 504 )
    {
        if( LEFT_DATA > 3000 )
        {
            count_of_one = count_of_one + 1;
        }
    }

    if( data_count > 504)// && LEFT_DATA < 500 )
    {
        break;
    }
    }
    return count_of_one;
}
/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  Function to detect bit 0                                                *
 *                                                                          *
 * ------------------------------------------------------------------------ */

Int16 detect_zero()
{
    /*
    Paste your code in bit_receiver.c to detect zero here
    */
    Int16 count_of_zero = 0;
    Int16 data_count = 1;

    while(TRUE)
    {
    while((Rcv & I2S0_IR) == 0);  // Wait for interrupt pending flag
    LEFT_DATA = I2S0_W0_MSW_R;  // 16 bit left channel received audio data
    RIGHT_DATA = I2S0_W1_MSW_R;  // 16 bit right channel received audio data

    data_count = data_count + 1;

    //These values of data_count correspond to the mid values of peaks of RZ waveforms
    //The original waveform was of 192 samples, hence its peak was in the first 96 samples
    //96/2=48 , 48 + 192 = 240, ...
    if( data_count == 48 || data_count == 240 || data_count == 432 || data_count == 624 || data_count == 816 || data_count == 1008 )
    {
        if( LEFT_DATA < -3000 )
        {
            count_of_zero = count_of_zero + 1;
        }
    }

    if( data_count > 1008)// && LEFT_DATA < 500 )//break out of loop to enable to detect the new bit
    {
        break;
    }
    }
    return count_of_zero;

}
 /* ------------------------------------------------------------------------ *
 *                                                                          *
 *  This function helps in creating a sync between transmitter and receiver *
 *  It checks for the preamble match after each received bit                *
 *  Once it finds a match it will set data_next = 1 indicating next 8 bits  *
 *  will be my data                                                         *
 *                                                                          *
 * ------------------------------------------------------------------------ */
void check_pattern()
{
    Int16 preamble_arr[8];
        Int16 j = 0;
        Int16 no_match_local;
        Int16 loc_var=0;
        no_match_local = no_match_count;

        for(loc_var=0,j=no_match_count+1;loc_var<8;loc_var++,j++)
        {
            if(j>7){
                preamble_arr[loc_var]=data_received[j-8];
            }
            else{
                preamble_arr[loc_var]=data_received[j];
            }
        }

        for(j=0;j<8;j++){
            if(preamble_arr[j]!=header[j])
            {
                no_match_count++;
                data_next=0;
                break;
            }
        }

        if(no_match_local==no_match_count){
            data_next=1;
            no_match_count=0;
        }
}

int binary_to_integer()
{
    int i = 0;
    long int tempc =0;
    int pow2=128;
     for(i=0;i<8;i++)
    {
        tempc = tempc + (pow2*data_received[i]);
        pow2/=2;
    }
    int tempo=tempc;
    //printf("Tempo = %d\n",tempo);
    return tempo;
}

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

int file_read(char* s)
{
    FILE *fp;
    int i = 0;
    fp = fopen("C:\\Users\\dell\\Desktop\\DSP_LAB\\Transmit_data.txt","r");
    s[i] = fgetc(fp);
    while (s[i] != EOF)
        {
            i = i + 1;
            s[i] = fgetc(fp);
        }
    fclose(fp);
    return s[0];
}

void write_file(char* s)
{
    FILE *fp;
    int i;
    fp = fopen("C:\\Users\\dell\\Desktop\\DSP_LAB\\Transmit_data.txt","w");
    fprintf(fp,"%c",'0');
    for(i = 1; i<100; i++)
        fprintf(fp,"%c",s[i]);
    fclose(fp);
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  main( )                                                                 *
 *                                                                          *
 * ------------------------------------------------------------------------ */
 main( void )
{
     setvbuf(stdout, NULL, _IONBF, 0);
     char s[100];
     int num = 0;
     int data_int_of_ascii = 0;
     Int16 *bin_data_pointer;
    //In this we will read the stream of bits being received. After each bit has been detected, i.e.
    //after each input to our data_received[] buffer, we will check whether the preamble has been detected
    //in the data stream or not. If it has been detected, then we will treat next 8 bits as data, convert the binary ASCII to
    //character and print it on the console. We will keep searching the data stream till a preamble is detected.
    //For detecting the preamble you will surely need a memory of atleast 8 bits. Try to make use of as less memory space as possible
    //and implement this algorithm in the receiver.

    Int16 i=0;
    Int16 j=0;
    Int16 k=0;
    Int16 c = 1, d = 1;
    Int16 count=0;
    data_cap_pointer = 0;
    ascii_pointer = 0;


    /* Initialize BSL */
    SYS_EXBUSSEL = 0x6100;
    USBSTK5515_init( );
    AIC3204_config(freq_48);

    for(i = 0; i<10; i++)
         {
                LEFT_DATA = xmit_seq;
                RIGHT_DATA = xmit_seq;

                //while((Xmit & I2S0_IR) == 0);
                I2S0_W0_MSW_W = LEFT_DATA;  // 16 bit left channel transmit audio data
                I2S0_W1_MSW_W = RIGHT_DATA; // 16 bit right channel transmit audio data
         }

    for(j=0; j<8; j++)
    {
        data_received[j] = 5;
    }
    for(j=0; j<1024; j++)
    {
        data_captured[j] = 5;
    }

    while(TRUE)
    {
        //printf("Hello\n");
        if(file_read(s) == '1')         // This implies that the changed file has been read
        {
            num = 0;
            while(s[num] != '0')
                num++;
            //call the send function here
            puts("Sending...");
            for(k = 1 ; k < num ; k++)
            {
                data_int_of_ascii = (int)s[k];
                bin_data_pointer = convert_to_ascii(data_int_of_ascii);
                printf("%c",data_int_of_ascii);

                //puts("I am  here");
                    /*
                    (checkpoint)
                    Use the bin_data_pointer to transmit the data that you want to send.
                    You can use the transmit() function for this.
                    */
                data_to_send[8] = bin_data_pointer[0];
                data_to_send[9] = bin_data_pointer[1];
                data_to_send[10] = bin_data_pointer[2];
                data_to_send[11] = bin_data_pointer[3];
                data_to_send[12] = bin_data_pointer[4];
                data_to_send[13] = bin_data_pointer[5];
                data_to_send[14] = bin_data_pointer[6];
                data_to_send[15] = bin_data_pointer[7];
                transmit();
                //delay after each character transmitted to allow receiver to properly decode each character
                for ( c = 1 ; c <= 1500 ; c++ )
                    for ( d = 1 ; d <= 1500 ; d++ )
                        {}
            }
            printf("\n");
            // Write to file
            s[0] = 1;
            num = 0;
            write_file(s);
        }
        while((Rcv & I2S0_IR) == 0);  // Wait for interrupt pending flag
        LEFT_DATA = I2S0_W0_MSW_R;  // 16 bit left channel received audio data
        RIGHT_DATA = I2S0_W1_MSW_R;  // 16 bit right channel received audio data
        if( LEFT_DATA > 3000 )
        {
            count = detect_one();
            if( count > 3 )
            {
                data_received[i] = 0;
                data_captured[data_cap_pointer] = data_received[i];
                data_cap_pointer = ((data_cap_pointer + 1)%1024);
                //printf("bit %d: %d\n", i, data_received[i]);
                i = i + 1;
                //1 bit received now check pattern
                // (checkpoint)
                // Check for the data and once data comes, extract and display it.
                if(data_next==1)
                {
                    data_ascii[ascii_pointer]=data_received[i];
                    ascii_pointer = ((ascii_pointer + 1)%1024);
                    if(i==8)
                    {
                        data_next=0;
                        single = binary_to_integer();
                        printf("%c",single);
                        /*for(temp=0;temp<8;temp++)
                        {
                            printf("%d",data_received[temp]);
                        }
                        printf("\n");*/
                    }
                }
                else
                {
                    check_pattern();
                    if(data_next==1)
                        i=0;
                }
            }
        }

        if( LEFT_DATA < -3000 )
        {
            count = detect_zero();
            if( count > 3 )
            {
                data_received[i] = 1;
                data_captured[data_cap_pointer] = data_received[i];
                data_cap_pointer = ((data_cap_pointer + 1)%1024);
                //printf("bit %d: %d\n", i, data_received[i]);
                //printf("%d", data_received[i]);
                i = i + 1;
                //1 bit received now check pattern
                // (checkpoint)
                // Check for the data and once data comes, extract and display it.
                if(data_next==1)
                {
                    data_ascii[ascii_pointer]=data_received[i];
                    ascii_pointer = ((ascii_pointer + 1)%1024);
                    if(i==8)
                    {
                        data_next=0;
                        single = binary_to_integer();
                        printf("%c\n",single);
                        /*for(temp=0;temp<8;temp++)
                        {
                            printf("%d",data_received[temp]);
                        }
                        */
                    }
                }
                else
                {
                    check_pattern();
                    if(data_next==1)
                        i=0;
                }

           }
       }
       if(i==8)
           i=0;
    }

    /* Disble I2S */

    I2S0_CR = 0x00;
    return 0;
}
