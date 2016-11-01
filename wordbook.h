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


typedef struct _wordbookentry{  struct _wordbookentry* next;
  char *entry;
  int value;
} wordbookentry;

wordbookentry* parseWordbook(int);
