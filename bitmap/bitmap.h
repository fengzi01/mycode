#ifndef _BITMAP_H
#define _BITMAP_H 
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace mycode {
    class bitmap {
        private:
            int *_bitmap;
            int _size;
            /**
             * 创建内存
             */
            bool malloc(int size);
        public:
            bitmap():_bitmap(NULL),_size(0){}
            ~bitmap() {
                if (NULL != _bitmap) {
                    free();
                }
            }

            bool create(int max_num);
            /**
             * 析构内存
             */
            bool free();

            bool set(int num);
            bool unset(int num);
            bool test(int num);
    };
}
#endif /* ifndef _BITMAP_H */
