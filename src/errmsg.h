/*
 * Error routines
 * Print message and return error code
 */

#ifndef ERROR_H
#define ERROR_H

int err_alloc(void);
int err_no_arg(const char *, const char *);
int err_no_preset(const char *, const char *);
int err_no_opt(const char *, const char *);
int err_parse_color(const char *, const char *);
int err_parse_json(const char *, const char *);
int err_no_file(const char *, const char *);
int err_json_root_not_array(void);
int err_json_no_object(const char *);
int err_json_not_string(const char *);
int err_json_not_int(const char *);
int err_json_not_number(const char *);
int err_json_not_array(const char *);
int err_json_parse_color(const char *);

#endif
