#include <stdlib.h>
#include <unistd.h>
#include "mraa/pwm.h"

int main(int argc, char **argv) {
	mraa_pwm_context pwm;
  pwm = mraa_pwm_init(3);
  if (pwm == NULL) {
    return 1;
  }
  mraa_pwm_period_ms(pwm, 20);
  mraa_pwm_pulsewidth_us(pwm, 1000);
  mraa_pwm_enable(pwm, 1);
  float output = mraa_pwm_read(pwm);
  printf("PWM value 1 is %f\n", output);
  sleep(5);
  mraa_pwm_pulsewidth_us(pwm, 2000);
  output = mraa_pwm_read(pwm);
  printf("PWM value 2 is %f\n", output);  
  sleep(5);
  mraa_pwm_pulsewidth_us(pwm, 1500);
  output = mraa_pwm_read(pwm);
  printf("PWM value is %f\n", output);
  

  mraa_pwm_close(pwm);

  /*while (1) {
    value = value + 0.01f;
    mraa_pwm_write(pwm, value);
    usleep(500000);
    if (value >= 1.0f) {
      return 0;
    }
    float output = mraa_pwm_read(pwm);
    printf("PWM value is %f\n", output);
  }*/
  return 0;
}