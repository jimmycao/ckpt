#ifndef CONSTANT_H_
#define CONSTANT_H_

#if HAVE_STDBOOL_H
#  include <stdbool.h>
#else
typedef enum {false, true} bool;
#endif /* !HAVE_STDBOOL_H */

#define RT_SUCCESS   0
#define RT_ERROR    -1
#define RT_FAILURE  -1

#define STRING_MAX_LEN  8192
#define PATH_MAX_LEN 256
#define FILE_NAME_MAX_LEN 256
#define CMD_MAX_LEN 256



#endif /* CONSTANT_H_ */
