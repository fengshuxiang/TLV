#ifndef TLV_H
#define TLV_H

/*
 * TLV: [type:2字节][len:2字节][value:len字节]，连续存放多个。
 *
 *   used += TLV_SET(buf + used, left, 1, "hello", 5);
 *   TLV_FOREACH(buf, buflen, type, len, val) {
 *       // type / len / val 直接可用
 *   }
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define TLV_HDR_SIZE 4

/* 读取 pos 处 TLV 的字段 */
#define TLV_TYPE(buf, pos)  ((uint16_t)((buf)[pos] << 8 | (buf)[(pos) + 1]))
#define TLV_LEN(buf, pos)   ((uint16_t)((buf)[(pos) + 2] << 8 | (buf)[(pos) + 3]))
#define TLV_VAL(buf, pos)   (&(buf)[(pos) + TLV_HDR_SIZE])
#define TLV_TOTAL(buf, pos) ((size_t)TLV_HDR_SIZE + TLV_LEN(buf, pos))

/* SET: 在 buf 起始处写一个 TLV，返回写入字节数（0 = 空间不足）。 */
#define TLV_SET(buf, buf_size, type, val, val_len)                     \
    ({                                                                 \
        size_t _n = (val_len), _ret = 0;                               \
        if (TLV_HDR_SIZE + _n <= (buf_size)) {                          \
            (buf)[0] = (uint8_t)((type) >> 8); (buf)[1] = (uint8_t)(type); \
            (buf)[2] = (uint8_t)(_n >> 8);    (buf)[3] = (uint8_t)_n;  \
            memcpy(&(buf)[TLV_HDR_SIZE], (val), _n);                    \
            _ret = TLV_HDR_SIZE + _n;                                  \
        }                                                             \
        _ret;                                                         \
    })

/*
 * FOREACH: 遍历 buf 中所有 TLV，循环体直接用 type / len / val。
 *   uint16_t type, len; const uint8_t *val;
 *   TLV_FOREACH(buf, buflen, type, len, val) { ... }
 */
#define TLV_FOREACH(buf, buflen, type, len, val)                          \
    for (size_t _pos = 0;                                               \
         _pos + TLV_HDR_SIZE <= (buflen) &&                                \
         (type = TLV_TYPE(buf, _pos),                                   \
          len  = TLV_LEN(buf, _pos),                                    \
          val  = TLV_VAL(buf, _pos),                                    \
          _pos + TLV_TOTAL(buf, _pos) <= (buflen));                        \
         _pos += TLV_TOTAL(buf, _pos))

#endif /* TLV_H */
