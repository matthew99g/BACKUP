#ifndef __UGID_LIB
#define __UGID_LIB

#include "ugidlibvar.h"

char *
userNameFromId(uid_t uid)
{
    struct passwd *pwd;

    pwd = getpwuid(uid);

    return (pwd == NULL) ? NULL : pwd->pw_name;
}

uid_t
userIdFromName(const char *name)
{
    struct passwd *pwd;
    uid_t u;
    char *endptr;

    if (name == NULL || *name == '\0')
        return -1;

    // Allow numerical string
    u = strtol(name, &endptr, 10);
    if (*endptr == '\0')
        return u;

    pwd = getpwnam(name);
    
    return (pwd == NULL) ? -1 : pwd->pw_uid;
}

gid_t
groupIdFromName(const char *name)
{
    struct group *grp;
    gid_t g;
    char *endptr;

    if (name == NULL || *name == '\0')
        return -1;

    g = strtol(name, &endptr, 10);
    if (*endptr == '\0')
        return g;
    
    grp = getgrnam(name);
    
    return (grp == NULL) ? -1 : grp->gr_gid;
}

spwd *
getShawdowFromName(const char *name)
{
    struct spwd *sPwd;
    char *endptr;
    unsigned long s;

    if (name == NULL || *name == '\0')
        return NULL;

    s = strtol(name, &endptr, 10);
    if(endptr == NULL || *endptr == '\0')
        return NULL;

    if (s)
        sPwd = getspnam(endptr);
    else
        sPwd = getspnam(name);

    return (sPwd == NULL) ? NULL : sPwd;
}

#endif