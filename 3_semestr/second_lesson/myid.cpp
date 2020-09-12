#include <limits.h>
#include <grp.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <pwd.h>
#include <assert.h>
#include <ctype.h>

int PrintGroup(gid_t gid)
{
  printf("%u(", gid);
  
  group* gr_struct = getgrgid(gid);
   
  if (gr_struct == NULL)
  {
    printf("getgrgid error on [%u] group\n", gid);      
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
    printf("getpwuid error\n");
    return 0;
  }
  
  return pw_struct;
}

passwd* GetStructByUID(uid_t uid)
{
  passwd* pw_struct = getpwuid(uid);
  if (pw_struct == NULL)
  {
    printf("getpwuid error\n");
    return 0;
  }
  
  return pw_struct;
}

bool GiveInfo(uid_t uid, gid_t gid)
{
  // UID
  printf("uid=%u(", uid);
    
  passwd* pw_struct = getpwuid(uid);
  if (pw_struct == NULL)
  {
    printf("getpwuid error\n");
    return false;
  }
  printf("%s) ", pw_struct->pw_name);

    
  // GID
  printf("gid=");
  PrintGroup(gid);
  putchar(' ');


  // GROUPS
  gid_t list[NGROUPS_MAX] = {};
    
  int num_groups = getgroups(NGROUPS_MAX, list);
    
  if (num_groups == -1)
  {
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
      if (!PrintGroup(list[i]))
        return false;
    }
  }

  return true;
}
  

int main(int argc, char** argv)
{
    
  if (argc == 2)
  {
    uid_t uid = 0;
    gid_t gid = 0;

    if (IsNumber(argv[1]))
    {
      sscanf(argv[1], "%u", &uid);
      gid = GetStructByUID(uid)->pw_gid;
    }
    else 
    {
      uid = GetStructByName(argv[1])->pw_uid;
      assert(uid);
      
      gid = GetStructByName(argv[1])->pw_gid;
    }
    assert(GiveInfo(uid, gid));

  }

  if (argc == 1)
  {
    assert(GiveInfo(getuid(), getgid()));
  }

  return 0;
}
