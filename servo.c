#include <errno.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mraa/pwm.h"

int main(int argc, char **argv) {
	mraa_pwm_context pwm;
  pwm = mraa_pwm_init(3);
  if (pwm == NULL) {
    return 1;
  }
  mraa_pwm_period_ms(pwm, 20);
  mraa_pwm_pulsewidth_us(pwm, 1500);
  mraa_pwm_enable(pwm, 1);

  struct pollfd pfd[1];
  pfd[0].fd = 0;
  pfd[0].events = POLLIN | POLLERR;

  char buffer[10];
  float output;
  while (1) {
    int ret_poll = poll(pfd, 1, 0);
    if (ret_poll == -1) {
      fprintf(stderr, "poll() failed: %s\n", strerror(errno));
      mraa_pwm_close(pwm);
      exit(1);
    } else {
      if (ret_poll == 1) {
        if (pfd[0].revents & POLLIN) {
          bzero(buffer, 10);
          fgets(buffer, 10, stdin);
          if (!strcmp(buffer, "OFF\n")) {
            mraa_pwm_close(pwm);
            exit(0);
          } else {
            mraa_pwm_pulsewidth_us(pwm, atoi(buffer));
            output = mraa_pwm_read(pwm);
            printf("PWM value is %f\n", output);
            sleep(1);
          }
        }
        if (pfd[0].revents & POLLERR) {
          fprintf(stderr, "read() failed: %s\n", strerror(errno));
          mraa_pwm_close(pwm);
          exit(1);
        }
      }
    }
  }

  /*mraa_pwm_pulsewidth_us(pwm, 1000);
  output = mraa_pwm_read(pwm);
  printf("PWM value 2 is %f\n", output);  
  sleep(5);
  mraa_pwm_pulsewidth_us(pwm, 1500);
  output = mraa_pwm_read(pwm);
  printf("PWM value is %f\n", output);*/
  

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