#if !defined(_PLIBC_STRCONV_H_)
#define _PLIBC_STRCONV_H_

/**
 * strtowchar:
 * @str: a string (UTF-8-encoded) to convert
 * @wretstr: a pointer to variable (pointer to wchar_t) to receive the result
 * @cp: codepage to convert from
 *
 * Allocates new wchar_t string, fills it with converted @str and writes the
 * pointer to it into @wretstr. If the function fails, *@wretstr remains
 * unmodified. Converts from UTF-8 to UTF-16
 * See http://msdn.microsoft.com/en-us/library/dd319072%28VS.85%29.aspx
 * MultiByteToWideChar() documentation for values of cp.
 * CP_ACP and CP_UTF8 are recommended.
 * Free the string returned in @wretstr with free() when it is no longer needed
 *
 * Returns:
 *  0 - conversion is successful
 * -1 - conversion failed at length-counting phase
 * -2 - conversion failed at memory allocation phase
 * -3 - conversion failed at string conversion phase
 */
int
strtowchar (const char *str, wchar_t **wretstr, UINT cp);

/**
 * wchartostr:
 * @wstr: a string (UTF-16-encoded) to convert
 * @wretstr: a pointer to variable (pointer to char) to receive the result
 * @cp: codepage to convert from
 *
 * Allocates new wchar_t string, fills it with converted @wstr and writes the
 * pointer to it into @retstr. If the function fails, *@retstr remains
 * unmodified. Converts from UTF-8 to UTF-16
 * See http://msdn.microsoft.com/en-us/library/dd319072%28VS.85%29.aspx
 * WideCharToMultiByte() documentation for values of cp.
 * CP_ACP and CP_UTF8 are recommended.
 * Free the string returned in @retstr with free() when it is no longer needed
 *
 * Returns:
 *  1 - conversion is successful, but some characters were replaced by placeholders
 *  0 - conversion is successful
 * -1 - conversion failed at length-counting phase
 * -2 - conversion failed at memory allocation phase
 * -3 - conversion failed at string conversion phase
 */
int
wchartostr (const wchar_t *wstr, char **retstr, UINT cp);


int
wchartostr_buf (const wchar_t *wstr, char *str, long str_len, UINT cp);

int
strtowchar_buf (const char *str, wchar_t *wstr, long wstr_size, UINT cp);

#endif /* !defined(_PLIBC_STRCONV_H_) */
