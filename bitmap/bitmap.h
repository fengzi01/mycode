#ifndef _BITMAP_H
#define _BITMAP_H 
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "stdint.h"

// 定义类型

namespace mycode {
    class bitmap {
        private:
            uint32_t *_bitmap;
            uint64_t _size;
            /**
             * 创建内存
             */
            bool _malloc(uint64_t size);
        public:
            bitmap():_bitmap(NULL),_size(0){}
            ~bitmap() {
                if (NULL != _bitmap) {
                    destroy();
                }
            }

            bool create(uint64_t max_num);
            /**
             * 析构内存
             */
            bool destroy();

            bool set(uint64_t num);
            bool unset(uint64_t num);
            bool test(uint64_t num);
    };
}
#endif /* ifndef _BITMAP_H */
