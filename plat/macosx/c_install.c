/*
 * Trivial -install and -uninstall support for MacOS X
 * a test main() is at the end of this file.
 *
 * int macosx_uninstall(const char *argv0, int quietly);
 * int macosx_install(const char *argv0, int argc, const char *argv[], 
 *                      int quietly); // argv[1..(argc-1)] are boot options
 * Both functions return 0 on success, else -1.
 *
 * $Id: c_install.c,v 1.1.2.2 2002/03/27 21:57:23 mfeiri Exp $
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <dirent.h>
#include <errno.h>

#define STARTUPITEMS_HOST "/Library/StartupItems"
#define STARTUPITEMS_DIR (STARTUPITEMS_HOST"/dnetc")
#define START_SCRIPT_PATH (STARTUPITEMS_HOST"/dnetc/dnetc")
#define START_PARAM_PATH (STARTUPITEMS_HOST"/dnetc/StartupParameters.plist")

#ifdef __cplusplus /* to ensure gcc -lang switches don't mess this up */
extern "C" {
#endif
int macosx_uninstall(const char *argv0, int quietly);
int macosx_install(const char *argv0, int argc, const char *argv[], 
                  int quietly); /* argv[1..(argc-1)] are boot options */
#ifdef __cplusplus
}
#endif

static int my_fchmod(int fd, int mode)
{
  int (*my_fchmode)(int,int);
  my_fchmode = (int (*)(int,int))fchmod; /* work around broken .h */
  return (*my_fchmode)( fd, mode);
}

static int create_start_script(const char *script_name,
                               const char *basename,
                               const char *appname,
                               int argc, const char *argv[],
                               int quietly)
{
  int i, n; FILE *file;
  const char *p;
  int got_ini_opt = 0;

  file = fopen(script_name,"w");
  if (!file)
  {
    if (!quietly)
      fprintf(stderr,"%s:\tUnable to create '%s'\n\t%s\n",
                     basename, script_name, strerror(errno) );
    return -1;
  }
  fprintf(file,
    "#!/bin/sh\n"
    "\n"
    ". /etc/rc.common\n"
    "\n"
    "# distributed.net client startup script.\n"
    "# generated by '%s -install'. Use '%s -uninstall' to\n"
    "# stop the client from being started automatically.\n"
    "#\n"
    "# Don't forget to change buffer/.ini file perms if you wish to run suid'd.\n"
    "\n"
    "CLIENT_DIR=\"", basename, basename );
  p = strrchr(appname,'/');    
  if (!p) 
    fprintf(file,"./");
  else
  {
    while (appname < p)
      fputc(*appname++, file);
    appname = p+1;
  }  
  fprintf(file,"\"\n"  
    "CLIENT_FNAME=\"%s\"\n"
    "CLIENT_OPTS=\"", appname );
  n = 0;
  for (i=1;i<argc;i++)
  {
    const char *p = argv[i];
    if (*p == '-' && p[1] == '-') p++; 
    if (strcmp(p,"-quiet")==0 || strcmp(p,"-hide")==0)
      continue;
    if (strcmp(p,"-ini")==0)
    {
      if ((++i)==argc)
        break;
      got_ini_opt = 1;
      fprintf(file,"%s-ini",(((++n)==1)?(""):(" ")) );
    }
    if (strchr(argv[i],' ') || strchr(argv[i],'\t'))
      fprintf(file,"%s\'%s\'",(((++n)==1)?(""):(" ")), argv[i]);
    else  
      fprintf(file,"%s%s",(((++n)==1)?(""):(" ")), argv[i]);
  }    
//  if (!got_ini_opt)
//    fprintf(file,"%s-ini '${CLIENT_DIR}/${CLIENT_FNAME}.ini'",(((++n)==1)?(""):(" ")) );

  fprintf( file,"\" # specify any optional switches for dnetc\n"
    "CLIENT_RUNAS=\"%s\" # specifiy the owner of the dnetc process\n", getlogin());

  fprintf( file,"\n"
    "if [ -x \"${CLIENT_DIR}/${CLIENT_FNAME}\" ]; then\n"
    "  # first announce what we are going to do.\n"
    "  ConsoleMessage \"Starting distributed.net client\"\n"
    "  # stop any other instances that may be running.\n"
    "  \"${CLIENT_DIR}/${CLIENT_FNAME}\" -quiet -shutdown\n"
    "  # change directory so we get nicer ps listings.\n"
    "  wd=`pwd`\n"
    "  chdir \"${CLIENT_DIR}\"\n"
    "  # now launch the client (with -quiet so it fork()s into the background)\n"
    "  sudo -u ${CLIENT_RUNAS} ./${CLIENT_FNAME} -quiet ${CLIENT_OPTS}\n"
    "  chdir \"$wd\"\n"
    "else\n"
    "  # announce what happened.\n"
    "  ConsoleMessage \"distributed.net client disabled\"\n"
    "fi\n"
    "\n"
  );

  my_fchmod( fileno(file), 0775); /* -rwxrwxr-x */
  fclose(file);
  return 0;
}


static int create_startup_params_file(const char *stparams_filename,
                                      const char *basename, int quietly)
{
  FILE *file = fopen(stparams_filename,"w");
  if (!file)
  {
    if (!quietly)
      fprintf(stderr,"%s:\tUnable to create '%s'\n\t%s\n",
                     basename, stparams_filename, strerror(errno) );
    return -1;
  }

  fprintf(file,
  "{\n"
  "  Description     = \"distributed.net client\";\n"
  "  Provides        = (\"dnetc\");\n"
  "  Requires        = (\"Disks\", \"Resolver\");\n"
  "  OrderPreference = \"None\";\n"
  "  Messages =\n"
  "  {\n"
  "    start = \"Starting distributed.net client\";\n"
  "    stop  = \"Stopping distributed.net client\";\n"
  "  };\n"
  "}\n" );

  my_fchmod(fileno(file),0664); /* -rw-rw-r-- */
  fclose(file);
  return 0;
}


#if 0
static int destroy_start_script(const char *script_name,
                                const char *basename,
                                const char *appname,
                                int quietly)
{
  int i, n; FILE *file;
  int got_ini_opt = 0;

  file = fopen(script_name,"w");
  if (!file)
  {
    if (!quietly)
      fprintf(stderr,"%s:\tUnable to create '%s'\n\t%s\n",
                     basename, script_name, strerror(errno) );
    return -1;
  }
  fprintf(file,
    "#! /bin/sh\n"
    "#\n"
    "# the distributed.net client is no longer configured to start automatically\n"
    "# run '%s -install' to reconstruct this startup script\n"
    "#\n", appname
  );

  my_fchmod( fileno(file), 0775); /* -rwxrwxr-x */
  fclose(file);
  return 0;
}
#endif

/* ------------------------------------------------------------------ */

static const char *get_basename(const char *argv0)
{
  const char *p = strrchr(argv0, '/');
  return (p)?(p+1):(argv0);
}


int macosx_install(const char *argv0, int argc, const char *argv[], 
                  int quietly) /* argv[1..(argc-1)] are boot options */
{
  int rc = -1;
  char fullpath[MAXPATHLEN];
  const char *basename = get_basename(argv0);
  const char *appname = realpath(argv0, fullpath);

  if (!appname)
  {
    if (!quietly)
      fprintf(stderr,"%s:\tUnable to determine full path to '%s'\n",
                     basename, argv0 );
  }
  else
  {
    const char *basepath = STARTUPITEMS_DIR;
    const char *hostpath = STARTUPITEMS_HOST;
    int dir_created = 0;

    rc = mkdir(hostpath,0775);
    if ((rc == 0) || (errno == EEXIST))
        rc = 0;
    else if (!quietly)
      fprintf(stderr,"%s:\tUnable to create directory '%s'\n\t%s\n",
                     basename, hostpath, strerror(errno) );

    if (rc == 0)
    {
    rc = mkdir(basepath, 0775);
    if (rc == 0)
      dir_created = 1;
    else if (errno == EEXIST)
      rc = 0;
    else if (!quietly)
      fprintf(stderr,"%s:\tUnable to create directory '%s'\n\t%s\n",
                     basename, basepath, strerror(errno) );
    }
    
    if (rc == 0)
    {
      rc = -1;
      if (create_start_script(START_SCRIPT_PATH, 
                              basename, appname, argc, argv, quietly) == 0)
      {
        int reinstalled = (access(START_PARAM_PATH,0)==0);
        rc = 0;
        if (!reinstalled)
          rc = create_startup_params_file(START_PARAM_PATH, basename, quietly);
        if (rc != 0)
          unlink(START_SCRIPT_PATH);
   
        if (rc == 0 &&!quietly)
        {
          fprintf(stderr,
          "%s:\tThe client has been sucessfully %sinstalled\n"
          "\tand will be automatically started on system boot.\n"
          "\t*** Please ensure that the client is configured ***\n", 
           basename, ((!reinstalled)?(""):("re-")) );
        }
      }
    }
    if (rc != 0 && dir_created)
      rmdir(basepath);
  }        
  return rc;
}

static int get_ftype(const char *fname)
{
  struct stat statblk;
  if (lstat(fname, &statblk) != 0)
  {
    if (stat(fname, &statblk) != 0)
      return -1;
  }    
  if ((statblk.st_mode & S_IFMT) == S_IFLNK)
    return 'l';
  if ((statblk.st_mode & S_IFMT) == S_IFDIR)
    return 'd';
  return 'f';
}

static int recursive_delete(const char *basename, 
                            const char *basepath, int quietly)
{
  int rc = 0;
  struct dirent *dp;
  DIR *dir;

  dir = opendir(".");
  if (!dir)
  {
    if (!quietly)
      fprintf(stderr,
       "%s:\tUnable to list directory entries\n\t%s\n",
            basename, strerror(errno) );
    rc = -1;    
  }
  else
  {
    while ((dp = readdir(dir)) != ((struct dirent *)0))
    {
      if (strcmp(dp->d_name,".") != 0 && strcmp(dp->d_name,"..") != 0)
      {
        int typ = get_ftype(dp->d_name);
        rc = -1;
        if (typ == -1)
        {
          if (!quietly)
            fprintf(stderr,
             "%s:\tUnable to stat %s\n\t%s\n", basename, 
                  dp->d_name, strerror(errno) );
        }
        else if (typ == 'f' || typ == 'l')
        {
          rc = unlink(dp->d_name);
          if (rc != 0 && !quietly)
            fprintf(stderr,
             "%s:\tUnable to unlink %s\n\t%s\n", basename, 
                  dp->d_name, strerror(errno) );
        }
        else if (typ == 'd')
        {
          rc = chdir(dp->d_name);
          if (rc != 0 && !quietly)
             fprintf(stderr,
             "%s:\tUnable to enumerate %s\n\t%s\n", basename, 
                  dp->d_name, strerror(errno) );
          if (rc == 0)        
          {
            rc = recursive_delete(basename,dp->d_name,quietly);
          }
          if (rc == 0)
          {
            rc = chdir("..");
            if (rc != 0 && !quietly)
              fprintf(stderr,
               "%s:\tUnable to chdir ..\n\t%s\n", basename, 
                    strerror(errno) );
          }
          if (rc == 0)
          {
            rc = rmdir(dp->d_name);
            if (rc != 0 && !quietly)
              fprintf(stderr,
               "%s:\tUnable to rmdir %s\n\t%s\n", basename, 
                    dp->d_name, strerror(errno) );
          }
        }
        if (rc != 0)
          break;
      } /* if (strcmp(dp->d_name,".") != 0 && strcmp(dp->d_name,"..") != 0) */
    } /* while (readdir) */
    closedir(dir);
  }
  basepath = basepath; /* may be unused */
  return rc;    
}
       
int macosx_uninstall(const char *argv0, int quietly)
{
  int rc = -1;
  const char *basename = get_basename(argv0);
  const char *basepath = STARTUPITEMS_DIR;
  char curdir[PATH_MAX];

  if (getcwd(curdir,sizeof(curdir))!=0)
    strcpy(curdir,"..");

  if (chdir(basepath)!=0)
  {
    if (errno == ENOENT)
    {
      if (!quietly)
        fprintf(stderr,
         "%s:\tThe client was not previously installed and therefore\n"
         "\tcannot be uninstalled.\n", basename );
      rc = 0; /* treat as success */
    }
    else if (!quietly)
      fprintf(stderr,
        "%s:\tUnable to change directory to '%s'\n\t%s\n",
        basename, basepath, strerror(errno));
  }
  else
  {
    rc = recursive_delete(basename, basepath, quietly);
    chdir(curdir);
    if (rc == 0)
    {
      if (rmdir(basepath) != 0)
      {
        rc = -1;
        if (!quietly)
          fprintf(stderr,
            "%s:\tUnable to remove directory '%s'\n\t%s\n",
            basename, basepath, strerror(errno));
      }
      else if (!quietly)
      {
        fprintf(stderr,
           "%s:\tThe client has been sucessfully uninstalled and \n"
           "\twill no longer be automatically started on system boot.\n",
            basename );
      }
    }
  }
  return rc;
}   

#if 0
int main(int argc,char *argv[])
{
  if (argc > 1 && strcmp(argv[1],"-uninstall")==0)
    return macosx_uninstall(argv[0],0);
  else if (argc == 1)    
    return macosx_install(argv[0], argc, (const char **)argv, 0);
  return 0;
}
#endif