#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* mraa header */
#include "mraa/i2c.h"

#define MAX_BUFFER_LENGTH 6
#define HMC5883L_I2C_ADDR 0x1E

/* configuration registers */
#define HMC5883L_CONF_REG_A 0x00
#define HMC5883L_CONF_REG_B 0x01

/* mode register */
#define HMC5883L_MODE_REG 0x02

/* data register */
#define HMC5883L_X_MSB_REG 0
#define HMC5883L_X_LSB_REG 1
#define HMC5883L_Z_MSB_REG 2
#define HMC5883L_Z_LSB_REG 3
#define HMC5883L_Y_MSB_REG 4
#define HMC5883L_Y_LSB_REG 5
#define DATA_REG_SIZE 6

/* status register */
#define HMC5883L_STATUS_REG 0x09

/* ID registers */
#define HMC5883L_ID_A_REG 0x0A
#define HMC5883L_ID_B_REG 0x0B
#define HMC5883L_ID_C_REG 0x0C

#define HMC5883L_CONT_MODE 0x00
#define HMC5883L_DATA_REG 0x03

/* scales */
#define GA_0_88_REG 0x00 << 5
#define GA_1_3_REG 0x01 << 5
#define GA_1_9_REG 0x02 << 5
#define GA_2_5_REG 0x03 << 5
#define GA_4_0_REG 0x04 << 5
#define GA_4_7_REG 0x05 << 5
#define GA_5_6_REG 0x06 << 5
#define GA_8_1_REG 0x07 << 5

/* digital resolutions */
#define SCALE_0_73_MG 0.73
#define SCALE_0_92_MG 0.92
#define SCALE_1_22_MG 1.22
#define SCALE_1_52_MG 1.52
#define SCALE_2_27_MG 2.27
#define SCALE_2_56_MG 2.56
#define SCALE_3_03_MG 3.03
#define SCALE_4_35_MG 4.35

#define I2C_BUS 6

volatile sig_atomic_t flag = 1;

void
sig_handler(int signum)
{
    if (signum == SIGINT) {
        fprintf(stdout, "Exiting...\n");
        flag = 0;
    }
}

int
main(void)
{
    mraa_result_t status = MRAA_SUCCESS;
    mraa_i2c_context i2c;
    float direction = 0;
    int16_t x = 0, y = 0, z = 0;
    uint8_t rx_tx_buf[MAX_BUFFER_LENGTH];

    /* install signal handler */
    signal(SIGINT, sig_handler);

    /* initialize mraa for the platform (not needed most of the times) */
    //mraa_init();

    //! [Interesting]
    i2c = mraa_i2c_init(I2C_BUS);
    if (i2c == NULL) {
        fprintf(stderr, "Failed to initialize I2C\n");
        mraa_deinit();
        return EXIT_FAILURE;
    }

    /* set slave address */
    status = mraa_i2c_address(i2c, 0x3C);
    if (status != MRAA_SUCCESS) {
        goto err_exit;
    }

    /* set device gain */
    rx_tx_buf[0] = HMC5883L_CONF_REG_B;
    rx_tx_buf[1] = GA_1_3_REG;

    status = mraa_i2c_write(i2c, rx_tx_buf, 2);
    if (status != MRAA_SUCCESS) {
        goto err_exit;
    }

    int a = mraa_i2c_read_byte(i2c);
    printf("%d\n", a);
    status = mraa_i2c_address(i2c, HMC5883L_I2C_ADDR);
    if (status != MRAA_SUCCESS) {
        goto err_exit;
    }
    mraa_i2c_stop(i2c);

    //! [Interesting]
    /* deinitialize mraa for the platform (not needed most of the times) */
    mraa_deinit();

    return EXIT_SUCCESS;

err_exit:
    mraa_result_print(status);

    /* stop i2c */
    mraa_i2c_stop(i2c);

    /* deinitialize mraa for the platform (not needed most of the times) */
    mraa_deinit();

    return EXIT_FAILURE;
}