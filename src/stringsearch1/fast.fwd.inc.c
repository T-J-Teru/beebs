/*
	search routine generated by gen.
	skip=fast, match=fwd (using fwdr), shift=inc
*/
/*
 * The authors of this software are Andrew Hume and Daniel Sunday.
 *
 * Copyright (c) 1991 by AT&T and Daniel Sunday.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose without fee is hereby granted, provided that this entire notice
 * is included in all copies of any software which is or includes a copy
 * or modification of this software and in all copies of the supporting
 * documentation for such software.
 *
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY.  IN PARTICULAR, NEITHER THE AUTHORS NOR AT&T MAKE ANY
 * REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
 * OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 *

   Copyright (C) 2013 Embecosm Limited and University of Bristol

   Contributor: James Pallister <james.pallister@bristol.ac.uk>

   This file is part of the Bristol/Embecosm Embedded Energy Benchmark Suite.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>. */


#ifndef	CHARTYPE
#define	CHARTYPE	unsigned char
#endif
#define	MAXPAT	256

#ifndef	TABTYPE
#define	TABTYPE	long
#endif
typedef TABTYPE Tab;

#include <string.h>
#include <stdlib.h>

static struct
{
	int patlen;
	CHARTYPE pat[MAXPAT];
	Tab delta[256];
	int lastchar;
} pat;

void
prep1(CHARTYPE *base, register int m)
{
	CHARTYPE *skipc;
	register CHARTYPE *pe, *p;
	register int j;
	register Tab *d;

	pat.patlen = m;
	if(m > MAXPAT)
		abort();
	memcpy(pat.pat, base, m);
	skipc = 0;
	d = pat.delta;
	for(j = 0; j < 256; j++)
		d[j] = pat.patlen;
	for(p = pat.pat, pe = p+m-1; p < pe; p++)
		d[*p] = pe-p;
	pat.lastchar = *p;
	skipc = (CHARTYPE *)p;
}

int
exec1(CHARTYPE *base, int n)
{
	int nmatch = 0;
	register CHARTYPE *e, *s;
	register Tab *d0 = pat.delta;
	int lastdelta;
	register int k;
	register CHARTYPE *p, *q;
	register CHARTYPE *ep;
	register int n1 = pat.patlen-1;

	lastdelta = n+pat.patlen;
	d0[pat.lastchar] = lastdelta;	/* guaranteed to break s < e loop */
	s = base+pat.patlen-1;
	e = base+n;
	ep = pat.pat + pat.patlen-1;
	while(s < e){
#ifdef	STATS
		for(;;){
			stats.jump++;
			if((s += (k = d0[*s])) >= e) break;
			stats.step[k]++;
		}
		if(s < e+pat.patlen)
			stats.step[k]++;
#else
		while((s += d0[*s]) < e)
			;
#endif
		if(s < e+pat.patlen)	/* no match */
			break;
		s -= lastdelta;
#ifdef	STATS
		stats.slow++;
#endif
		for(p = pat.pat, q = s-n1; p < ep; ){
#ifdef	STATS
			stats.cmp++;
#endif
			if(*q++ != *p++)
				goto mismatch;
		}
		nmatch++;
	mismatch:
		s++;
#ifdef	STATS
		stats.step[1]++;
#endif
	}
	return(nmatch);
}
