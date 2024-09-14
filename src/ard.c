#include <scl/ard.h>
#include <scl/calt.h>




SCL_EXPORT_API_BEFORE_TYPE
ARDRule SCL_EXPORT_API_AFTER_TYPE ard_find_rule(const uint8_t* data, uint8_t key)
{
    ARDRule rule = {0, 0};
    while (1)
    {
        uint8_t head = *data;
        uint8_t hkey = head & ARD_KEY_MASK;
        rule.value = data[1];
        rule.id_after = 0;
        if (head & ARD_HIA)
            rule.id_after = (++data)[1];
        if (hkey == key || hkey == 0)
            break;
        data += 2;
    }
    return rule;
}
