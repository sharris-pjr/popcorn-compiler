#include <dirent.h>
#include <unistd.h>
#include "__dirent.h"
#include "libc.h"

int closedir(DIR *dir)
{
	int ret = close(dir->fd);
	pfree(dir);
	return ret;
}
