#ifndef STREQ_H
#define STREQ_H

#define streq(s1, s2)       (strcmp(s1, s2) == 0)
#define streqor(s1, s2, s3) (streq(s1, s2) || streq(s1, s3))

#endif