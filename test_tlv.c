#include <stdio.h>
#include "tlv.h"

int main(void)
{
    uint8_t buf[64];
    size_t used = 0;

    used += TLV_SET(buf + used, sizeof(buf) - used, 1, "hello", 5);
    used += TLV_SET(buf + used, sizeof(buf) - used, 2, "world!", 6);
    uint32_t num = 12345;
    used += TLV_SET(buf + used, sizeof(buf) - used, 3, &num, 4);
    printf("used = %zu\n", used);

    uint16_t type, len;
    const uint8_t *val;
    TLV_FOREACH(buf, used, type, len, val) {
        printf("type=%u len=%u val=%.*s\n", type, len, len, val);
    }
    return 0;
}
