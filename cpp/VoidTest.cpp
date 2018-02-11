class NullStream {
    public:
        NullStream() = default;
        ~NullStream() = default;

        template<class T>
        inline NullStream &operator<<(T t){(void)t;return *this;}
};
#define LOG NullStream()
int main(int argc, char *argv[])
{
    int a = 0;
    float b = 0;
    LOG << a << b;
    return 0;
}
