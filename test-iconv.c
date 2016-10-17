#include <stdlib.h>
#include <iconv.h>
#include <wchar.h>
#include <string.h>
#include <stdio.h>

int wchar_to_utf8(const wchar_t *src, char *dst) {
	int len = wcslen(src);
	if (sizeof(dst) < (len + 1))
		return -1;
	memset(dst, 0, len + 1);

	char *iconv_in = (char *)src;
	char *iconv_out = (char *)dst;

	size_t iconv_in_bytes = (len + 1) * sizeof(wchar_t);
	size_t iconv_out_bytes = sizeof(dst);

	iconv_t cd = iconv_open("UTF-8", "WCHAR_T");
	if ((iconv_t) -1 == cd)
		return -1;

	size_t ret = iconv(cd, &iconv_in, &iconv_in_bytes,
			&iconv_out, &iconv_out_bytes);
	if ((size_t) -1 == ret)
		return -1;

	return 0;
}

FILE *w_fopen(const wchar_t *path, const wchar_t *mode) {
	char path_utf8[wcslen(path) + 1];
	char  mode_utf8[wcslen(mode) + 1];

	if (wchar_to_utf8(path, path_utf8) != 0)
		return NULL;
  printf("path: %s\n", path_utf8);
	if (wchar_to_utf8(mode, mode_utf8) != 0)
		return NULL;
  printf("mode: %s\n", mode_utf8);

	return fopen(path_utf8, mode_utf8);
}

int main()
{
  wchar_t path[] = L"a.out";
  char utf8[wcslen(path) + 1];
  FILE *f = w_fopen(L"alabalaporto.o", L"rb");
  if (f == NULL)
    printf("conversion failed\n");
  else
    printf("success\n");

  fclose(f);

  return 0;
}
