/*
     This file is part of PlibC.
     (C) 2005, 2006 Nils Durner (and other contributing authors)

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
 * @file src/choosefile.c
 * @brief Common dialog to choose files
 */

#include "plibc_private.h"
#include <commdlg.h>

/* Remember the previously selected path */
static char szFilename[_MAX_PATH + 1] = "\0";

/**
 * @brief Displays a dialog box enabling the user to select a file
 * @param pszTitle the dialog's title
 * @param ulFlags the dialog's flags, see http://msdn.microsoft.com/library/en-us/winui/winui/windowsuserinterface/userinput/commondialogboxlibrary/commondialogboxreference/commondialogboxstructures/openfilename.asp
 * @return the selected filename or NULL
 */
char *plibc_ChooseFile(char *pszTitle, unsigned long ulFlags)
{
  OPENFILENAME theDlg;
  
  memset(&theDlg, 0, sizeof(OPENFILENAME));
  szFilename[0] = '\0';
  
  theDlg.lStructSize = sizeof(OPENFILENAME);
  theDlg.hwndOwner = GetActiveWindow();
  theDlg.lpstrFile = szFilename;
  theDlg.nMaxFile = _MAX_PATH;
  theDlg.Flags = ulFlags;
  
  if (GetOpenFileName(&theDlg))
    return strdup(theDlg.lpstrFile);
  else
    return NULL;
}

/* end of choosefile.c */
