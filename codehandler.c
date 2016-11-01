/*
 * Copyright (c) 2012, 2016 Thomas Jahn <thomas@t-und-j.de>
 *
 * This file is part of monocbcdecoder.
 *
 * monocbcdecoder is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * monocbcdecoder is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with monocbcdecoder.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

/*
 * Extract letters out of a textfile. Convert them to uppercase.
 */
char *parsefile(char *filename){
  // Open file and determine filsize
  int fd = open(filename,O_RDONLY);
  struct stat filestat;
  stat(filename, &filestat);
  long filesize = filestat.st_size;
  
  // allocate memory to store file
  char *cryptogram = malloc(filesize+1);

  // parse File
  int i=0;
  char symbol;

  while(read(fd,&symbol,1) != 0){
    if(symbol >= 'A' && symbol <= 'Z'){
      *(cryptogram+i)=symbol;
      i++;
    }
    if(symbol >= 'a' && symbol <= 'z'){
      *(cryptogram+i)=symbol-'a'+'A';
      i++;
    }
  }
  *(cryptogram+i)=0;
  
  close(fd);
  return cryptogram;
};

/*
 * Make a copy of *text.
 */
char *copyCode(char const *text){
  // count the number of necessary bytes
  int count = 0;
  while(*(text+count) != 0){
    count++;
  }
  char *newtext = malloc(count);
  if(newtext < 0) return newtext;

  for(count = count; count >= 0; count--){
    *(newtext+count) = *(text+count);
  }

  return newtext;
}
