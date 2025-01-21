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

#ifndef AYF_SPI_APP_H
#define AYF_SPI_APP_H

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

typedef struct {
    uint32_t adc_values[16];
} adc_data_t;

typedef struct {
    uint32_t abs_pos[6];
    uint32_t vel_enc[6];
    uint32_t acc_enc[6];
    uint32_t last_z_pulse_pos[6];
}enc_data_t;

typedef struct {
    uint32_t y1_pwm;
    uint32_t y2_pwm;
    uint32_t auxaxis_pwm[4];
    uint32_t analog_output_pwm[4];
    uint32_t pwm_freq_cons[10];
    uint32_t y1_cont_val;
    uint32_t y2_cont_val;
    uint32_t auxaxis_con[4];
    uint32_t analog_output_cont[4];
    uint32_t q_valve_pwm;
    uint32_t p_valve_pwm;
    uint32_t q_valve_pwm_freq_cons;
    uint32_t p_valve_pwm_freq_cons;
    uint32_t q_p_control_val;
}cnt_data_t;

typedef struct {
    uint32_t adc_ov_0;
    uint32_t adc_ov_1;
    uint32_t adc_ov_2;
    uint32_t adc_ov_3;
    uint32_t adc_ov_4;
    uint32_t adc_ov_5;
    uint32_t adc_ov_6;
    uint32_t adc_ov_7;
    uint32_t adc_ov_8;
    uint32_t adc_ov_9;
    uint32_t adc_ov_10;
    uint32_t adc_ov_11;
    uint32_t adc_ov_12;
    uint32_t adc_ov_13;
    uint32_t adc_ov_14;
    uint32_t adc_ov_15;
} adcov_interface_t;

typedef struct {
    uint32_t dout;
} doutdt_interface_t;

typedef struct {
    uint32_t din;
} dinstd_interface_t;

/////////////////////////////////////////////////////////////////////////////////

typedef struct {
    uint32_t pwv_1;
    uint32_t pwv_2;
    uint32_t pwv_3;
    uint32_t pwv_4;
    uint32_t pwv_5;
    uint32_t pwv_6;
} pwv_interface_t;

typedef struct {
    uint32_t apore_1;
    uint32_t apore_2;
    uint32_t apore_3;
    uint32_t apore_4;
    uint32_t apore_5;
    uint32_t apore_6;
} apore_interface_t;

typedef struct {
    uint32_t encc_1;
    uint32_t encc_2;
    uint32_t encc_3;
    uint32_t encc_4;
    uint32_t encc_5;
    uint32_t encc_6;
} enccon_interface_t;

/////////////////////////////////////////////////////////////////////////////////

typedef struct {
    uint32_t y1pwm_val;
} y1pwm_val_interface_t;

typedef struct {
    uint32_t y2pwm_val;
} y2pwm_val_interface_t;

typedef struct {
    uint32_t axpwm_1;
    uint32_t axpwm_2;
    uint32_t axpwm_3;
    uint32_t axpwm_4;
} axpwm_interface_t;

typedef struct {
    uint32_t aopwm_1;
    uint32_t aopwm_2;
    uint32_t aopwm_3;
    uint32_t aopwm_4;
} aopwm_interface_t;

typedef struct {
    uint32_t ldcnst_1;
    uint32_t ldcnst_2;
    uint32_t ldcnst_3;
    uint32_t ldcnst_4;
    uint32_t ldcnst_5;
    uint32_t ldcnst_6;
    uint32_t ldcnst_7;
    uint32_t ldcnst_8;
    uint32_t ldcnst_9;
    uint32_t ldcnst_10;
} ldcnst_interface_t;

typedef struct {
    uint32_t y1_cnt;
} y1cnt_interface_t;

typedef struct {
    uint32_t y2_cnt;
} y2cnt_interface_t;

typedef struct {
    uint32_t axcnt_1;
    uint32_t axcnt_2;
    uint32_t axcnt_3;
    uint32_t axcnt_4;
} axcnt_interface_t;

typedef struct {
    uint32_t aocnt_1;
    uint32_t aocnt_2;
    uint32_t aocnt_3;
    uint32_t aocnt_4;  
} aocnt_interface_t;

typedef struct {
    uint32_t q_pwm;
} qpwm_interface_t;

typedef struct {
    uint32_t p_pwm;
} ppwm_interface_t;

typedef struct {
    uint32_t q_vldcnst;
} qvldcnst_interface_t;

typedef struct {
    uint32_t p_vldcnst;
} pvldcnst_interface_t;

typedef struct {
    uint32_t qp_cnt;
} qpcnt_interface_t;

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

int read_adc_cv_registers(spi_device_config_t* config, adc_data_t* device_data);
int write_adc_offset_values(spi_device_config_t* config, adcov_interface_t* adc_offsets);
int write_data_to_digital_output(spi_device_config_t* config, doutdt_interface_t* dout_value);
int write_data_to_digital_input(spi_device_config_t* config, dinstd_interface_t* din_value);

int read_absolute_position_encoder(spi_device_config_t* config, enc_data_t* enc_data);
int read_velocity_of_encoder(spi_device_config_t* config, enc_data_t* enc_data);
int read_acceleration_of_encoder(spi_device_config_t* config, enc_data_t* enc_data);
int read_last_z_pulse_position(spi_device_config_t* config, enc_data_t* enc_data);
int write_pulse_with_value_encoder(spi_device_config_t* config, pwv_interface_t* pwv);
int write_absolute_position_ofset_encoder(spi_device_config_t* config, apore_interface_t* apore) ;
int write_encoder_control(spi_device_config_t* config, enccon_interface_t* encc);

int read_y1_pwm_value(spi_device_config_t* config, cnt_data_t* cnt_data);
int read_y2_pwm_value(spi_device_config_t* config, cnt_data_t* cnt_data);
int read_auxiliary_axis_pwm(spi_device_config_t* config, cnt_data_t* cnt_data);
int read_analog_output_pwm(spi_device_config_t* config, cnt_data_t* cnt_data);
int read_pwm_frequency_constant(spi_device_config_t* config, cnt_data_t* cnt_data);
int read_y1_control(spi_device_config_t* config, cnt_data_t* cnt_data);
int read_y2_control(spi_device_config_t* config, cnt_data_t* cnt_data);
int read_auxiliary_axis_control(spi_device_config_t* config, cnt_data_t* cnt_data);
int read_analog_output_control(spi_device_config_t* config, cnt_data_t* cnt_data);
int read_q_valve_pwm(spi_device_config_t* config, cnt_data_t* cnt_data);
int read_p_valve_pwm(spi_device_config_t* config, cnt_data_t* cnt_data) ;
int read_q_valve_pwm_frequency_constant(spi_device_config_t* config, cnt_data_t* cnt_data);
int read_p_valve_pwm_frequency_constant(spi_device_config_t* config, cnt_data_t* cnt_data);
int read_q_and_p_control_register(spi_device_config_t* config, cnt_data_t* cnt_data);
int write_y1_pwm_value(spi_device_config_t* config, y1pwm_val_interface_t* y1pwm);
int write_y2_pwm_value(spi_device_config_t* config, y2pwm_val_interface_t* y2pwm);
int write_auxiliary_axis_pwm(spi_device_config_t* config, axpwm_interface_t* axpwm);
int write_analog_output_pwm(spi_device_config_t* config, aopwm_interface_t* aopwm);
int write_pwm_frequency_constant(spi_device_config_t* config, ldcnst_interface_t* ldcnst);
int write_y1_control(spi_device_config_t* config, y1cnt_interface_t* y1cnt);
int write_y2_control(spi_device_config_t* config, y2cnt_interface_t* y2cnt);
int write_auxiliary_axis_control(spi_device_config_t* config, axcnt_interface_t* axcnt);
int write_analog_output_control(spi_device_config_t* config, aocnt_interface_t* aocnt);
int write_q_valve_pwm(spi_device_config_t* config, qpwm_interface_t* qpwm);
int write_p_valve_pwm(spi_device_config_t* config, ppwm_interface_t* ppwm);
int write_q_valve_pwm_frequency_constant(spi_device_config_t* config, qvldcnst_interface_t* qvldcnst);
int write_p_valve_pwm_frequency_constant(spi_device_config_t* config, pvldcnst_interface_t* pvldcnst);
int write_q_and_p_control_register(spi_device_config_t* config, qpcnt_interface_t* qpcnt);

#endif /* AYF_SPI_APP_H */