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
 * Our Wordbook is a singly linked list. And _wordbookentry is one of its
 * nodes.
 */
typedef struct _wordbookentry{  struct _wordbookentry* next;
  char *entry;
  int value;
} wordbookentry;

/*
 * Parse the wordbookfile provided and create the wordbook linked list.
 */
wordbookentry* parseWordbook(int wordbookfile){
  wordbookentry *wordbook = 0;

  int bytesread = 0;
  char byte;

  char *entry = calloc(50, sizeof(char));
  char number[] = "\0\0\0\0\0";
  //  int state = 0;
  int ecounter=0;

  // Read the whole wordbookfile byte by byte.
  while((bytesread = read(wordbookfile, &byte,1))>0){
    // letters are added to the current word; newlines result in putting
    // the current word into the linked list and starting with a new word.
    if(byte >= 'A' && byte <= 'Z'){
      *(entry+ecounter) = byte-'A'+'a';
      ecounter++;
    }
    if(byte >= 'a' && byte <= 'z'){
      *(entry+ecounter) = byte;
      ecounter++;
    }
    if(byte == '\n'){
      ecounter=0;
      wordbookentry *tmp = malloc(sizeof(wordbookentry));
      if(tmp == 0){
	fprintf(stderr,"Can't parse wordbook. Running out of memory.\n");
      }
      if(wordbook != 0){
	(*tmp).next = wordbook;
      }
      wordbook = tmp;

      (*tmp).entry = entry;
      entry = calloc(50,1);

      // Each word gets a weight.
      // Right now we assign the same weight to each word.
      // In this weight we could incorporate the frequence of words.
      (*tmp).value = 1;
      
    }
  }
  return wordbook;
}
