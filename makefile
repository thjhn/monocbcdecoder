# Copyright (c) 2012, 2016 Thomas Jahn <thomas@t-und-j.de>
#
# This file is part of monocbcdecoder.
#
# monocbcdecoder is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# monocbcdecoder is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with monocbcdecoder.  If not, see <http://www.gnu.org/licenses/>.

all: monocbcdecoder monocbcrandomencoder

monocbcrandomencoder:
	gcc -std=c99 -o monocbcrandomencoder monocbcrandomencoder.c

monocbcdecoder: wordbook.o evaluations.o codehandler.o monocbcdecoder.c
	gcc -std=c99 -o monocbcdecoder monocbcdecoder.c wordbook.o evaluations.o codehandler.o

wordbook.o: wordbook.c
	gcc -std=c99 -c wordbook.c

evaluations.o: evaluations.c
	gcc -std=c99 -c evaluations.c

codehandler.o: codehandler.c
	gcc -std=c99 -c codehandler.c
