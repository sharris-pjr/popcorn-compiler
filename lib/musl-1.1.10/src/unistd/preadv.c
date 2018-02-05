#define _BSD_SOURCE
#include <sys/uio.h>
#include <unistd.h>
#include "syscall.h"
#include "libc.h"

ssize_t __preadv(int fd, const struct iovec *iov, int count, off_t ofs)
{
	return syscall_cp(SYS_preadv, fd, iov, count,
		(long)(ofs), (long)(ofs>>32));
}

LFS64(__preadv);
weak_alias(__preadv, preadv);