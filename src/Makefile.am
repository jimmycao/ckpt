AM_CPPFLAGS = -I /usr/local/blcr-0.8.5/include
#AM_CFLAGS = -g -Wall 
LDADD = -L/usr/local/blcr-0.8.5/lib/ -lcr 

bin_PROGRAMS = main-bin ckpt-modeloff-bin
#ckpt_modeloff_bin_SOURCES = loop.c log_utils.c

main_bin_SOURCES = log_utils.c cr.c cr_main.c 
ckpt_modeloff_bin_SOURCES = loop_callback.c log_utils.c