#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

static void print_error() {
  printf("Error code: %d - %s\n", errno, strerror(errno));
  exit(-1);
}

static void print_name() {
  printf("\033[0;36m  ______    _                                  _____                  _             __\n");
  printf(" /_  __/   (_)   ____   ___    __  __         / ___/  ___    _____   (_)  ____ _   / /\n");
  printf("  / /     / /   / __ \\ / _ \\  / / / /         \\__ \\  / _ \\  / ___/  / /  / __ `/  / / \n");
  printf(" / /     / /   / / / //  __/ / /_/ /         ___/ / /  __/ / /     / /  / /_/ /  / /  \n");
  printf("/_/     /_/   /_/ /_/ \\___/  \\__, /         /____/  \\___/ /_/     /_/   \\__,_/  /_/   \n");
  printf("                            /____/ \033[0m                                                  \n ");
}

static int check_args(int argc, char **argv, int *baud) {
  if(argc < 2) {
    printf("Usage: tiny-serial [path to tty device] [baud rate (defaults to 9600)]\n");
    return -1;
  }

  if(argv[2]) {
    *baud = atoi(argv[2]);
  }

  printf("TTY path: %s\t\tBaud rate:%d\n", argv[1], *baud);
  return 1;
}

static int init_serial(struct termios *tty, const int fd, const int baud) {
  /* get serial ttyigurations */
  if(tcgetattr(fd, tty) < 0) {
    return -1;
  }

  /* set serial connection flags */
  tty->c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
  tty->c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
  tty->c_cflag = CS8 | CREAD | CLOCAL;

  tty->c_cc[VMIN]  = 1;
  tty->c_cc[VTIME] = 0;

  /* set baudrate */
  cfsetispeed(tty, baud);
  cfsetospeed(tty, baud);

  /* apply new serial settings */
  if(tcsetattr(fd, TCSAFLUSH, tty) != 0) {
    return -1;
  }

  /* make reads non blocking */
  fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

  return 0;
}

static void data_recv_loop(const int fd) {
  unsigned char incoming_byte = 0, outgoing_byte = 0;

  setbuf(stdout, NULL);
  
  do {
    /* if new data is available on the serial port, print it out */
    while (read(fd, &incoming_byte, 1) > 0) {
      printf("\033[1;32m%c\033[0m", incoming_byte);
    }

    /* if new data is available on the console, send it to the serial port */
    if (read(STDIN_FILENO, &outgoing_byte, 1) > 0 && tolower(outgoing_byte) != 'q')
      write(fd, &outgoing_byte, 1); 

  } while(tolower(outgoing_byte) != 'q');
}

int main(int argc, char **argv) {
  struct termios tty;
  int fd = 0, baud = 9600;

  print_name();
  check_args(argc, argv, &baud);

  fd = open(argv[1], O_RDWR | O_NOCTTY | O_NDELAY);
  if(fd < 1 || !isatty(fd))
    print_error();

  if(init_serial(&tty, fd, baud) != 0)
    print_error();
  
  data_recv_loop(fd);

  close(fd);
  return 0;
}
