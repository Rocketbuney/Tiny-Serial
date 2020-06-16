#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

struct termios conf;
int fd;

static int setupTermios() {
  /* get serial configurations */
  if(tcgetattr(fd, &conf) < 0) {
    printf("error getting serial configuration, exitting\n");
    return -1;
  }

  /* set input flags */
  conf.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);

  /* set output flags */
  conf.c_oflag = 0;

  /* set line processing flags */
  conf.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);

  /* set character processing */
  conf.c_cflag = CS8 | CREAD | CLOCAL;

  conf.c_cc[VMIN]  = 1;
  conf.c_cc[VTIME] = 0;

  if(cfsetispeed(&conf, B9600) < 0 || cfsetospeed(&conf, B9600) < 0) {
    printf("unable to set baudrate, exitting\n");
    return -1;
  }

  /* apply new serial settings */
  if(tcsetattr(fd, TCSAFLUSH, &conf) < 0) {
    printf("unable to apply serial settings, exitting\n");
    return -1;
  }

  /* make reads non blocking */
  fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

  return 0;
}

int main(int argc, char const *argv[]) {
  unsigned char c = 'D';

  if(argc != 2) {
    printf("Usage: tiny-serial [path to tty device]\n");
    return 1;
  }

  /* open file with read write and no delay */
  fd = open(argv[1], O_RDWR | O_NOCTTY | O_NDELAY);
  if(fd == -1) {
    printf( "failed to open port\n" );
    return -1;
  }

  /* check if file descryptor points to a TTY device */
  if(!isatty(fd)) {
    printf("file entered is not a TTY device, exitting.\n");
    goto exit;
  }

  /* set up serial configurations */
  if(setupTermios() < 0) {
    goto exit;
  }

  while (c != 'q') {
    if (read(fd,&c,1)>0) {
      write(STDOUT_FILENO,&c,1); /* if new data is available on the serial port, print it out */
    }

    if (read(STDIN_FILENO,&c,1)>0) {
      write(fd,&c,1); /* if new data is available on the console, send it to the serial port */
    }
  }

  exit:
  close(fd);
  return 0;
}
