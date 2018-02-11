#include <unistd.h>   
#include <signal.h>   
#include <fcntl.h>  
#include <sys/syslog.h>  
#include <sys/param.h>   
#include <sys/types.h>   
#include <sys/stat.h>   
#include <stdio.h>  
#include <stdlib.h>  
#include <time.h>  
#include <string.h>
  
int init_daemon(void)  
{   
    int pid;   
    int i;  
      
    // 1）屏蔽一些控制终端操作的信号  
    signal(SIGTTOU,SIG_IGN);   
    signal(SIGTTIN,SIG_IGN);   
    signal(SIGTSTP,SIG_IGN);   
    signal(SIGHUP ,SIG_IGN);  
   
    // 2）在后台运行  
    if( pid=fork() ){ // 父进程  
        exit(0); //结束父进程，子进程继续  
    }else if(pid< 0){ // 出错  
        perror("fork");  
        exit(EXIT_FAILURE);  
    }  
      
    // 3）脱离控制终端、登录会话和进程组  
    setsid();    
      
    // 4）禁止进程重新打开控制终端  
    if( pid=fork() ){ // 父进程  
        exit(0);      // 结束第一子进程，第二子进程继续（第二子进程不再是会话组长）   
    }else if(pid< 0){ // 出错  
        perror("fork");  
        exit(EXIT_FAILURE);  
    }    
      
    // 5）关闭打开的文件描述符  
    // NOFILE 为 <sys/param.h> 的宏定义  
    // NOFILE 为文件描述符最大个数，不同系统有不同限制  
    for(i=0; i< NOFILE; ++i){  
        close(i);  
    }  
      
    // 6）改变当前工作目录  
    chdir("/tmp");   
      
    // 7）重设文件创建掩模  
    // umask值为022，则默认目录权限为755，默认文件权限为644。
    umask(0);    
      
    // 8）处理 SIGCHLD 信号  
    signal(SIGCHLD,SIG_IGN);  
      
    return 0;   
}   

int main(int argc, char *argv[])   
{  
    init_daemon();  

    time_t t;
    int fd;
      
    while(1) { 
        fd = open("daemon.log",O_WRONLY|O_CREAT|O_APPEND,0644);
        if(fd == -1) {
            perror("open file");  
            exit(EXIT_FAILURE);  
        }
        t = time(0);
        char *buf = asctime(localtime(&t));
        write(fd,buf,strlen(buf));
        close(fd);
        sleep(1);
    }
  
    return 0;  
}  
