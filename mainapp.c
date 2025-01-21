/*
    Derleme örneği
    gcc mainapp.c ayf_spi_app.c -o blabla
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include "ayf_spi_app.h"

#define SPI_MODE_0 1

extern spi_device_config_t adc_dev_spi;
extern spi_device_config_t enc_dev_spi;
extern spi_device_config_t cnt_dev_spi;

int main() 
{
    uint32_t get_data ;
    abkant_spi_init();
    spi_register_read(&adc_dev_spi, 0 , &get_data);
    printf("ADC0: %x" , get_data);
    spi_register_read(&enc_dev_spi, 0 , &get_data);
    printf("ENC0: %x" , get_data);
    spi_register_read(&cnt_dev_spi, 0 , &get_data);
    printf("CNT0: %x" , get_data);
    spi_register_write(&enc_dev_spi, 33 , 0x009A0AF3);
    
    return 0;
}