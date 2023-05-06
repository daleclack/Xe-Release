#include <cstdio>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include "xerelease.hh"
#include "xeapi.hh"

static json data1;

// typedef void(*LP)(struct tm *local);//define a  pointer function

int total_day(int year, int month, int day)
{
    // Calculate day of 1 year
    int sum = 0;
    switch (month)
    {
    case 1:
        sum = day;
        break;
    case 2:
        sum = day + 31;
        break;
    case 3:
        sum = day + 59;
        break;
    case 4:
        sum = day + 90;
        break;
    case 5:
        sum = day + 120;
        break;
    case 6:
        sum = day + 151;
        break;
    case 7:
        sum = day + 181;
        break;
    case 8:
        sum = day + 212;
        break;
    case 9:
        sum = day + 243;
        break;
    case 10:
        sum = day + 273;
        break;
    case 11:
        sum = day + 304;
        break;
    case 12:
        sum = day + 334;
        break;
    default:
        printf("Date Wrong!");
    }
    if (year % 4 == 0 && year % 100 != 0)
        sum = sum + 1;
    return sum;
}

int total_year_day(int year1, int year2)
{
    // Calculate day of years
    int sum = 0;
    sum = (year2 - year1) * 365;
    for (int i = year1; i < year2; i++)
    {
        if (i % 4 == 0 && i % 100 != 0)
        {
            sum = sum + 1;
        }
    }
    return sum;
}

static void path_translate(char *result, const char *version)
{
    if (!data1.empty())
    {
        // Just combine the release file path and filename
        std::string path;
        switch (get_os_type())
        {
        case OS_Type::Linux:
            path = data1["Release_Path_Unix"];
            snprintf(result, 57, "%s/xe-%c.x", path.c_str(), version[0]);
            break;
        case OS_Type::Darwin:
            path = data1["Release_Path_Darwin"];
            snprintf(result, 57, "%s/xe-%c.x", path.c_str(), version[0]);
            break;
        case OS_Type::Windows:
            path = data1["Release_Path_Win32"];
            snprintf(result, 57, "%s\\xe-%c.x", path.c_str(), version[0]);
            break;
        }
        // if (rel_unix_file_system_detected())
        // {
        //     path = data1["Release_Path_Unix"];
        //     sprintf(result, "%s/xe-%c.x", path.c_str(), version[0]);
        // }
        // else
        // {
        //     path = data1["Release_Path_Win32"];
        //     sprintf(result, "%s\\xe-%c.x", path.c_str(), version[0]);
        // }
    }
    else
    {
        snprintf(result, 57, "./xe-%c.x", version[0]);
    }
}

void dale(struct tm *local)
{
    printf("xeinit release maker by dale\n");
}

void longterm(struct tm *local, const char *lts, char *str)
{
    // Print Version of longterm release in the file
    char filename[57];
    path_translate(filename, lts);
    // sprintf(filename, "xe-%c.x", lts[0]);
    int lts_ver = 0; // default release version
    int year1 = 2019, month1 = 1, day1 = 11, year2 = local->tm_year + 1900,
        month2 = local->tm_mon + 1, day2 = local->tm_mday;
    // get release version
    lts_ver = total_year_day(year1, year2) - total_day(year1, month1, day1) +
              total_day(year2, month2, day2);
    // For show in dialog or console
    snprintf(str, 57, "Xeinit LTS version:%s.%d\n", lts, lts_ver);
    freopen(filename, "a", stdout);
    // put all output in the release file
    // output:release branch time in xe-release
    printf("%4d-%02d-%02d ", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);
    printf("%s.%d Api:%d\n", lts, lts_ver, xeapi1(local));
    fclose(stdout);
    return;
}

void stable(struct tm *local, const char *rel, char *str)
{
    // Print Version of stable release in the file
    char filename[57];
    path_translate(filename, rel);
    // sprintf(filename, "xe-%c.x", rel[0]);
    int devel1; // stable release version
    int year1 = 2017, month1 = 5, day1 = 19, year2 = local->tm_year + 1900,
        month2 = local->tm_mon + 1, day2 = local->tm_mday;
    // get release version
    devel1 = total_year_day(year1, year2) - total_day(year1, month1, day1) +
             total_day(year2, month2, day2);
    snprintf(str, 57, "Xeinit stable Version:%s.%d\n", rel, devel1);
    freopen(filename, "a", stdout);
    // put all output in the release file
    // output:development branch time in xe-release
    printf("%4d-%02d-%02d ", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);
    printf("%s.%d build:%d\n", rel, devel1, xeapi1(local));
    fclose(stdout);
    return;
}

void develop(struct tm *local, const char *devel, char *str)
{
    // Print Version of develop release in the file
    char filename[57];
    path_translate(filename, devel);
    // sprintf(filename, "xe-%c.x", devel[0]);
    int devel1; // development version
    int year1 = 2017, month1 = 5, day1 = 19, year2 = local->tm_year + 1900,
        month2 = local->tm_mon + 1, day2 = local->tm_mday;
    // get release version
    devel1 = total_year_day(year1, year2) - total_day(year1, month1, day1) +
             total_day(year2, month2, day2);
    snprintf(str, 57, "Xeinit devel Version:%s.%d\n", devel, devel1);
    freopen(filename, "a", stdout);
    // put all output in the release file
    // output:development branch time in xe-release
    printf("%4d-%02d-%02d ", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);
    printf("%s.%d build:%d\n", devel, devel1, xeapi1(local));
    fclose(stdout);
    return;
}

void json_config_init(json &user_data)
{
    data1 = user_data;
}
