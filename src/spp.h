/*
 * Copyright (c) 2014-2015, hit9
 *
 * SPP - Simple Protocol Parser
 *
 *   Packet := Block+ '\n'
 *   Block  := Size '\n' Data '\n'
 *   Size   := literal_integer
 *   Data   := string_bytes
 *
 * Example
 *
 *   '2\nok\n\n'  - complete
 *    / => ['ok']
 */


#ifndef __SPP_H
#define __SPP_H

#include <stdint.h>
#include <stdlib.h>
#include "hbuf.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SPP_HBUF_UNIT 128  // .128kb

typedef enum {
    SPP_OK = 0,
    SPP_ENOMEM = -1,
    SPP_EBADFMT = -2,
    SPP_EUNFINISH = -3,
} spp_error_t;

typedef struct spp_st {
    hbuf_t *buf;   /* feed buf */
    void *priv;   /* user-settable arbitrary field */
    /* *
     * required handler, called with 4 arguments: spp itself,
     * received-data, received-data size and received-data index.
     */
    void (*handler)(struct spp_st*, char *, size_t, int);
} spp_t;

/* Public API for the protocol parser */
spp_t* spp_new();
int spp_feed(spp_t *, char *);
void spp_free(spp_t *);
int spp_parse(spp_t *);
void spp_clear(spp_t *);

#ifdef __cplusplus
}
#endif
#endif
