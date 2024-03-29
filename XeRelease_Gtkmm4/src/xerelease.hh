#ifndef __XE_RELEASE_
#define __XE_RELEASE_

#include "json_nlohmann/json.hpp"
#include "config.hh"

using json = nlohmann::json;

typedef struct{
    int year;
    int month;
    int day;
    const char *branch;
    const char *version;
}XeVer;

int total_day(int year,int month,int day);

int total_year_day(int year1,int year2);

void get_release_str(struct tm *local,const XeVer &version1, char *callback_str);

void json_config_init(json &user_data);

// void dale(struct tm *local);

// void longterm(struct tm *local,const char * lts,char *str);

// void stable(struct tm *local,const char * rel,char *str);

// void develop(struct tm *local,const char * devel,char *str);

// static inline bool rel_unix_file_system_detected(){
// #ifdef _WIN32
//     return false;
// #else
//     return true;
// #endif
// }

#endif