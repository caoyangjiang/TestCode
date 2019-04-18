// Copyright 2017 Caoyang Jiang

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string>

const std::string RUNNING_DIR = "/tmp";
const std::string LOCK_FILE   = "exampled.lock";
const std::string LOG_FILE    = "exampled.log";

void log_message(const std::string& filename, const std::string& message)
{
  std::ofstream file(filename, std::ios::app | std::ios::binary);

  if (file.is_open())
  {
    file << message << "\n";
  }

  file.close();
}

void signal_handler(int sig)
{
  switch (sig)
  {
    case SIGHUP:
      log_message(LOG_FILE, std::string("hangup signal catched"));
      break;
    case SIGTERM:
      log_message(LOG_FILE, std::string("terminate signal catched"));
      exit(0);
      break;
  }
}

void daemonize()
{
  int i;
  // int lfp;
  // char str[10];
  if (getppid() == 1) return; /* already a daemon */
  i = fork();
  if (i < 0) exit(1); /* fork error */
  if (i > 0) exit(0); /* parent exits */
  /* child (daemon) continues */
  setsid(); /* obtain a new process group */
  // for (i = getdtablesize(); i >= 0; --i) close(i); /* close all descriptors
  // */ i = open("/dev/null", O_RDWR); dup(i); dup(i);             /* handle
  // standart I/O */ umask(027);         /* set newly created file permissions
  // */ chdir(RUNNING_DIR); /* change running directory */ lfp = open(LOCK_FILE,
  // O_RDWR | O_CREAT, 0640); if (lfp < 0) exit(1);                    /* can
  // not open */ if (lockf(lfp, F_TLOCK, 0) < 0) exit(0); /* can not lock */
  // /* first instance continues */
  // sprintf(str, "%d\n", getpid());
  // write(lfp, str, strlen(str)); /* record pid to lockfile */

  signal(SIGCHLD, SIG_IGN); /* ignore child */
  signal(SIGTSTP, SIG_IGN); /* ignore tty signals */
  signal(SIGTTOU, SIG_IGN);
  signal(SIGTTIN, SIG_IGN);
  signal(SIGHUP, signal_handler);  /* catch hangup signal */
  signal(SIGTERM, signal_handler); /* catch kill signal */
}

int main()
{
  daemonize();
  while (1)
  {
    sleep(1); /* run */
    std::cout << "running" << std::endl;
  }
  return 0;
}
