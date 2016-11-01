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

#include "wordbook.h"

/*
 * A rule consists of two letters to be substituted and one replacement
 * letter.
*/
typedef struct _rule{
  char precondition[3];
  char substitut;
} rule;

/*
 * Evaluate the current translation using the given wordbook.
 */
int eval(wordbookentry *wordbook, char const *text){
  int sumvalue = 0;
  wordbookentry *next = wordbook;
  
  // check the wordbook linearly
  while(next != 0){
    char *i;
    if((i = strstr(text, (*next).entry)) != 0){
      do{
	i = strstr(i+1, (*next).entry);
	if(i != 0){
	  sumvalue += (*next).value;
	}
      }while(i != 0);
    }
    
    next = (*next).next;
  }
  
  return sumvalue;
}

/*
 * Evaluate the use of the rule myRule.
 */
int evalrule(char const *code, char *partly, rule myRule,
	     wordbookentry *wordbook){
  int counter=0;
  int state=0;
  while( *(code+counter) != 0 ){
    if(*(code+counter) == myRule.precondition[1]){
      *(partly+counter) = ((myRule.precondition[0]+myRule.substitut-2*'A')
			   -state+'A'+260)%26+'a';
    }

    state=*(code+counter);
    counter++;
  }
  
  return eval(wordbook,partly);
}


/*
 * Create an initial set of rules.
 */
rule* createStartRules(char const *code){
  rule *rules = calloc(26,sizeof(rule));
  int distribution[26][26]={{0}};
  int i,j;
  
  char last = 0;
  while(*code != 0){
    if(last != 0){
      (distribution[last-'A'][*code-'A'])++;
    }
    last = *code;
    code++;
  }

  for(i=0;i<26;i++){
    int max=0;
    int second = 0;
    int maxAt = 0;
    for(j=0;j<26;j++){
      if(distribution[i][j] >= max){
	second = max;
	max = distribution[i][j];
	maxAt = j;
      }
    }
    // Replace the most frequent bigrams by E.
    if(max-second > 3){
      rules[i].precondition[0] = i+'A';
      rules[i].precondition[1] = maxAt+'A';
      rules[i].substitut = 'E';
    }else{
      rules[i].precondition[0] = 0;
      rules[i].precondition[1] = 0;
      rules[i].substitut = 0;
    }
    
  }
  return rules;
}
