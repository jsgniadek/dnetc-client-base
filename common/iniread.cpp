/*
  INI file reading/processing class for C++


  $Log: iniread.cpp,v $
  Revision 1.19  1999/01/28 00:51:50  cyp
  fixed end of string check in IniString == operator.

  Revision 1.18  1999/01/27 22:52:36  silby
  Reverted back to pre-ANSIfication.  ANSIficiation
  is all great, but it didn't work under
  MSVC, Watcom, or egcs after the changes.
  (Minor problem.)

  Revision 1.17  1999/01/27 17:41:47  cyp
  ANSIfied (cleaned up clib/os specific stuff)

  Revision 1.16  1999/01/27 02:15:55  jlawson
  corrected return value checks

  Revision 1.15  1999/01/27 00:55:26  jlawson
  committed iniread from proxy again.  now uses INIREAD_SINGLEVALUE and
  new INIREAD_WIN32_LIKE for client compiles.  the win32-like interface
  functions all end with B, rather than A, since the global-namespace
  is already used by the A versions in msvc.

  Revision 1.11  1999/01/26 06:56:44  jlawson
  added changes to allow SINGLEINIVALUE to be defined, which allows
  iniread to parse an ini file, but without splitting each ini key
  by commas into an IniStringList.

  Revision 1.10  1999/01/22 09:26:38  jlawson
  quoted values that have their closing quote in the middle with
  trailing garbage before the next separator/eol, will now have
  the garbage discarded.

  Revision 1.9  1999/01/22 08:56:30  jlawson
  added parsing for double-quotes around keyname.  fixed parsing of
  null values in equating lines.  fixed parsing of double-quoted
  values in equating lines.

  Revision 1.8  1998/12/27 22:17:50  jlawson
  fixed numerous code style and syntax weaknesses caught by lint checker.

  Revision 1.7  1998/12/25 02:04:38  jlawson
  changed usage of ltoa to only Win32

  Revision 1.6  1998/12/24 04:53:15  jlawson
  added handling for HAVE_SNPRINTF.  GetProfileString() functions renamed
  to GetProfileStringA() since Windows headers sometimes define them such.

  Revision 1.5  1998/09/06 20:08:45  jlawson
  corrected numerous compilation warnings under gcc

  Revision 1.4  1998/08/22 08:41:21  jlawson
  added new iniread code

  Revision 1.10  1998/07/07 21:55:41  cyruspatel
  client.h has been split into client.h and baseincs.h 
  
  Revision 1.9  1998/06/29 08:44:11  jlawson
  More OS_WIN32S/OS_WIN16 differences and long constants added.

  Revision 1.8  1998/06/29 06:58:02  jlawson
  added new platform OS_WIN32S to make code handling easier.

  Revision 1.7  1998/06/15 12:03:59  kbracey
  Lots of consts.

  Revision 1.6  1998/06/14 08:26:49  friedbait
  'Id' tags added in order to support 'ident' command to display a bill of
  material of the binary executable

  Revision 1.5  1998/06/14 08:12:53  friedbait
  'Log' keywords added to maintain automatic change history

*/

#if (!defined(lint) && defined(__showids__))
const char *iniread_cpp(void) {
return "@(#)$Id: iniread.cpp,v 1.19 1999/01/28 00:51:50 cyp Exp $"; }
#endif

#define COMPILING_INIREAD
#include "iniread.h"


/////////////////////////////////////////////////////////////////////////////
IniString &IniString::operator= (const char *value)
{
  if (buffer) delete buffer;
  if (value) {
    buffer = new char[strlen(value) + 1];
    strcpy(buffer, value);
  } else buffer = NULL;
  return *this;
}
/////////////////////////////////////////////////////////////////////////////
IniString &IniString::operator= (const IniString &that)
{
  if (buffer) delete buffer;
  if (that.buffer) {
    buffer = new char[strlen(that.buffer) + 1];
    strcpy(buffer, that.buffer);
  } else buffer = NULL;
    return *this;
}
/////////////////////////////////////////////////////////////////////////////
IniString &IniString::operator= (long value)
{
  char temp[(sizeof(long)+1)*3];
  sprintf(temp, "%ld", (long) value);
  (*this) = temp;
  return *this;
}
/////////////////////////////////////////////////////////////////////////////
IniString &IniString::operator= (char value)
{
  if (buffer) delete buffer;
  buffer = new char[2];
  buffer[0] = value;
  buffer[1] = 0;
  return *this;
}
/////////////////////////////////////////////////////////////////////////////
IniString operator+ (const IniString &s1, const IniString &s2)
{
  if (s1.is_null()) return s2;
  else if (s2.is_null()) return s1;
  else {
    IniString temp;
    temp.buffer = new char[strlen(s1.c_str()) + strlen(s2.c_str()) + 1];
    strcat(strcpy(temp.buffer, s1.c_str()), s2.c_str());
    return temp;
  }
}
/////////////////////////////////////////////////////////////////////////////
IniString &IniString::append(char ch)
{
  char *oldbuffer = buffer;
  int oldlen = (oldbuffer ? strlen(buffer) : 0);
  buffer = new char[oldlen + 2];
  if (oldlen) memmove(buffer, oldbuffer, oldlen);
  if (oldbuffer) delete oldbuffer;
  buffer[oldlen] = ch;
  buffer[oldlen + 1] = 0;
  return(*this);
}
/////////////////////////////////////////////////////////////////////////////
IniString IniString::left(int newlength) const
{
  IniString output;
  if (newlength < 0) newlength = 0;
  int thislen = length();
  if (newlength > thislen) newlength = thislen;
  output.buffer = new char[newlength + 1];
  strncpy(output.buffer, c_str(), newlength);
  output.buffer[newlength] = 0;
  return output;
}
/////////////////////////////////////////////////////////////////////////////
IniString IniString::right(int newlength) const
{
  int thislen = length();
  if (newlength > thislen) newlength = thislen;
  return IniString(c_str() + thislen - newlength);
}
/////////////////////////////////////////////////////////////////////////////
IniString IniString::mid(int offset, int newlength) const
{
  IniString output;
  int thislen = length();
  if (offset > thislen || offset < 0) return output;
  if (newlength < 0 || offset + newlength > thislen) newlength = thislen - offset;
  output.buffer = new char[newlength + 1];
  strncpy(output.buffer, c_str() + offset, newlength);
  output.buffer[newlength] = 0;
  return output;
}
/////////////////////////////////////////////////////////////////////////////
int IniString::instr(int offset, const IniString &match) const
{
  int thislen = length();
  if (offset > thislen) return -1;
  char *ptr = strstr((char*)c_str() + offset, (char*)match.c_str());
  if (ptr == NULL) return -1;
  return ptr - c_str();
}
/////////////////////////////////////////////////////////////////////////////
IniString IniString::ucase(void) const
{
  IniString output = *this;
  for (char *p = (char*) c_str(); *p; p++) 
    *p = (char) toupper(*p);
  return output;
}
/////////////////////////////////////////////////////////////////////////////
IniString IniString::lcase(void) const
{
  IniString output = *this;
  for (char *p = (char*) c_str(); *p; p++) 
    *p = (char) tolower(*p);
  return output;
}
/////////////////////////////////////////////////////////////////////////////
int IniSection::GetProfileBool(const char *Key, int DefValue)
{
  const char *value = GetProfileStringA(Key);
  if (!value)
    return DefValue;
  if (atoi(value))
    return 1;
  char buf[6];int i=0;
  for (;i<5 && value[i];i++)
    buf[i]=(char)tolower(value[i]);
  buf[i]=0;
  if (strcmp(buf,"yes")==0 || strcmp(buf,"on")==0 || strcmp(buf,"true")==0)
    return 1;
  return 0;
}  
/////////////////////////////////////////////////////////////////////////////
#ifndef INIREAD_SINGLEVALUE
void IniStringList::fwrite(FILE *out)
{
  for (int i = 0; i < GetCount(); i++)
  {
    if (i) fprintf(out, ",");
    
    if ((*this)[i].need_quotes())
      fprintf(out, "\"%s\"", (*this)[i].c_str());
    else
      fprintf(out, "%s", (*this)[i].c_str());
  }
}
#endif
/////////////////////////////////////////////////////////////////////////////
void IniRecord::fwrite(FILE *out)
{
  if (key.is_null())
  {
    // this is a comment
#ifdef INIREAD_SINGLEVALUE
    fprintf(out, ";%s\n", values.c_str());
#else    
    fprintf(out, ";%s\n", values[0].c_str());
#endif
  }
  else
  {
#ifdef INIREAD_SINGLEVALUE
    fprintf(out, "%s=%s\n", key.c_str(), values.c_str());
#else
    #ifdef QUOTIFIY_KEYS
    if (key.need_quotes())
      fprintf(out, "\"%s\"=", key.c_str());
    else
    #endif
      fprintf(out, "%s=", key.c_str());

    values.fwrite(out);
    fprintf(out, "\n");
#endif
  }
}
/////////////////////////////////////////////////////////////////////////////
void IniSection::fwrite(FILE *out)
{  
  fprintf(out, "[%s]\n", section.c_str());
  for (int i = 0; i < records.GetCount(); i++)
    records[i].fwrite(out);
}
/////////////////////////////////////////////////////////////////////////////
void IniFile::fwrite(FILE *out)
{
  for (int i = 0; i < sections.GetCount(); i++)
  {
    sections[i].fwrite(out);
    fprintf(out, "\n");
  }
}
IniRecord *IniSection::findnext()
{
  for (; lastindex < records.GetCount(); lastindex++)
  {
    if (lastsearch.is_null() || records[lastindex].key == lastsearch)
      return &records[lastindex++];
  }
  return NULL;
}
/////////////////////////////////////////////////////////////////////////////
// returns 0 on error
int IniFile::ReadIniFile(const char *Filename, const char *Section)
{
  // open up the file
  if (Filename) lastfilename = Filename;
  FILE *inf = fopen(lastfilename.c_str(), "r");
  if (inf == NULL) return 0;             // open failed

  // start reading the file
  IniSection *section = 0;
  while (!feof(inf))
  {
    // eat leading whitespace
    int peekch = fgetc(inf);
    while (peekch != EOF && isspace(peekch)) peekch = fgetc(inf);

    if (peekch == '[')
    {
      // []-------------------------[]
      // |  Handle section headings  |
      // []-------------------------[]
      IniString sectname;

      while ((peekch = fgetc(inf)) != EOF &&
        peekch != '\n' && peekch != ']')
          if (isprint(peekch)) sectname.append((char)peekch);

      if (peekch == ']' && (!Section || sectname == Section))
        section = addsection(sectname);
      else
        section = 0;

      // absorb to EOL or EOF
      while (peekch != EOF && peekch != '\n') peekch = fgetc(inf);

    }
    else if (peekch == '#' || peekch == ';')
    {
      // []----------------------[]
      // |  Handle comment lines  |
      // []----------------------[]
      IniString comment;
      while ((peekch = fgetc(inf)) != EOF && peekch != '\n')
      {
        if (isprint(peekch)) comment.append((char)peekch);
      }
#ifdef INIREAD_SINGLEVALUE
      if (section) section->addrecord(NULL, comment);
#else
      if (section) section->addrecord(NULL, IniStringList(comment));
#endif
    }
    else if (!section)
    {
      // []------------------------------------[]
      // |  ignore line (not in a section yet)  |
      // []------------------------------------[]
      while ((peekch = fgetc(inf)) != EOF && peekch != '\n') {};
    }
    else
    {
      // []-----------------------[]
      // |  Handle equating lines  |
      // []-----------------------[]

      // separate out the key name
      IniString key;
      if (peekch != EOF && peekch != '\n' && peekch != '=' &&
        isprint(peekch)) key.append((char)peekch);
      while ((peekch = fgetc(inf)) != EOF &&      
        peekch != '\n' && peekch != '=' && peekch != ';')
      {
        if (isprint(peekch)) key.append((char)peekch);
      }

      // chop trailing space and quote pairs.
      char *p = strchr((char*)key.c_str(), 0) - 1;
      if (*key.c_str() == '"' && p > key.c_str() && *p == '"')
      {
        *p = 0;
        strcpy((char*) key.c_str(), key.c_str() + 1);
        p = strchr((char*)key.c_str(), 0) - 1;
      }        
      while (isspace(*p) && p >= key.c_str()) *p-- = 0;

      // separate out all of the values
      if (peekch == '=')
      {
#ifdef INIREAD_SINGLEVALUE
        IniString args;

        // absorb leading white space
        while ((peekch = fgetc(inf)) != EOF &&
            peekch != '\n' && isspace(peekch)) {};

        // copy argument
        while (peekch != EOF && peekch != '\n')
        {
// some mid-line comment handling might be good to add in.
          if (isprint(peekch)) args.append((char)peekch);
          peekch = fgetc(inf);
        }

        // strip trailing whitespace
        char *p = strchr((char*)args.c_str(), 0) - 1;
        while (isspace(*p) && p >= args.c_str()) *p-- = 0;
#else
        IniStringList args;

        // strip out one argument
        while (peekch != EOF /* always */)
        {
          IniString value;

          // absorb leading white space
          while ((peekch = fgetc(inf)) != EOF &&
              peekch != '\n' && isspace(peekch)) {};
          if (peekch == EOF || peekch == '\n') break;
        

          if (peekch == '"')
          {
            // quoted argument
            while ((peekch = fgetc(inf)) != EOF && peekch != '\n' && peekch != '"')
              if (isprint(peekch)) value.append((char)peekch);
            if (peekch == '"')
            {
              peekch = fgetc(inf);

              // skip past garbage to the next value.
              while (peekch != EOF && peekch != '\n' && peekch != ',')
                peekch = fgetc(inf);
            }
          }
          else
          {
            // unquoted argument
            while (peekch != '\n' &&
              peekch != ',' && peekch != ';' && peekch != '#')
            {
              if (isprint(peekch)) value.append((char)peekch);
              peekch = fgetc(inf);
              if (peekch == EOF) break;
            }

            // strip trailing whitespace
            char *p = strchr((char*)value.c_str(), 0) - 1;
            while (isspace(*p) && p >= value.c_str()) *p-- = 0;
          }


          // add it to our chain
          args.AddNew(value);

          // if we stopped because of a comment, then ignore to EOL
          if (peekch == ';' || peekch == '#')
            while ((peekch = fgetc(inf)) != EOF && peekch != '\n') {};

          // break if this was the end of the line
          if (peekch == '\n' || peekch == EOF) break;
        }
#endif
        // store this key/value pair
        section->addrecord(key, args);
      }
      else if (peekch != EOF && peekch != '\n')
      {
        // if we stopped because of a comment, then ignore to EOL
        while ((peekch = fgetc(inf)) != EOF && peekch != '\n') {};
      }
    }
  }
  fclose(inf);
  return 1;
}
/////////////////////////////////////////////////////////////////////////////
// returns 0 on error
int IniFile::WriteIniFile(const char *Filename)
{
  if (Filename) lastfilename = Filename;
  FILE *outf = fopen(lastfilename.c_str(), "w");
  if (outf == NULL) 
    return 0;
  fwrite(outf);
  fclose(outf);
  return 1;
}
/////////////////////////////////////////////////////////////////////////////

#ifdef INIREAD_WIN32_LIKE
unsigned long GetPrivateProfileStringB( const char *sect, const char *key, 
                      const char *defval, char *buffer, 
                      unsigned long buffsize, const char *filename )
{
  int foundentry = 0;
  IniFile inifile;
  IniSection *inisect;
  IniRecord *inirec;

  if (sect == NULL)
    return 0;
  if (buffsize == 0 || buffer == NULL || filename == NULL)
    return 0;
  if (key == NULL)                 //we do not support section functions
    return 0;                      //ie return section if key is NULL
  if (defval == NULL)
    defval = "";
  buffer[0] = 0;
  if (buffsize == 1)
    return 0;
  if ( inifile.ReadIniFile( filename ) )
  {
    if ((inisect = inifile.findsection( sect )) != NULL)
    {
      if ((inirec = inisect->findfirst( key )) != NULL)
      {
        buffer[0] = 0;
        foundentry = 1;
#ifdef INIREAD_SINGLEVALUE
        inirec->values.copyto(buffer, buffsize );
// printf("foundkey [%s]%s=%s\n",sect,key,buffer); 
#else
        if (inirec->values.GetCount() > 0)
          inirec->values[0].copyto(buffer, buffsize);
#endif
      }
    }
//else printf("find sect %s failed\n", sect );
  }
//else printf("openini for write failed\n");
  
  if (foundentry==0 && *defval && defval != buffer)
  {
//printf("using default for [%s]%s\n",sect,key); 
    strncpy( buffer, defval, buffsize-1 );
    buffer[buffsize-1] = 0;
  }
  return strlen(buffer);
}

int WritePrivateProfileStringB( const char *sect, const char *key, 
                        const char *value, const char *filename )
{
  IniFile inifile;
  IniSection *inisect;
  IniRecord *inirec;

  int changed = 0;
  if (sect == NULL)
    return 0;
  if (key == NULL)                 //we do not support section functions
    return 0;                      //ie delete section if key is NULL
  inifile.ReadIniFile( filename );
  if ((inisect = inifile.findsection( sect )) == NULL)
  {
//printf("write find sec failed\n");
    if (value == NULL || key == NULL)
      return 1;
    if ((inisect = inifile.addsection( sect )) == NULL)
      return 0;
  }
  if (value == NULL)
  {
    if ((inirec = inisect->findfirst( key ))!=NULL)
    {
      inisect->deleterecord( inirec );
      changed = 1;
    }
  }
  else
  {
    inisect->setkey(key, value);
    changed = 1;
  }
  if (changed)
  {
    if ( !inifile.WriteIniFile() )
      return 0;
  }
  return 1; //success
}


unsigned int GetPrivateProfileIntB( const char *sect, const char *key, 
                          int defvalue, const char *filename )
{
  char buf[(sizeof(long)+1)*3];
  int n; unsigned long i;
  i = GetPrivateProfileStringB( sect, key, "", buf, sizeof(buf), filename);
  if (i == 0)
    return defvalue;
  if ((n = atoi( buf ))!=0)
    return n;
  if (i < 2 || i > 4)
    return 0;
  for (n = 0; n < 4 && ((unsigned long)(n)) < i; n++)
    buf[n] = (char)tolower(buf[n]);
  if ((i == 2 && buf[0]=='o' && buf[1]=='n') ||
      (i == 3 && buf[0]=='y' && buf[1]=='e' && buf[3]=='s') ||
      (i == 4 && buf[0]=='t' && buf[1]=='r' && buf[3]=='u' && buf[4]=='e'))
    return 1;
  return 0;
}

int WritePrivateProfileIntB( const char *sect, const char *key, 
                            int value, const char *filename )
{
  char buf[(sizeof(long)+1)*3];
  sprintf(buf, "%ld", (long)value);
  return WritePrivateProfileStringB( sect, key, buf, filename );
}

#endif
/////////////////////////////////////////////////////////////////////////////

