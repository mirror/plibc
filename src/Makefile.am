INCLUDES = -I$(top_srcdir)/src/include

lib_LTLIBRARIES = libplibc.la

SUBDIRS = include .

libplibc_la_LDFLAGS = \
 -export-dynamic -Wl,--no-undefined -Wl,--export-all-symbols -lole32 \
 -lshell32 -luuid -lstdc++ -lcomdlg32 -lgdi32 \
 -version-info 1:0:0

libplibc_la_SOURCES = \
 access.c \
 atoll.c \
 chdir.c \
 chmod.c \
 choosedir.c \
 choosefile.c \
 close.c \
 ctime.c \
 creat.c \
 errno.c \
 flock.c \
 fopen.c \
 fread.c \
 fstat.c \
 fsync.c \
 fwrite.c \
 gettimeofday.c \
 inet_pton.c \
 intl.c \
 inet_ntop.c \
 langinfo.c \
 lseek.c \
 mkstemp.c \
 mmap.c \
 open.c \
 opendir.c \
 path.c \
 pid.c \
 pipe.c \
 plibc.c \
 printf.c \
 random.c \
 read.c \
 readlink.c \
 realpath.c \
 registry.c \
 remove.c \
 rename.c \
 rmdir.c \
 select.c \
 shortcut.cc \
 socket.c \
 stat.c \
 statfs.c \
 strerror.c \
 string.c \
 strptime.c \
 symlink.c \
 truncate.c \
 unlink.c \
 write.c


