#include <limits.h>
#include <grp.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <pwd.h>
#include <assert.h>
#include <ctype.h>


///////////////////////////////////////////////////
//
// Print info about group in stdout in format: 
//  <pw_uid(pw_name)>
//
// RETURN: 0 - if all is good
//        -1 - in case of getgrgid() error
//
///////////////////////////////////////////////////
int PrintGroup(gid_t gid);

///////////////////////////////////////////////////
//
// Check is buf is number
//
// RETURN: false - buf it is not a number
//         true  - buf is number
//
///////////////////////////////////////////////////
bool IsNumber(char* buf);

///////////////////////////////////////////////////
//
// Return passwd* struct by pw_name
//
// RETURN: passwd* - if all is okey
//         NULL    - in case of getpwnam() error
//
///////////////////////////////////////////////////
passwd* GetStructByName(const char* name);

///////////////////////////////////////////////////
//
// Return passwd* struct by pw_uid
//
// RETURN: passwd* - if all is okey
//         NULL    - in case of getpwuid error
//
///////////////////////////////////////////////////
passwd* GetStructByUID(uid_t uid);

///////////////////////////////////////////////////
//
// Print info about user in stdout in <id> linux 
//  command style
//
// RETURN: true  - if all is okey
//         false - in case of error
//
///////////////////////////////////////////////////
bool GiveInfo(uid_t uid, gid_t gid);




int PrintGroup(gid_t gid) 
{
  printf("%u(", gid);
  
  group* gr_struct = getgrgid(gid);
  if (gr_struct == NULL)
  {
    printf("PrintGroup(%u) error\n", gid);
    return -1;
  }
  
  printf("%s)", gr_struct->gr_name);
  return 0;
}  

bool IsNumber(char* buf)
{
  assert(buf);

  for (size_t i = 0; buf[i] != '\0'; ++i)
    if (!isdigit(buf[i])) return 0;
  
  return 1;
}

passwd* GetStructByName(const char* name)
{
  assert(name);
  
  passwd* pw_struct = getpwnam(name);
  if (pw_struct == NULL)
  {
    printf("getpwuid(%s) error\n", name);
    return NULL;
  }
  
  return pw_struct;
}

passwd* GetStructByUID(uid_t uid)
{
  passwd* pw_struct = getpwuid(uid);
  if (pw_struct == NULL)
  {
    printf("getpwuid(%u) error\n", uid);
    return NULL;
  }
  
  return pw_struct;
}

bool GiveInfo(uid_t uid, gid_t gid)
{
  // UID
  printf("uid=%u(", uid);

  passwd* pw_struct = GetStructByUID(uid);
  if (pw_struct == NULL)
    return false;
  printf("%s) ", pw_struct->pw_name);

  
  // GID
  printf("gid=");
  PrintGroup(gid);
  putchar(' ');


  // GROUPS
  int ngroups = NGROUPS_MAX;
  char* name  = pw_struct->pw_name;
  gid_t list[NGROUPS_MAX];

  int num_groups = getgrouplist(name, gid, list, &ngroups);
    
  if (num_groups == -1)
  {
    perror("Sorry");
    printf("getgroups error\n");
    return false;
  }

  if (num_groups > 0)
  { 
    printf("groups=");
    PrintGroup(gid); // put the primary group in the beginning of the line, 
                     // not in the end, as getgroups() give

    for (int i = 0; i < num_groups - 1; ++i)
    {
      putchar(',');
      if (PrintGroup(list[i]))
        return false;
    }
  }

  return true;
}
  


int main(int argc, char** argv)
{
  // info about parameter user
  if (argc == 2)
  {
    uid_t uid = 0;
    gid_t gid = 0;

    if (IsNumber(argv[1]))                      //
    {                                           //
      sscanf(argv[1], "%u", &uid);              //
                                                //
      passwd* pw_struct = GetStructByUID(uid);  // ./myid <number>
      if (!pw_struct)                           //
        goto ERROR;                             //
                                                //
      gid = pw_struct->pw_gid;                  //
    }                                           //
    else 
    {                                               //
      passwd* pw_struct = GetStructByName(argv[1]); //
      if (!pw_struct)                               //
        goto ERROR;                                 // ./myid <name>
                                                    //
      uid = pw_struct->pw_uid;                      //
      gid = pw_struct->pw_gid;                      //
    }                                               //

    if (!GiveInfo(uid, gid))
      goto ERROR;
  }
  
  // info about current process user
  if (argc == 1)                        //
  {                                     //
    if (!GiveInfo(getuid(), getgid()))  // ./myid
      goto ERROR;                       //
  }                                     //

  return 0;

ERROR:
  printf("Some errors occurs during the program. Maybe, user with this parameters: <%s> is not exist\n", argv[1]);
  return -1;
}
