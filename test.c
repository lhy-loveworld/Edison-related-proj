#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <unistd.h>
#include <math.h>
//#include "mraa/gpio.h"
//#include "mraa/aio.h"
//mraa_gpio_context ch1_context, ch2_context, ch3_context, ch4_context;

int main(int argc, char *argv[]) {

/*  ch1_context = mraa_gpio_init(3);
  ch2_context = mraa_gpio_init(4);
  ch3_context = mraa_gpio_init(7);
  ch4_context = mraa_gpio_init(8);*/

  struct timeval tp1, tp2;
  gettimeofday(&tp1, NULL);
  //int a = tp.tv_usec;
  //printf("%d\n", tp.tv_usec);
  int ch1 = 2;//mraa_gpio_read(ch1_context);
  /*
  int ch2 = mraa_gpio_read(ch2_context);
  int ch3 = mraa_gpio_read(ch3_context);
  int ch4 = mraa_gpio_read(ch4_context);
*/
  gettimeofday(&tp2, NULL);

  printf("%d\n", tp2.tv_usec - tp1.tv_usec);

  return 0;
}