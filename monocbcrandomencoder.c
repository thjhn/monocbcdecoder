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
#include <time.h>

int main(int argc, char *argv[]){
  // check whether we have enough parameters.
  if(argc < 3){
    fprintf(stderr,"Wrong number of parameters!\n");
    fprintf(stderr,"Usgae: monocbcrandomencoder plaintext_filename output_filename\n");
    return -1;
  }

  // initialize random seed
  srand ( time(NULL) );

  // the key array states which leter replaces A, B, ...
  // initially it is the stupid key.
  char key[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

  // shuffle the key array.
  int i;
  for(i=0; i<26; i++){
    int change = rand()%26;
    char tmp = key[i];
    key[i] = key[change];
    key[change] = tmp;
  }
  
  // print out the generated key
  for(i=0;i<26;i++) printf("%c  ",key[i]);
  printf("\n");

  // open input and output files
  int fd_in = open(argv[1],O_RDONLY);
  int fd_out = open(argv[2],O_WRONLY|O_CREAT,0644);
  int fd_out_n = open(argv[3],O_WRONLY|O_CREAT,0644);

  // CBC needs an initial value. We pick a random one.
  char last=rand()%26+'A';

  // Encrypt
  int bytesread;
  char *buf = malloc(512);
  while((bytesread = read(fd_in,buf,512))>0){
    int i;
    for(i=0;i<bytesread;i++){
      if( (*(buf+i) >= 'A') && (*(buf+i) <= 'Z')){
	*(buf+i) = *(buf+i)-'A'+'a';
	write(fd_out_n,buf+i,1);
	*(buf+i) = (*(buf+i)-'a'+last-'A'+260)%26+'A';
	write(fd_out,buf+i,1);
	last = *(buf+i)-'a'+'A';
      }
      if( *(buf+i) >= 'a' && *(buf+i) <= 'z'){
	write(fd_out_n,buf+i,1);
	*(buf+i) = (*(buf+i)-'a'+last-'A'+260)%26+'A';
	write(fd_out,buf+i,1);
	last = *(buf+i)-'a'+'A';
      }
    }
  }
  
  return 0;
}
