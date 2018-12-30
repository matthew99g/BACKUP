#ifndef __GET_NM
#define __GET_NM

#include <pwd.h>
#include <grp.h>
#include <ctype.h>

char *
getUserFromUid(int uid)
{
    struct passwd *pwd;

    pwd = getpwuid(uid);
    return (pwd == 0) ? 0 : pwd->pw_name;
}

#endif