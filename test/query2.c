/*
	Audio File Library

	Copyright 1998-2000, Michael Pruett <michael@68k.org>

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License as
	published by the Free Software Foundation; either version 2 of
	the License, or (at your option) any later version.

	This program is distributed in the hope that it will be
	useful, but WITHOUT ANY WARRANTY; without even the implied
	warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
	PURPOSE.  See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public
	License along with this program; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
	MA 02111-1307, USA.
*/

#ifdef __USE_SGI_HEADERS__
#include <dmedia/audiofile.h>
#include <dmedia/audioutil.h>
#else
#include <audiofile.h>
#include <aupvlist.h>
#endif

#include <stdio.h>
#include <stdlib.h>

const char *paramtypename (int paramtype);

#ifdef DEBUG
#define DEBG printf
#else
#define DEBG
#endif

int main (int ac, char **av)
{
	AUpvlist	formatlist;
	long		*flist;
	long		*larray;
	long		lvalue;
	int			i, formatcount;

	formatlist = afQuery(AF_QUERYTYPE_FILEFMT, AF_QUERY_IDS, 0, 0, 0);
	formatcount = afQueryLong(AF_QUERYTYPE_FILEFMT, AF_QUERY_ID_COUNT, 0, 0, 0);

	DEBG("formatcount = %d\n", formatcount);

	AUpvgetval(formatlist, 0, &flist);
	AUpvfree(formatlist);

	for (i=0; i<formatcount; i++)
	{
		long	format;
		char	*formatstring;

		format = flist[i];
		DEBG("format = %ld\n", format);
		formatstring = afQueryPointer(AF_QUERYTYPE_FILEFMT, AF_QUERY_NAME,
			format, 0, 0);
		DEBG("format = %s\n", formatstring);

		lvalue = afQueryLong(AF_QUERYTYPE_INST, AF_QUERY_SUPPORTED,
			format, 0, 0);
		DEBG("instrument query: supported: %ld\n", lvalue);

		lvalue = afQueryLong(AF_QUERYTYPE_INST, AF_QUERY_MAX_NUMBER,
			format, 0, 0);
		DEBG("instrument query: maximum number: %ld\n", lvalue);

		lvalue = afQueryLong(AF_QUERYTYPE_INSTPARAM, AF_QUERY_SUPPORTED,
			format, 0, 0);
		DEBG("instrument parameter query: supported: %ld\n", lvalue);

		lvalue = afQueryLong(AF_QUERYTYPE_INSTPARAM, AF_QUERY_ID_COUNT,
			format, 0, 0);
		DEBG("instrument parameter query: id count: %ld\n", lvalue);

		larray = afQueryPointer(AF_QUERYTYPE_INSTPARAM, AF_QUERY_IDS,
			format, 0, 0);

		if (larray != NULL)
		{
			int	i;
			for (i=0; i<lvalue; i++)
			{
				int	paramtype;

				DEBG("instrument parameter query: id: %ld\n", larray[i]);
				paramtype = afQueryLong(AF_QUERYTYPE_INSTPARAM,
					AF_QUERY_TYPE, format, larray[i], 0);
				DEBG("	type of parameter: %s\n",
					paramtypename(paramtype));
				DEBG("	name of parameter: %s\n",
				       (char *)
					afQueryPointer(AF_QUERYTYPE_INSTPARAM, AF_QUERY_NAME,
						format, larray[i], 0));
			}
			free(larray);
		}
	}
	free(flist);

	return 0;
}

const char *paramtypename (int paramtype)
{
	static const char	*longname = "long";
	static const char	*doublename = "double";
	static const char	*pointername = "pointer";

	switch (paramtype)
	{
		case AU_PVTYPE_LONG:
			return longname;
		case AU_PVTYPE_DOUBLE:
			return doublename;
		case AU_PVTYPE_PTR:
			return pointername;
	}

	return NULL;
}