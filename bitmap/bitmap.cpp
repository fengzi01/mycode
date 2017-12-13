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
    bool bitmap::create(int max_num) {
        if (max_num <= 0) {
            return false;
        }

        _size = max_num / 32 + 1;
        return malloc(_size);
    }

    bool bitmap::malloc(int size) {
        if (size <= 0) {
            return false;
        }
        if (_bitmap != NULL) {
            free();
        }
        _bitmap = (int *)::malloc(sizeof(int)*size);
        // bzero 
        memset(_bitmap, 0, size * sizeof(int));

        return true;
    }

    bool bitmap::free() {
        if (NULL != _bitmap) {
            ::free(_bitmap);
            _bitmap = NULL;
        }
        return true;
    }

    bool bitmap::set(int num) {
        int idx_of_loc = num >> 5;
        int idx_of_bit = num % 32;
        
        if (idx_of_loc >= _size) {
            return false; 
        }
        
        _bitmap[idx_of_loc] = _bitmap[idx_of_loc] | 1 << idx_of_bit;
        return true;
    }

    bool bitmap::unset(int num) {
        int idx_of_loc = num >> 5;
        int idx_of_bit = num % 32;
        
        if (idx_of_loc >= _size) {
            return false; 
        }
        
        if (test(num)) {
            // 异或
            _bitmap[idx_of_loc] = _bitmap[idx_of_loc] ^ 1 << idx_of_bit;
        }
        return true;
    }

    bool bitmap::test(int num) {
        int idx_of_loc = num >> 5;
        int idx_of_bit = num % 32;
        int i = 1 << idx_of_bit;

        int flag = _bitmap[idx_of_loc] & i;

        return flag > 0;
    }
}
