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
/* FIXME: this doesn't make any sense - szFilename[0] is set to 0
 * within the function. I think we should reassert this static variable's
 * usefulness.
 */
static wchar_t szFilename[_MAX_PATH + 1] = L"\0";

wchar_t *
plibc_ChooseFileW(wchar_t *pszTitle, unsigned long ulFlags)
{
  OPENFILENAMEW theDlg;
  
  memset(&theDlg, 0, sizeof(theDlg));
  szFilename[0] = L'\0';
  
  theDlg.lStructSize = sizeof(theDlg);
  theDlg.hwndOwner = GetActiveWindow();
  theDlg.lpstrFile = szFilename;
  theDlg.nMaxFile = _MAX_PATH;
  theDlg.Flags = ulFlags;
  
  if (GetOpenFileNameW(&theDlg))
    return wcsdup (theDlg.lpstrFile);
  return NULL;
}

/**
 * @brief Displays a dialog box enabling the user to select a file
 * @param pszTitle the dialog's title
 * @param ulFlags the dialog's flags, see http://msdn.microsoft.com/library/en-us/winui/winui/windowsuserinterface/userinput/commondialogboxlibrary/commondialogboxreference/commondialogboxstructures/openfilename.asp
 * @return the selected filename or NULL
 */
char *
plibc_ChooseFile(char *pszTitle, unsigned long ulFlags)
{
  wchar_t *wTitle = NULL;
  wchar_t *wfn = NULL;
  char *fn = NULL;
  int r;
  if (pszTitle != NULL)
  {
    if (plibc_utf8_mode() == 1)
      r = strtowchar (pszTitle, (wchar_t **) &wTitle, CP_UTF8);
    else
      r = strtowchar (pszTitle, (wchar_t **) &wTitle, CP_ACP);
    if (r < 0)
      return NULL;
  }
  wfn = plibc_ChooseFileW (wTitle, ulFlags);
  if (wTitle != NULL)
    free (wTitle);
  if (wfn == NULL)
    return NULL;
  if (plibc_utf8_mode() == 1)
    r = wchartostr (wfn, &fn, CP_UTF8);
  else
    r = wchartostr (wfn, &fn, CP_ACP);
  free (wfn);
  if (r < 0)
    return NULL;
  return fn;
}

/* end of choosefile.c */
