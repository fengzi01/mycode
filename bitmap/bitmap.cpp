#include "bitmap.h"

/*
 * 求十进制数0-N对应的在数组a中的下标
 * index_loc = N / 32即可，index_loc即为n对应的数组下标。例如n = 76, 则loc = 76 / 32 = 2,因此76在a[2]中。
 *
 * 求十进制数0-N对应的bit位
 * bit_loc = N % 32即可，例如 n = 76, bit_loc = 76 % 32 = 12
 *
 * space = max_num / 32 + 1;
 *
 */

namespace mycode {
    bool bitmap::create(uint64_t max_num) {
        _size = (uint64_t)((max_num >> 5) + 1);

        printf("size:%lu\n",_size);
        return _malloc(_size);
    }

    bool bitmap::_malloc(uint64_t size) {
        if (_bitmap != NULL) {
            destroy();
        }
        _bitmap = (uint32_t *)::malloc(sizeof(uint32_t)*size);
        printf("_bitmap:%p\n",_bitmap);
        if (!_bitmap) {
            printf("malloc mem fail.\n");
            ::free(_bitmap);
        }
        // bzero 
        memset(_bitmap, 0, size * sizeof(uint32_t));
        return true;
    }

    bool bitmap::destroy() {
        if (NULL != _bitmap) {
            ::free(_bitmap);
            _bitmap = NULL;
        }
        return true;
    }

    bool bitmap::set(uint64_t num) {
        uint64_t idx_of_loc = num >> 5;  // num/32
        uint32_t idx_of_bit = num & 0x1F; // num % 32
        
        if (idx_of_loc >= _size) {
            return false; 
        }
        
        _bitmap[idx_of_loc] = _bitmap[idx_of_loc] | 1 << idx_of_bit;
        return true;
    }

    bool bitmap::unset(uint64_t num) {
        uint64_t idx_of_loc = num >> 5;
        uint32_t idx_of_bit = num & 0x1F;
        
        if (idx_of_loc >= _size) {
            return false; 
        }
        
        if (test(num)) {
            // 异或
            _bitmap[idx_of_loc] = _bitmap[idx_of_loc] ^ (1 << idx_of_bit);
        }
        return true;
    }

    bool bitmap::test(uint64_t num) {
        uint64_t idx_of_loc = num >> 5;
        uint32_t idx_of_bit = num & 0x1F;
        uint32_t i = 1 << idx_of_bit;

        uint32_t flag = _bitmap[idx_of_loc] & i;
        return flag > 0;
    }
}
