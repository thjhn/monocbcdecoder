/*
 * Copyright (c) 2012, 2016 Thomas Jahn <thomas@t-und-j.de>
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
#include "evaluations.h"
#include "codehandler.h"

#define MAX_ROUNDS 2000 // No of tries we allow at most.

int main(int argc, char *argv[]){
  // Make sure we get the wordbook and the encrypted file as parameters.
  // In fact: If there is a fourth parameter (argc>=4) then we do not
  // print any intermediate results.
  if(argc < 3){
    fprintf(stderr,"Wrong number of parameters!\n");
    fprintf(stderr,"Usage: monocbcdecoder wordbook_filename enrypted_filename\n");
    return -1;
  }
  
  // load the wordbook
  // The wordbook is a plain text file and each line contains exactly
  // one natural word.
  int fd_wordbook = open(argv[1], O_RDONLY);
  wordbookentry *neu;
  neu = parseWordbook(fd_wordbook);
  close(fd_wordbook);
  // load the encrypted text
  char *code = parsefile(argv[2]);
  // and make a copy
  char *partly = copyCode(code);
  
  // Create a set of inital substitution rules.
  rule *rules=createStartRules(code);
  
  char b1, b2, s;
  
  int rounds=0;
  int lastscore = -1;
  int newscore = 0;

  // We add new substitutions as long as we are improving and as long
  // we do not exceed MAX_ROUNDS
  while((lastscore != newscore) && (rounds < MAX_ROUNDS)){
    rounds++;
    lastscore = newscore;

    int maxvalue = 0;
    rule bestrule;
    // As we are trying to decode CBC-mode we replace tuples [b1][b2] by s.
    // We try every possible replacement.
    // Note that we do not have to iterate over b1!
    b1='A';
    for(b2='A'; b2<='Z'; b2++){         
      for(s='A'; s<='Z'; s++){
	// Create a new rule.	
	rule r;
	r.precondition[0]=b1;
	r.precondition[1]=b2;
	r.substitut=s;
	
	// Make a copy of our text and evaluate the new rule.
	char *test = copyCode(partly);
	int value = evalrule(code, test, r, neu);
	// Keep track of the best rule we found so far.
	if(value > maxvalue){ bestrule = r; maxvalue = value;}
	// Delete the test rule.
	free(test);	
      }
    }

    // Compute the new score.
    // @TODO: Actually we had computed it in the loop. Why do it again?
    int value = evalrule(code,partly,bestrule,neu);
    // Print the intermediate result, if not opted out.
    if(argc<4) printf("Using %c%c->%c, scored %i, improvement %i\n",bestrule.precondition[0],bestrule.precondition[1],bestrule.substitut,value,value-newscore);
    newscore = value;
    if(argc<4) printf("%s\n\n",partly);
  }

  // Now we finished our task. Either we could no longer imporve our we
  // run out of MAX_ROUNDS. Print something nice...
  if(rounds == MAX_ROUNDS){
    if(argc<4) printf("Maximum number of encoding rounds reached. Thus encoding was stopped.\n");
  }else{
    if(argc<4) printf("Encoding converged. I hope having done a good job?\n");
  }
  if(argc>=4) printf("%s",partly);

  return 0;
}
