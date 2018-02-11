#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <sys/poll.h>
#include <curl/curl.h>
#include <ev.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DPRINT(x...) printf(x)

#define MSG_OUT stdout /* Send info to stdout, change to stderr if you want */

#define  PEER_IP_LENGTH      16

static int getpeerinfo(int fd, char* ipaddr, int *port)
{
	struct sockaddr_in name;
	socklen_t namelen = sizeof(name);
	struct hostent* result;

	/*
	 * Clear the memory.
	 */
	memset(ipaddr, '\0', PEER_IP_LENGTH);
	if (getpeername(fd, (struct sockaddr *)&name, &namelen) != 0) {
		return -1;
	} 
	strncpy(ipaddr,
			inet_ntoa(*(struct in_addr *)&name.sin_addr.s_addr),
			PEER_IP_LENGTH);
	*port = ntohs(name.sin_port);
  }



/* Global information, common to all connections */
typedef struct _GlobalInfo
{
  struct ev_loop *loop; // ev reactor
  struct ev_io fifo_event; // io event handler
  struct ev_timer timer_event; // time event handler
  CURLM *multi; //  multi curl ptr
  int still_running;
  FILE *input;
} GlobalInfo;


/* Information associated with a specific easy handle */
typedef struct _ConnInfo
{
  CURL *easy;  // curl ptr
  char *url; 
  GlobalInfo *global; // global info
  char error[CURL_ERROR_SIZE]; // error msg
  char buf[1024];
  unsigned int len;
} ConnInfo;


/* Information associated with a specific socket */
typedef struct _SockInfo
{
  curl_socket_t sockfd; // fd
  CURL *easy;
  int action; // 
  long timeout; // 超时时间
  struct ev_io ev; // event handler assoc with fd
  int evset;
  GlobalInfo *global; // global info
} SockInfo;

/* Die if we get a bad CURLMcode somewhere */
static void mcode_or_die(const char *where, CURLMcode code)
{
  if(CURLM_OK != code) {
    const char *s;
    switch(code) {
    case CURLM_BAD_HANDLE:
      s = "CURLM_BAD_HANDLE";
      break;
    case CURLM_BAD_EASY_HANDLE:
      s = "CURLM_BAD_EASY_HANDLE";
      break;
    case CURLM_OUT_OF_MEMORY:
      s = "CURLM_OUT_OF_MEMORY";
      break;
    case CURLM_INTERNAL_ERROR:
      s = "CURLM_INTERNAL_ERROR";
      break;
    case CURLM_UNKNOWN_OPTION:
      s = "CURLM_UNKNOWN_OPTION";
      break;
    case CURLM_LAST:
      s = "CURLM_LAST";
      break;
    default:
      s = "CURLM_unknown";
      break;
    case CURLM_BAD_SOCKET:
      s = "CURLM_BAD_SOCKET";
      fprintf(MSG_OUT, "ERROR: %s returns %s\n", where, s);
      /* ignore this error */
      return;
    }
    fprintf(MSG_OUT, "ERROR: %s returns %s\n", where, s);
    exit(code);
  }
}
static void check_multi_info(GlobalInfo *g)
{
  char *eff_url;
  CURLMsg *msg;
  int msgs_left;
  ConnInfo *conn;
  CURL *easy;
  CURLcode res;

  fprintf(MSG_OUT, "REMAINING: %d\n", g->still_running);
  while((msg = curl_multi_info_read(g->multi, &msgs_left))) {
    if(msg->msg == CURLMSG_DONE) {
      easy = msg->easy_handle;
      res = msg->data.result;
      curl_easy_getinfo(easy, CURLINFO_PRIVATE, &conn);
      curl_easy_getinfo(easy, CURLINFO_EFFECTIVE_URL, &eff_url);
      fprintf(MSG_OUT, "DONE: %s => (%d) %s\n", eff_url, res, conn->error);
      // 打印出数据
      conn->buf[conn->len] = '\0';
      fprintf(MSG_OUT,"MSG: %s \n",conn->buf);
      curl_multi_remove_handle(g->multi, easy);
      free(conn->url);
      curl_easy_cleanup(easy);
      free(conn);
    }
  }
}

static void timer_cb(EV_P_ struct ev_timer *w, int revents);

static void timer_cb(EV_P_ struct ev_timer *w, int revents)
{
  DPRINT("%s  w %p revents %i\n", __PRETTY_FUNCTION__, w, revents);

  GlobalInfo *g = (GlobalInfo *)w->data;
  CURLMcode rc;

  rc = curl_multi_socket_action(g->multi, CURL_SOCKET_TIMEOUT, 0, &g->still_running);
  mcode_or_die("timer_cb: curl_multi_socket_action", rc);
  check_multi_info(g);
}

/* Update the event timer after curl_multi library calls */
static int multi_timer_cb(CURLM *multi, long timeout_ms, GlobalInfo *g)
{
  DPRINT("%s %li\n", __PRETTY_FUNCTION__,  timeout_ms);
  ev_timer_stop(g->loop, &g->timer_event);
  if(timeout_ms > 0) {
    double  t = timeout_ms / 1000;
    ev_timer_init(&g->timer_event, timer_cb, t, 0.);
    ev_timer_start(g->loop, &g->timer_event);
  }
  else if(timeout_ms == 0)
    timer_cb(g->loop, &g->timer_event, 0);
  return 0;
}

/* Called by libevent when we get action on a multi socket */
static void event_cb(EV_P_ struct ev_io *w, int revents)
{
  DPRINT("%s  w %p revents %i\n", __PRETTY_FUNCTION__, w, revents);
  GlobalInfo *g = (GlobalInfo*) w->data;
  CURLMcode rc;

  int action = (revents&EV_READ?CURL_POLL_IN:0)|
    (revents&EV_WRITE?CURL_POLL_OUT:0);
  // 在这里触发curl函数调用
  rc = curl_multi_socket_action(g->multi, w->fd, action, &g->still_running);
  mcode_or_die("event_cb: curl_multi_socket_action", rc);
  check_multi_info(g);
  if(g->still_running <= 0) {
    fprintf(MSG_OUT, "last transfer done, kill timeout\n");
    ev_timer_stop(g->loop, &g->timer_event);
  }
}

/* Called by libevent when our timeout expires */

/* Clean up the SockInfo structure */
static void remsock(SockInfo *f, GlobalInfo *g)
{
  printf("%s  \n", __PRETTY_FUNCTION__);
  if(f) {
    if(f->evset)
      ev_io_stop(g->loop, &f->ev);
    free(f);
  }
}



/* Assign information to a SockInfo structure */
static void setsock(SockInfo *f, curl_socket_t s, CURL *e, int act,
                    GlobalInfo *g)
{
  printf("%s  \n", __PRETTY_FUNCTION__);

  int kind = (act&CURL_POLL_IN?EV_READ:0)|(act&CURL_POLL_OUT?EV_WRITE:0);

  f->sockfd = s;
  f->action = act;
  f->easy = e;
  if(f->evset) {
      printf(">>>>stop connect info\n");
      // 重新初始化
    ev_io_stop(g->loop, &f->ev);
  }
  char ip[PEER_IP_LENGTH];
  int port = 0;
  getpeerinfo(s,ip,&port);
  printf(">>>> IP [%s:%d]\n",ip,port);
  // 初始化 io event handler
  ev_io_init(&f->ev, event_cb, f->sockfd, kind);
  f->ev.data = g;
  f->evset = 1;
  // 注册
  ev_io_start(g->loop, &f->ev);
}



/* Initialize a new SockInfo structure */
static void addsock(curl_socket_t s, CURL *easy, int what, GlobalInfo *g)
{
  DPRINT("%s e %p s %i what %i cbp %p\n",
         __PRETTY_FUNCTION__,easy, s, what, g);
  SockInfo *fdp = calloc(sizeof(SockInfo), 1);

  fdp->global = g;
  setsock(fdp, s, easy, what, g);
  curl_multi_assign(g->multi, s, fdp);
}

/* CURLMOPT_SOCKETFUNCTION */
static int sock_cb(CURL *e, curl_socket_t s, int what, void *cbp, void *sockp)
{
  DPRINT("%s e %p s %i what %i cbp %p sockp %p\n",
         __PRETTY_FUNCTION__, e, s, what, cbp, sockp);

  GlobalInfo *g = (GlobalInfo*) cbp;
  SockInfo *fdp = (SockInfo*) sockp;
  const char *whatstr[]={ "none", "IN", "OUT", "INOUT", "REMOVE"};

  fprintf(MSG_OUT,
          "socket callback: s=%d e=%p what=%s ", s, e, whatstr[what]);
  if(what == CURL_POLL_REMOVE) {
    fprintf(MSG_OUT, "\n");
    remsock(fdp, g);
  }
  else {
    // 如果fdp为空
    if(!fdp) {
      fprintf(MSG_OUT, "Adding data: %s\n", whatstr[what]);
      addsock(s, e, what, g);
    }
    else {
      fprintf(MSG_OUT,
              "Changing action from %s to %s\n",
              whatstr[fdp->action], whatstr[what]);
      setsock(fdp, s, e, what, g);
    }
  }
  return 0;
}


/* CURLOPT_WRITEFUNCTION */
static size_t write_cb(void *ptr, size_t size, size_t nmemb, void *data)
{
  size_t realsize = size * nmemb;
  ConnInfo *conn = (ConnInfo*) data;
  memcpy(conn->buf,ptr,realsize);
  conn->len += realsize;
  return realsize;
}


/* CURLOPT_PROGRESSFUNCTION */
static int prog_cb(void *p, double dltotal, double dlnow, double ult,
                   double uln)
{
  ConnInfo *conn = (ConnInfo *)p;
  (void)ult;
  (void)uln;

  fprintf(MSG_OUT, "Progress: %s (%g/%g)\n", conn->url, dlnow, dltotal);
  return 0;
}


/* Create a new easy handle, and add it to the global curl_multi */
static void new_conn(char *url, GlobalInfo *g)
{
  ConnInfo *conn;
  CURLMcode rc;

  conn = calloc(1, sizeof(ConnInfo));
  memset(conn, 0, sizeof(ConnInfo));
  conn->error[0]='\0';

  conn->len = 0;

  conn->easy = curl_easy_init();
  if(!conn->easy) {
    fprintf(MSG_OUT, "curl_easy_init() failed, exiting!\n");
    exit(2);
  }
  conn->global = g;
  conn->url = strdup(url);
  curl_easy_setopt(conn->easy, CURLOPT_URL, conn->url);
  curl_easy_setopt(conn->easy, CURLOPT_WRITEFUNCTION, write_cb);
  curl_easy_setopt(conn->easy, CURLOPT_WRITEDATA, conn);
  // curl_easy_setopt(conn->easy, CURLOPT_VERBOSE, 1L);
  curl_easy_setopt(conn->easy, CURLOPT_ERRORBUFFER, conn->error);
  curl_easy_setopt(conn->easy, CURLOPT_PRIVATE, conn);
  /*curl_easy_setopt(conn->easy, CURLOPT_NOPROGRESS, 0L);           */
  /*curl_easy_setopt(conn->easy, CURLOPT_PROGRESSFUNCTION, prog_cb);*/
  /*curl_easy_setopt(conn->easy, CURLOPT_PROGRESSDATA, conn);       */
  curl_easy_setopt(conn->easy, CURLOPT_LOW_SPEED_TIME, 3L);
  curl_easy_setopt(conn->easy, CURLOPT_LOW_SPEED_LIMIT, 10L);

  long int ctimeout = 100;
  long int rwtimeout = 200;
  curl_easy_setopt(conn->easy, CURLOPT_CONNECTTIMEOUT_MS, ctimeout);
  curl_easy_setopt(conn->easy, CURLOPT_TIMEOUT_MS,  rwtimeout);

  fprintf(MSG_OUT,
          "Adding easy %p to multi %p (%s)\n", conn->easy, g->multi, url);
  rc = curl_multi_add_handle(g->multi, conn->easy);
  mcode_or_die("new_conn: curl_multi_add_handle", rc);

  /* note that the add_handle() will set a time-out to trigger very soon so
     that the necessary socket_action() call will be called by this app */
}

/* This gets called whenever data is received from the fifo */
static void fifo_cb(EV_P_ struct ev_io *w, int revents)
{
  char s[1024];
  long int rv = 0;
  int n = 0;
  GlobalInfo *g = (GlobalInfo *)w->data;

  do {
    s[0]='\0';
    rv = fscanf(g->input, "%1023s%n", s, &n);
    s[n]='\0';
    if(n && s[0]) {
      new_conn(s, g);  /* if we read a URL, go get it! */
    }
    else
      break;
  } while(rv != EOF);
}

/* Create a named pipe and tell libevent to monitor it */
static int init_fifo(GlobalInfo *g)
{
  struct stat st;
  static const char *fifo = "hiper.fifo";
  curl_socket_t sockfd;

  fprintf(MSG_OUT, "Creating named pipe \"%s\"\n", fifo);
  if(lstat (fifo, &st) == 0) {
    if((st.st_mode & S_IFMT) == S_IFREG) {
      errno = EEXIST;
      perror("lstat");
      exit(1);
    }
  }
  unlink(fifo);
  if(mkfifo (fifo, 0600) == -1) {
    perror("mkfifo");
    exit(1);
  }
  sockfd = open(fifo, O_RDWR | O_NONBLOCK, 0);
  if(sockfd == -1) {
    perror("open");
    exit(1);
  }
  g->input = fdopen(sockfd, "r");

  fprintf(MSG_OUT, "Now, pipe some URL's into > %s\n", fifo);

  // loop 监听fifo文件的可读事件
  ev_io_init(&g->fifo_event, fifo_cb, sockfd, EV_READ);
  ev_io_start(g->loop, &g->fifo_event);
  return (0);
}

int main(int argc, char **argv)
{
  GlobalInfo g;
  CURLMcode rc;
  (void)argc;
  (void)argv;

  memset(&g, 0, sizeof(GlobalInfo));
  g.loop = ev_default_loop(0); // init ev reactor

  init_fifo(&g);
  g.multi = curl_multi_init();

  ev_timer_init(&g.timer_event, timer_cb, 0., 0.);
  g.timer_event.data = &g;
  g.fifo_event.data = &g;

  curl_multi_setopt(g.multi, CURLMOPT_SOCKETFUNCTION, sock_cb);
  curl_multi_setopt(g.multi, CURLMOPT_SOCKETDATA, &g);
  curl_multi_setopt(g.multi, CURLMOPT_TIMERFUNCTION, multi_timer_cb);
  curl_multi_setopt(g.multi, CURLMOPT_TIMERDATA, &g);

  /* we don't call any curl_multi_socket*() function yet as we have no handles
     added! */

  ev_loop(g.loop, 0);
  curl_multi_cleanup(g.multi);
  return 0;
}
