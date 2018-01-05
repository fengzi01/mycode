// 利用libev和curl实现http客户端

#include <curl/curl.h>
#include <ev.h>
#include <curl/multi.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#define MSG_OUT stdout

/* 全局变量信息 */
typedef struct _GlobalInfo {
    struct ev_loop *loop; // reactor
    struct ev_timer timer_ehl;
    CURLM *multi; // multi curl句柄 
    int still_running; // 还在运行的curl数目
} GlobalInfo;

// 全局唯一实例
//GlobalInfo *g;

/* easy curl 句柄 */
typedef struct _ConnInfo {
    CURL *easy;
    char url[256];
    GlobalInfo *global;
    char error[256]; // error msg

    // 存储结果
    char data[1024];
    unsigned int len;

    struct ev_timer timeout_ehl;
    double timeout;
} ConnInfo;

/* socket句柄 */
typedef struct _SockInfo {
    curl_socket_t fd; // fd
    CURL *easy;

    int what; // 事件
    struct ev_io io_ehl; // io事件处理handler

    GlobalInfo *global;

    int evset; // ?
} SockInfo;

static void mcode_or_die(const char *where, CURLMcode code);

// 写函数
static size_t curl_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t real = size * nmemb;
    ConnInfo *conn = (ConnInfo *)userdata;

    // 读取结果
    memcpy(conn->data,ptr,real);
    conn->len += real;

    return real;
}

static void sockio_cb(struct ev_loop *loop,struct ev_io *w,int revents) {
    GlobalInfo *g = (GlobalInfo *)w->data;
    CURLMcode rc;

    int action = (revents&EV_READ?CURL_POLL_IN:0)|
        (revents&EV_WRITE?CURL_POLL_OUT:0);

    rc = curl_multi_socket_action(g->multi,w->fd,action,&g->still_running);

    if (g->still_running <= 0) {
        fprintf(MSG_OUT,"last transfer done. stop timer event\n");
        char *eff_url;
        CURLMsg *msg;
        int msgs_left;
        ConnInfo *conn;
        CURL *easy;
        CURLcode res;
        while((msg = curl_multi_info_read(g->multi, &msgs_left))) {

            //printf("ev loop start\n");

            if(msg->msg == CURLMSG_DONE) {
                easy = msg->easy_handle;
                res = msg->data.result;
                curl_easy_getinfo(easy, CURLINFO_PRIVATE, &conn);
                curl_easy_getinfo(easy, CURLINFO_EFFECTIVE_URL, &eff_url);
                // 打印出数据
                conn->data[conn->len] = '\0';
                fprintf(MSG_OUT,"MSG: %s \n",conn->data);
                curl_multi_remove_handle(g->multi, easy);
                ev_timer_stop(g->loop,&conn->timeout_ehl);
                curl_easy_cleanup(easy);
                free(conn);
            }

            //printf("ev loop end\n");
        }

        //printf("trans end\n");

        ev_timer_stop(g->loop,&g->timer_ehl);
    }
}

static void setsock(SockInfo *f, curl_socket_t s, CURL *e, int what, GlobalInfo *g) {
    printf("setsock\n");
    int action = (what&CURL_POLL_IN?EV_READ:0) | (what&CURL_POLL_OUT?EV_WRITE:0);
    if (f->evset) {
        ev_io_stop(g->loop,&f->io_ehl);
    }

    f->fd = s;
    f->what = what;
    f->easy = e;

    ev_io_init(&f->io_ehl,sockio_cb,f->fd,action);
    f->io_ehl.data = g;
    f->evset = 1;

    printf("set io\n");

    ev_io_start(g->loop,&f->io_ehl);

    printf("setsock stop\n");
}

static void timeout_cb (EV_P_ ev_timer *w, int revents) {
    ConnInfo *conn = (ConnInfo *)w->data;
    ev_timer_stop(conn->global->loop,&conn->timeout_ehl);
    printf("socket timeout after %e\n",conn->timeout);
    curl_multi_remove_handle(conn->global->multi, conn->easy);
}

static int mul_sock_cb(CURL *easy,      /* easy handle */
		curl_socket_t s, /* socket */
		int what,        /* describes the socket */
		void *userp,     /* private callback pointer */
		void *socketp)  /* private socket pointer */
{
	GlobalInfo *g = (GlobalInfo *) userp;
    SockInfo *fdp = (SockInfo *)socketp;

    if (what == CURL_POLL_REMOVE) {
        fprintf(MSG_OUT,"remove\n");
        ev_io_stop(g->loop,&fdp->io_ehl);
        free(fdp);
    } else {
        if (!fdp) {
            fprintf(MSG_OUT,"add\n");
            fdp = (SockInfo *)malloc(sizeof(SockInfo));
            fdp->global = g;

            // 初始化sock到ev
            setsock(fdp,s,easy,what,g);
            
            curl_multi_assign(g->multi,s,fdp);
        } else {
            fprintf(MSG_OUT,"change\n");
            setsock(fdp,s,easy,what,g);
        }
    }
}

static void timer_cb (EV_P_ ev_timer *w, int revents) {
    // 传0是啥意思呢
    GlobalInfo *g = (GlobalInfo *)w->data;
    curl_multi_socket_action(g->multi,CURL_SOCKET_TIMEOUT,0,&g->still_running);
}

static int mul_timer_cb(CURLM *multi,    /* multi handle */
		long timeout_ms, /* see above */
		void *userp)    /* private callback pointer */
{
	GlobalInfo *g = (GlobalInfo *)userp;
	ev_timer_stop(g->loop, &g->timer_ehl);
	
    if (timeout_ms > 0) {
        double t = timeout_ms / 1000.0;
        ev_timer_init(&g->timer_ehl,timer_cb,t,0.);
        ev_timer_start(g->loop,&g->timer_ehl);
    }
    else if(timeout_ms == 0) {
        timer_cb(g->loop,&g->timer_ehl,0);
    }
    return 0;
}


#define URL_SIZE 4 
int main(int argc, char *argv[])
{
    char *urls[URL_SIZE] = {
        "127.0.0.1:8081",
        "127.0.0.1:8081",
        "127.0.0.1:8081",
        "127.0.0.1:8080",
        //"www.baidu.com",
        //"www.mincooder.com"
    };
    GlobalInfo g;

    int i = 0;
    ConnInfo *conn;
    CURLMcode rc;

    g.loop = ev_default_loop(0);

    ev_timer_init(&g.timer_ehl,timer_cb,0.,0.);
    g.multi = curl_multi_init();
    g.timer_ehl.data = &g;

    curl_multi_setopt(g.multi,CURLMOPT_SOCKETFUNCTION,mul_sock_cb);
	curl_multi_setopt(g.multi,CURLMOPT_SOCKETDATA,&g);

	curl_multi_setopt(g.multi,CURLMOPT_TIMERFUNCTION,mul_timer_cb);
	curl_multi_setopt(g.multi,CURLMOPT_TIMERDATA,&g);

    for (;i<URL_SIZE;i++) {
        long int ctimeout = 10;
        long int rwtimeout = 200;
        conn = (ConnInfo *)malloc(sizeof(ConnInfo)); 
        memset(conn,0,sizeof(ConnInfo));

        strncpy(conn->url,urls[i],strlen(urls[i]));
        conn->error[0] = '\0';
        conn->len = 0;
        conn->global = &g;

        conn->easy = curl_easy_init();
        if (!conn->easy) {
            perror("curl easy init");
            return -1;
        }

        // 初始化curl easy句柄
        curl_easy_setopt(conn->easy, CURLOPT_URL, conn->url);
        curl_easy_setopt(conn->easy, CURLOPT_WRITEFUNCTION, curl_write_cb);
        curl_easy_setopt(conn->easy, CURLOPT_WRITEDATA, conn);

        curl_easy_setopt(conn->easy, CURLOPT_PRIVATE, conn);
        curl_easy_setopt(conn->easy, CURLOPT_ERRORBUFFER, conn->error);
        
        //curl_easy_setopt(conn->easy, CURLOPT_CONNECTTIMEOUT_MS, ctimeout);
        //curl_easy_setopt(conn->easy, CURLOPT_TIMEOUT_MS,  rwtimeout);
        
        conn->timeout = 1.0;
        // 初始化timeout监听
        conn->timeout_ehl.data = conn;
        ev_timer_init(&conn->timeout_ehl,timeout_cb,conn->timeout,0.);
        ev_timer_start(g.loop,&conn->timeout_ehl);

		fprintf(MSG_OUT,
				"Adding easy %p to multi %p (%s)\n", conn->easy, g.multi, conn->url);
		rc = curl_multi_add_handle(g.multi,conn->easy);
		mcode_or_die("new_conn: curl_multi_add_handle", rc);

    }

	ev_loop(g.loop,0);
	curl_multi_cleanup(g.multi);
    
    return 0;
}

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
