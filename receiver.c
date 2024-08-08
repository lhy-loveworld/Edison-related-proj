//Arthor: Hongyang Li
//This is for detecting the signal output of the receiver.

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
#include "mraa/gpio.h"
#include "mraa/aio.h"
/*
int num = 0;
int sample = 0;
int last_up = 0;
int last_loop = 0;
int period = 0;
int high = 0;
int eve_loop = 0;
*/

volatile sig_atomic_t sample_end = 0;
volatile sig_atomic_t ch1_num = 0;
volatile sig_atomic_t ch2_num = 0;
volatile sig_atomic_t ch3_num = 0;
volatile sig_atomic_t ch4_num = 0;
volatile sig_atomic_t low_num = 0;
volatile sig_atomic_t send_ready = 0;
volatile sig_atomic_t send_miss = 0;

//mraa_aio_context rec_a;
mraa_gpio_context ch1_context, ch2_context, ch3_context, ch4_context;

void Shutdown(int stat) {
	//mraa_aio_close(tmp);
  mraa_gpio_close(ch1_context);
  mraa_gpio_close(ch2_context);
  mraa_gpio_close(ch3_context);
  mraa_gpio_close(ch4_context);
  exit(stat);
}

void catch_alarm(int sig) {

  //struct timeval tp1, tp2;
  //gettimeofday(&tp1, NULL);
  //long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
  //printf("%d, %d, %d, %d, %d\n", ch1_num , ch2_num, ch3_num, ch4_num, low_num);
  //e++;
  int ch1 = mraa_gpio_read(ch1_context);
  int ch2 = mraa_gpio_read(ch2_context);
  int ch3 = mraa_gpio_read(ch3_context);
  int ch4 = mraa_gpio_read(ch4_context);

  if (ch1 < 0 || ch2 < 0 || ch3 < 0 || ch4 < 0) {
    fprintf(stderr, "mraa_gpio_read() failed: %s\n", strerror(errno));
    Shutdown(1);
  }

  if (sample_end) {
    if(ch1 | ch2 | ch3 | ch4) {
      sample_end = 0;
      low_num = 0;
      ch1_num = 0;
      ch2_num = 0;
      ch3_num = 0;
      ch4_num = 0;
      if(ch1) ch1_num++;
      else if(ch2) ch2_num++;
      else if(ch3) ch3_num++;
      else ch4_num++;
    }
    //ualarm (20, 0);
  } else {
    if (ch1) {
      ch1_num++;
      low_num = 0;
      //ualarm (20, 0);
    } else if (ch2) {
      ch2_num++;
      low_num = 0;
      //ualarm (20, 0);
    } else if (ch3) {
      ch3_num++;
      low_num = 0;
     //ualarm (20, 0);
    } else if (ch4) {
      ch4_num++;
      low_num = 0;
      //ualarm (20, 0);
    } else {
      low_num++;
      if (low_num == 10) {
        if (ch1_num > 20 && ch2_num > 20 && ch3_num > 20 && ch4_num > 20)
          send_ready = 1;
        else
          send_miss = 1;
        sample_end = 1;
      }/* else {
        ualarm (20, 0);
      }*/
    }
  }

/*
  if (last_loop > tp.tv_usec) {
    printf("%d, %d, %d\n", eve_loop, last_loop, tp.tv_usec);
    eve_loop = 0;
  }
  else {
    eve_loop = tp.tv_usec - last_loop;
  }
  last_loop = tp.tv_usec;
  //printf("%d\n", tp.tv_usec);

  if (sample != a) {
    if (sample == 1) {
      if (last_up > tp.tv_usec) high += tp.tv_usec + 1000000 - last_up;
      else high += tp.tv_usec - last_up;
    } else {
      //printf("%d, %d\n", high, period);
      if (last_up > tp.tv_usec) {
        period += tp.tv_usec + 1000000 - last_up;
        
      }
      else period += tp.tv_usec - last_up;
      //if (period == 0) printf("P = 0\n");
      //else printf("%f\n", high / (float) period);
      high = 0;
      period = 0;
      last_up = tp.tv_usec;
    }
    sample = a;
  }
    last_us = tp.tv_usec;

  if (tp.tv_usec < last_up) {
    if (period == 0) printf("P = 0\n");
    else printf("%f\n", high / float(period));
    high = 0;
    period = 0;
    last_us = 0;
  }
  else if (tp.tv_usec - last_us >= 50){
    printf("%d\n", tp.tv_usec);
  	int a = mraa_gpio_read(rec);
    //printf("%d\n", a);
  	if (a < 0) {
  		fprintf(stderr, "mraa_gpio_read() failed: %s\n", strerror(errno));
      mraa_gpio_close(rec);
      exit(1);
  	}

  	if (sample != a) {
      if (sample == 0) {
        num = num + 1;
      }
      sample = a;
    }
    last_us = tp.tv_usec;
  }*/
  //printf("%d, %d, %d, %d, %d\n", ch1_num , ch2_num, ch3_num, ch4_num, low_num);
  //gettimeofday(&tp2, NULL);
  //printf("%d\n", tp2.tv_usec - tp1.tv_usec);
  //exit(0);
  //signal (sig, catch_alarm);
  //signal (SIGALRM, SIG_DFL);
}

int main(int argc, char *argv[]) {

  ch1_context = mraa_gpio_init(2);
  ch2_context = mraa_gpio_init(4);
  ch3_context = mraa_gpio_init(7);
  ch4_context = mraa_gpio_init(8);
  /*if (tmp == NULL) {
  	fprintf(stderr, "mraa_aio_init() fail\n");
  	exit(1);
  }*/
  if (!ch1_context || !ch2_context || !ch3_context || !ch4_context) {
  	fprintf(stderr, "mraa_gpio_init() fail\n");
  	Shutdown(1);
  }

  mraa_gpio_dir(ch1_context, MRAA_GPIO_IN);
  mraa_gpio_dir(ch2_context, MRAA_GPIO_IN);
  mraa_gpio_dir(ch3_context, MRAA_GPIO_IN);
  mraa_gpio_dir(ch4_context, MRAA_GPIO_IN);

  mraa_gpio_use_mmaped(ch1_context, 1);
  mraa_gpio_use_mmaped(ch2_context, 1);
  mraa_gpio_use_mmaped(ch3_context, 1);
  mraa_gpio_use_mmaped(ch4_context, 1);
/*
  struct pollfd pfd[1];
  pfd[0].fd = 0;
  pfd[0].events = POLLIN | POLLERR;

  char buffer[20];


*/
/*
  struct sigaction sigact;

  sigact.sa_handler = catch_alarm;

  sigemptyset (&sigact.sa_mask);
  sigact.sa_flags = 0;

  sigaction (SIGALRM, &sigact, NULL);*/
  signal (SIGALRM, catch_alarm);

  ualarm (40000, 25);
  int i = 0;
  while (1) {
    /*i++;
    if (i == 100000) {
      printf("%d, %d, %d, %d, %d\n", ch1_num , ch2_num, ch3_num, ch4_num, low_num);
      return 0;
    }*/
    if (send_ready) {
      printf("%d, %d, %d, %d\n", ch1_num * 25, ch2_num * 25, ch3_num * 25, ch4_num * 25);
      send_ready = 0;
      //ualarm (20, 0);
    }
    if (send_miss) {
      printf("Invalid sample.\n");
      send_miss = 0;
      //ualarm (20, 0);
    }
    
    /*
    int ret_poll = poll(pfd, 1, 0);
    if (ret_poll == -1) {
      fprintf(stderr, "poll() failed: %s\n", strerror(errno));
      mraa_gpio_close(rec);
      exit(1);
    } else {
      if (ret_poll == 1) {
      	Check_rec();
        if (pfd[0].revents & POLLIN) {
        	bzero(buffer, 20);
          fgets(buffer, 20, stdin);
          if (!strcmp(buffer, "OFF\n")) {
          	Shutdown();
          }
        }
        if (pfd[0].revents & POLLERR) {
        	fprintf(stderr, "read() failed: %s\n", strerror(errno));
          //mraa_aio_close(tmp);
          mraa_gpio_close(rec);
          exit(1);
        }
      } else {
      	Check_rec();
      }
    }*/
  }

  return 0;
}