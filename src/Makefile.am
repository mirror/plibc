INCLUDES = -I$(top_srcdir)/src/include

lib_LTLIBRARIES = libplibc.la

SUBDIRS = include .

libplibc_la_LDFLAGS = \
 -export-dynamic -no-undefined -export-all-symbols -lole32 \
 -lshell32 -lcomdlg32 -lgdi32 -lws2_32 -Wl,-luuid \
 -version-info 1:0:0

libplibc_la_SOURCES = \
 access.c \
 chdir.c \
 chmod.c \
 choosedir.c \
 choosefile.c \
 close.c \
 closedir.c \
 ctime.c \
 creat.c \
 errno.c \
 fclose.c \
 flock.c \
 fopen.c \
 fread.c \
 fstat.c \
 fsync.c \
 fwrite.c \
 gmtime_r.c \
 kill.c \
 hsearch.c \
 hsearch_r.c \
 inet_pton.c \
 intl.c \
 inet_ntop.c \
 langinfo.c \
 lsearch.c \
 lseek.c \
 mkstemp.c \
 mmap.c \
 open.c \
 opendir.c \
 path.c \
 pid.c \
 pipe.c \
 plibc.c \
 plibc_strconv.c \
 plibc_strconv.h \
 printf.c \
 random.c \
 read.c \
 readdir.c \
 readlink.c \
 realpath.c \
 registry.c \
 remove.c \
 rename.c \
 resolv_ms.c \
 rmdir.c \
 select.c \
 shortcut.c \
 socket.c \
 stat.c \
 statfs.c \
 str-two-way.h \
 strcasestr.c \
 strerror.c \
 string.c \
 strptime.c \
 symlink.c \
 sysconf.c \
 truncate.c \
 tsearch.c \
 unlink.c \
 write.c


