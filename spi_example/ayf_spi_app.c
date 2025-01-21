#include <linux/spi/spidev.h>
#include "ayf_spi_app.h"

int spi_device_init(int device_count, ...) {
    va_list args;
    va_start(args, device_count);

    for (int i = 0; i < device_count; i++) 
    {
        spi_device_config_t* config = va_arg(args, spi_device_config_t*);
        
        config->spi_fd = open(config->device_path, O_RDWR);
        if (config->spi_fd < 0) 
        {
            return -1;
            //exit_sys("Fatal ERR 1234");
        }
        if (ioctl(config->spi_fd , SPI_IOC_WR_MODE, (void*)&config->mode) < 0)
        {
            close(config->spi_fd);
            return -1;
            //exit_sys("Fatal ERR 1234");
        }   
        if (ioctl(config->spi_fd, SPI_IOC_WR_BITS_PER_WORD, (void*)&config->bits) < 0) 
        {
            close(config->spi_fd);
            return -1;
            //exit_sys("Fatal ERR 1234")
        }
        if (ioctl(config->spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, (void*)&config->speed) < 0) 
        {
            close(config->spi_fd);
            return -1;
            //exit_sys("Fatal ERR 1234");
        }
        
        config->flag = SPI_STATUS_INITIALIZED;
        printf("Cihaz %d:\n", i + 1);
        printf(" SPI FD: %d\n", config->spi_fd + 1);
        printf(" Device Path: %s\n", config->device_path);
        printf(" Speed: %u Hz\n", config->speed);
        printf(" Mode: %u\n", config->mode);
        printf(" Bits per Word: %u\n", config->bits);
        printf(" SPI Flag: %d\n", config->flag);
        printf("\n");
    }
    va_end(args);
    return 0;
}

int 
spi_transfer(spi_device_config_t* config, const uint8_t *tx_buffer, uint8_t *rx_buffer) 
{
    struct spi_ioc_transfer spi_transfer;
    memset(&spi_transfer, 0, sizeof(spi_transfer));
    spi_transfer.tx_buf = (unsigned long)(uintptr_t)tx_buffer;
    spi_transfer.rx_buf = (unsigned long)(uintptr_t)rx_buffer;
    spi_transfer.len = 6;
    spi_transfer.delay_usecs = 0;

    int ret = ioctl(config->spi_fd, SPI_IOC_MESSAGE(1), &spi_transfer);
    if (ret < 0) 
    {
        printf("SPI: Unable to transfer\n");
    }
    return ret;
}

int 
spi_register_write(spi_device_config_t* config, uint16_t register_adr, const uint32_t send_data) 
{
    uint8_t tx_buffer[6], rx_buffer[6];
    register_adr = (register_adr << 7) | 0x8000;
    tx_buffer[0] = register_adr >> 8;
    tx_buffer[1] = (uint8_t)register_adr ;  
    tx_buffer[2] = (uint8_t)(send_data >> 24);
    tx_buffer[3] = (uint8_t)(send_data >> 16);
    tx_buffer[4] = (uint8_t)(send_data >> 8);
    tx_buffer[5] = (uint8_t)send_data;

    spi_transfer(config, tx_buffer, rx_buffer);
    return 0;
}

int 
spi_register_read(spi_device_config_t* config, uint16_t register_adr, uint32_t *get_data) 
{
    uint8_t tx_buffer[6], rx_buffer[6];
    register_adr = (register_adr << 7);
    tx_buffer[0] = register_adr >> 8; 
    tx_buffer[1] = (uint8_t)register_adr;  
    tx_buffer[2] = 0;
    tx_buffer[3] = 0;
    tx_buffer[4] = 0;
    tx_buffer[5] = 0;

    spi_transfer(config, tx_buffer, rx_buffer);

    *get_data = (rx_buffer[2] << 24) | (rx_buffer[3] << 16) | (rx_buffer[4] << 8) | rx_buffer[5];
    return 0;
}

void spi_close(spi_device_config_t* config) {
    close(config->spi_fd);
    config->flag = SPI_STATUS_CLOSED;
}
spi_device_config_t adc_dev_spi = {-1, "/dev/spidev4.0", 20000000, SPI_MODE_0, 8};
spi_device_config_t enc_dev_spi = {-1, "/dev/spidev5.0", 20000000, SPI_MODE_0, 8};
spi_device_config_t cnt_dev_spi = {-1, "/dev/spidev6.0", 20000000, SPI_MODE_0, 8};

int abkant_spi_init(void){
    return spi_device_init(3, &adc_dev_spi, &enc_dev_spi, &cnt_dev_spi);
}

