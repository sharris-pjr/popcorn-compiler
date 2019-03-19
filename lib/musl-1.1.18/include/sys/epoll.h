#ifndef	_SYS_EPOLL_H
#define	_SYS_EPOLL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <sys/types.h>
#include <fcntl.h>

#define __NEED_sigset_t

#include <bits/alltypes.h>

#define EPOLL_CLOEXEC O_CLOEXEC
#define EPOLL_NONBLOCK O_NONBLOCK

enum EPOLL_EVENTS { __EPOLL_DUMMY };
#define EPOLLIN 0x001
#define EPOLLPRI 0x002
#define EPOLLOUT 0x004
#define EPOLLRDNORM 0x040
#define EPOLLRDBAND 0x080
#define EPOLLWRNORM 0x100
#define EPOLLWRBAND 0x200
#define EPOLLMSG 0x400
#define EPOLLERR 0x008
#define EPOLLHUP 0x010
#define EPOLLRDHUP 0x2000
#define EPOLLEXCLUSIVE (1U<<28)
#define EPOLLWAKEUP (1U<<29)
#define EPOLLONESHOT (1U<<30)
#define EPOLLET (1U<<31)

#define EPOLL_CTL_ADD 1
#define EPOLL_CTL_DEL 2
#define EPOLL_CTL_MOD 3

typedef union epoll_data {
	void *ptr;
	int fd;
	uint32_t u32;
	uint64_t u64;
} epoll_data_t;

struct epoll_event_x86_64 {
	uint32_t events;
	epoll_data_t data;
} __attribute__ ((__packed__));

struct epoll_event_common {
	uint32_t events;
	epoll_data_t data;
};

struct epoll_event {
	uint32_t events;
	epoll_data_t data;
};

union epoll_event_union {
	struct epoll_event_x86_64 x86_64;
	struct epoll_event_common common;
};

#ifdef __x86_64__
	#define epoll_arch x86_64
#else
	#define epoll_arch common
#endif

static inline void translate_epoll_event(struct epoll_event *usr, union epoll_event_union *kus)
{
	usr->events=kus->epoll_arch.events;
	usr->data=kus->epoll_arch.data;
}

static inline void translate_epoll_event_rev(struct epoll_event *usr, union epoll_event_union *kus)
{
	kus->epoll_arch.events=usr->events;
	kus->epoll_arch.data=usr->data;
}


int epoll_create(int);
int epoll_create1(int);
int epoll_ctl(int, int, int, struct epoll_event *);
int epoll_wait(int, struct epoll_event *, int, int);
int epoll_pwait(int, struct epoll_event *, int, int, const sigset_t *);


#ifdef __cplusplus
}
#endif

#endif /* sys/epoll.h */
