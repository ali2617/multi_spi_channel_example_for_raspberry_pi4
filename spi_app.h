/*!
 * \file /boot/firmware/config.txt
 * \brief Raspberry Pi4 SPI configuration.
 *
 * Raspberry Pi configuration settings in /boot/firmware/config.txt:
 * \code
 * dtparam=spi=off         ///< Disable default SPI
 * dtoverlay=spi4-1cs      ///< Enable SPI4 with 1 chip select
 * dtoverlay=spi5-1cs      ///< Enable SPI5 with 1 chip select
 * dtoverlay=spi6-1cs      ///< Enable SPI6 with 1 chip select
 * \endcode
 *
 * Ensure these settings are added to the /boot/firmware/config.txt file to correctly initialize
 * SPI for the project on Raspberry Pi4.
 */

/*!
 * \file ayf_pinconfig.sh
 * \brief Raspberry Pi4 SPI pin configuration.
 *
 * Pin control configurations for SPI and UART:
 * \code
 * #spi4_configurations
 * pinctrl set 5 a3       ///< Set pin 5 to alternate function 3 (SPI4)
 * pinctrl set 6 a3       ///< Set pin 6 to alternate function 3 (SPI4)
 * pinctrl set 7 a3       ///< Set pin 7 to alternate function 3 (SPI4)
 * 
 * #spi5_configurations
 * pinctrl set 13 a3      ///< Set pin 13 to alternate function 3 (SPI5)
 * pinctrl set 14 a3      ///< Set pin 14 to alternate function 3 (SPI5)
 * pinctrl set 15 a3      ///< Set pin 15 to alternate function 3 (SPI5)
 * 
 * #spi6_configurations
 * pinctrl set 19 a3      ///< Set pin 19 to alternate function 3 (SPI6)
 * pinctrl set 20 a3      ///< Set pin 20 to alternate function 3 (SPI6)
 * pinctrl set 21 a3      ///< Set pin 21 to alternate function 3 (SPI6)
 * \endcode
 *
 * This script should be executed to configure the necessary pins for SPI functionality.
 */
//

#ifndef SPI_APP_H
#define SPI_APP_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>

typedef enum {
    ADC_CV_0 = 1, ADC_CV_1, ADC_CV_2, ADC_CV_3, ADC_CV_4, ADC_CV_5, ADC_CV_6, ADC_CV_7,
    ADC_CV_8, ADC_CV_9, ADC_CV_10, ADC_CV_11, ADC_CV_12, ADC_CV_13, ADC_CV_14, ADC_CV_15,
    ADC_OV_0, ADC_OV_1, ADC_OV_2, ADC_OV_3, ADC_OV_4, ADC_OV_5, ADC_OV_6, ADC_OV_7,
    ADC_OV_8, ADC_OV_9, ADC_OV_10, ADC_OV_11, ADC_OV_12, ADC_OV_13, ADC_OV_14, ADC_OV_15,
    DOUT_DT, DIN_STD
} adc_register_t;

typedef enum {
    APR1 = 1, APR2, APR3, APR4, APR5, APR6, VC1, VC2, VC3, VC4, VC5, VC6,
    ACS1, ACS2, ACS3, ACS4, ACS5, ACS6, PWV1, PWV2, PWV3, PWV4, PWV5, PWV6,
    APOR1, APOR2, APOR3, APOR4, APOR5, APOR6, ENCC1, ENCC2, ENCC3, ENCC4, ENCC5, ENCC6,
    LZP1, LZP2, LZP3, LZP4, LZP5, LZP6
} enc_register_t;

typedef enum {
    Y1PWM = 1, Y2PWM, AX1PWM, AX2PWM, AX3PWM, AX4PWM, AOPWM1, AOPWM2, AOPWM3, AOPWM4,
    LDCNST1, LDCNST2, LDCNST3, LDCNST4, LDCNST5, LDCNST6, LDCNST7, LDCNST8, LDCNST9, LDCNST10,
    Y1CNT, Y2CNT, AX1CNT, AX2CNT, AX3CNT, AX4CNT, AOCNT1, AOCNT2, AOCNT3, AOCNT4,
    QPWM, PPWM, QVLDCNST, PVLDCNST, QPCNT
} cnt_register_t;

typedef enum {
    SPI_STATUS_UNINITIALIZED = 0,
    SPI_STATUS_INITIALIZED,
    SPI_STATUS_CLOSED,
    SPI_STATUS_ERROR,
    SPI_STATUS_BUSY,
    SPI_STATUS_READY,
    SPI_STATUS_INVALID,
    SPI_STATUS_MAX
} spi_status_t;

typedef struct {
    int spi_fd;
    const char *device_path;
    uint32_t speed;
    uint8_t mode;
    uint8_t bits;
    spi_status_t flag; 
} spi_device_config_t;

int spi_device_init(int device_count, ...);
int spi_transfer(spi_device_config_t* config, const uint8_t *tx_buffer, uint8_t *rx_buffer);
int spi_register_write(spi_device_config_t* config, uint16_t register_adr, const uint32_t send_data);
int spi_register_read(spi_device_config_t* config, uint16_t register_adr, uint32_t *get_data);
void spi_close(spi_device_config_t* config);
int abkant_spi_init(void);

#endif /* SPI_APP_H */
