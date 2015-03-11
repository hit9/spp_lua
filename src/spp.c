#include "spp.h"

/**
 * New a spp parser.
 */
spp_t *
spp_new()
{
    spp_t *spp = malloc(sizeof(spp_t));
    if (spp == NULL)
        return NULL;
    spp->buf = hbuf_new(SPP_HBUF_UNIT);
    spp->priv = NULL;
    spp->handler = NULL;
    return spp;
}

/**
 * Free a spp parser and its data.
 */
void
spp_free(spp_t *spp)
{
    hbuf_clear(spp->buf);

    if (spp->buf != NULL)
        hbuf_free(spp->buf);

    if (spp != NULL)
        free(spp);
}

/**
 * Clear s spp parser (its buf)
 */
void
spp_clear(spp_t *spp)
{
    hbuf_clear(spp->buf);
}

/**
 * Feed a spp parser with data.
 */
int
spp_feed(spp_t *spp, char *data)
{
    int res = hbuf_puts(spp->buf, data);

    if (res == HBUF_ENOMEM)
        return SPP_ENOMEM;
    return SPP_OK;
}

/**
 * Parse data.
 */
int
spp_parse(spp_t *spp)
{
    uint8_t *start = spp->buf->data;
    uint8_t *end = spp->buf->data + spp->buf->size;
    uint8_t *ptr = start;
    long id = 0;
    long len = spp->buf->size;

    while(len > 0) {
        uint8_t *ch = (uint8_t *)memchr(ptr, '\n', len);

        if (ch == NULL) break;

        ch += 1;
        int dis = ch - ptr;

        if(dis == 1 || (dis == 2 && ptr[0] == '\r')) {
            hbuf_lrm(spp->buf, ch - start);
            return SPP_OK;
        }

        if (ptr[0] < '0' || ptr[0] > '9') {
            return SPP_EBADFMT;
        }

        uint8_t size_str[20] = {0};

        if (dis > (int)sizeof(size_str) - 1) {
            return SPP_EBADFMT;
        }

        memcpy(size_str, ptr, dis - 1); // no '\n'

        int sz = atoi((const char *)size_str);

        if (sz < 0) {
            return SPP_EBADFMT;
        }

        len -= dis + sz;
        ptr += dis + sz;

        if (len < 0 || ptr > end) break;

        if (len >= 1 && ptr[0] == '\n') {
            len -= 1;
            ptr += 1;
        } else if (len >= 2 && ptr[0] == '\r' && ptr[1] == '\n') {
            len -= 2;
            ptr += 2;
        } else {
            break;
        }

        (spp->handler)(spp, (char *)ch, sz, id++);
    }
    return SPP_EUNFINISH;
}
