#ifndef __UGID_LIB_VAR
#define __UGID_LIB_VAR

#define _DEFAULT_SOURCE
#define _XOPEN_SOURCE

typedef struct spwd spwd;

#include <pwd.h>
#include <grp.h>
#include <ctype.h>
#include <shadow.h>

//#include "memlib.h"
#include "errlib.h"

char *
userNameFromId(uid_t uid);

uid_t
userIdFromName(const char *name);

gid_t
groupIdFromName(const char *name);

spwd *
getShawdowFromName(const char *name);

#endif