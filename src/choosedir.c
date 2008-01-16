/*
     This file is part of PlibC.
     (C) 2005 Nils Durner (and other contributing authors)

	   This library is free software; you can redistribute it and/or
	   modify it under the terms of the GNU Lesser General Public
	   License as published by the Free Software Foundation; either
	   version 2.1 of the License, or (at your option) any later version.
	
	   This library is distributed in the hope that it will be useful,
	   but WITHOUT ANY WARRANTY; without even the implied warranty of
	   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	   Lesser General Public License for more details.
	
	   You should have received a copy of the GNU Lesser General Public
	   License along with this library; if not, write to the Free Software
	   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/**
 * @file src/choosedir.c
 * @brief Common dialogs to choose files
 */

#include "plibc_private.h"
#include <shlobj.h>

/**
 * @brief Displays a dialog box enabling the user to select a directory or a file
 * @param pszTitle the dialog's title
 * @param ulFlags the dialog's flags, see http://msdn.microsoft.com/library/en-us/shellcc/platform/shell/reference/structures/browseinfo.asp
 * @return the selected filename or NULL
 */
char *plibc_ChooseDir(char *pszTitle, unsigned long ulFlags)
{
	BROWSEINFO info;
	char *fn = NULL;
  LPITEMIDLIST pidl;
	
	memset(&info, 0, sizeof(BROWSEINFO));
	info.hwndOwner = GetActiveWindow();
	info.pszDisplayName = fn;
	info.lpszTitle = pszTitle;
	info.ulFlags = ulFlags;

	if ((pidl = SHBrowseForFolder(&info))) {
		fn = malloc(_MAX_PATH + 1);
    
		if (!SHGetPathFromIDList(pidl, fn)) {
			free(fn);
			fn = NULL;
		}

		CoTaskMemFree(pidl);
	}
	
	return fn;
}

/* end of choosedir.c */
