#ifndef COMMON_H
#define COMMON_H

#ifndef PUBLIC_H
#define PUBLIC_H
#include <iostream>
#include <cstring>
#include <ctime>

#include<stdio.h>
#define PATH_LEN 100
//#define FILE_NAME_LEN 100
#define IP_LEN 16
#define DATA_MAX 300000
#define VIDEO_WIDTH 640
#define VIDEO_HEIGHT 480
typedef unsigned char  ui8;
using namespace std;
class Tools{
public:
    static void prtinfo(ui8 *str){
        cout<<str;
        fflush(stdout);
    }
};

#define BUFSIZE 200

inline void add_title(char *label,char *str,int line,char *src_file)
{
    char tmp_str[BUFSIZE]={};
    char time_label[BUFSIZE]={};
    char title_label[BUFSIZE]={};
    char line_label[BUFSIZE]={};

    struct tm *p_tm;
    time_t timer;
    /* gets time of day */
    timer = time(NULL);
    /* converts date/time to a structure */
    p_tm = localtime(&timer);
    sprintf(time_label, "[%d:%d:%d]", p_tm->tm_hour,p_tm->tm_min,p_tm->tm_sec);
    sprintf(title_label, "[%s]", label);
    sprintf(line_label, "(%s:%d)====>", src_file,line);
    //	if(strcmp(label,"cam_state")==0){
    //		print_stacktrace();
    //	}
#if defined( LOG_NONE)
    if(strcmp(label,"info---1")==0\
            ||strcmp(label,"err--1")==0 \
            ||strcmp(label,"exit--1")==0 \
            ||strcmp(label,"net---1")==0 \
            ||strcmp(label,"in_loop---1")==0 \
            ||strcmp(label,"config_change---1")==0 \
            ||strcmp(label,"stack")==0 \
            ||strcmp(label,"config---1")==0 \
            ||strcmp(label,"cam_info---1")==0 \
            ||strcmp(label,"check_setting--1")==0 \
            ||strcmp(label,"check_client_cmd--1")==0 \
            ||strcmp(label,"cam_state--1")==0 \
            ||strcmp(label,"sig_state--1")==0 \
            ||strcmp(label,"alg---1")==0 \
            ||strcmp(label,"clients_msg---1")==0 \
            ||strcmp(label,"camera_msg---1")==0 \
            ||strcmp(label,"debug_sig----1")==0 )
#elif defined( LOG_LEVEL1)
    if(strcmp(label,"info---1")==0\
            ||strcmp(label,"err")==0 \
            ||strcmp(label,"exit")==0 \
            ||strcmp(label,"net---1")==0 \
            ||strcmp(label,"in_loop---1")==0 \
            ||strcmp(label,"config_change---1")==0 \
            ||strcmp(label,"stack")==0 \
            ||strcmp(label,"config---1")==0 \
            ||strcmp(label,"cam_info---1")==0 \
            ||strcmp(label,"check_setting--1")==0 \
            ||strcmp(label,"check_client_cmd--1")==0 \
            ||strcmp(label,"cam_state--1")==0 \
            ||strcmp(label,"sig_state--1")==0 \
            ||strcmp(label,"alg---1")==0 \
            ||strcmp(label,"clients_msg")==0 \
            ||strcmp(label,"camera_msg")==0 \
            ||strcmp(label,"debug_sig----1")==0 )
#elif defined( LOG_LEVEL2)
    if(strcmp(label,"info")==0\
            ||strcmp(label,"err")==0 \
            ||strcmp(label,"exit")==0 \
            ||strcmp(label,"net")==0 \
            ||strcmp(label,"in_loop---1")==0 \
            ||strcmp(label,"config_change---1")==0 \
            ||strcmp(label,"stack")==0 \
            ||strcmp(label,"config---1")==0 \
            ||strcmp(label,"cam_info---1")==0 \
            ||strcmp(label,"check_setting--1")==0 \
            ||strcmp(label,"check_client_cmd--1")==0 \
            ||strcmp(label,"cam_state--1")==0 \
            ||strcmp(label,"sig_state--1")==0 \
            ||strcmp(label,"server")==0 \
            ||strcmp(label,"alg---1")==0 \
            ||strcmp(label,"clients_msg")==0 \
            ||strcmp(label,"camera_msg")==0 \
            ||strcmp(label,"debug_sig")==0 )
#else defined( LOG_LEVEL3)
    if(strcmp(label,"info")==0\
            ||strcmp(label,"err")==0 \
            ||strcmp(label,"exit")==0 \
            ||strcmp(label,"net---1")==0 \
            ||strcmp(label,"in_loop---1")==0 \
            ||strcmp(label,"config_change---1")==0 \
            ||strcmp(label,"stack")==0 \
            ||strcmp(label,"config---1")==0 \
            ||strcmp(label,"cam_info---1")==0 \
            ||strcmp(label,"check_setting--1")==0 \
            ||strcmp(label,"check_client_cmd--1")==0 \
            ||strcmp(label,"cam_state--1")==0 \
            ||strcmp(label,"sig_state--1")==0 \
            ||strcmp(label,"alg---1")==0 \
            ||strcmp(label,"clients_msg")==0 \
            ||strcmp(label,"camera_msg")==0 \
            ||strcmp(label,"debug_sig----1")==0 )
#endif
    {
        strcpy(tmp_str,str);
        strcpy(str,time_label);
        strcat(str,title_label);
        strcat(str,line_label);
        strcat(str,tmp_str);

        strcat(str,"\n");
        if(!strcmp(label,"stack")){
            //       print_stacktrace();
        }
    }else{
        memset(str,0,BUFSIZE);
        //	sprintf(str, "ignoring unknown label [%s]\n", label);
    }

}
inline void print_str(char *str) {
    printf("%s", str);
    fflush(NULL);
}
#define prt(label,... ) {\
    char tmp_string[BUFSIZE] ;	\
    sprintf(tmp_string,__VA_ARGS__);	\
    add_title(#label,tmp_string,__LINE__,__FILE__);  	\
    print_str(tmp_string);   \
    }

#endif // PUBLIC_H

#endif // COMMON_H
