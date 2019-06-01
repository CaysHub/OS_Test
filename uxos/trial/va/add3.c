#include <stdio.h>
#include <stdlib.h>
typedef char *va_list;

#define va_start(ap, last_arg) 						                      \
    (ap = ((char *)&(last_arg) + sizeof(last_arg)))

#define va_end(ap)

#define va_arg(ap, type)						                           \
    (ap += sizeof(type),					                               \
    *((type *)(ap - sizeof(type))))

int add(char *format, ...)
{
    va_list ap;
    char ch;
    int sum = 0;
    va_start(ap,format);
    while(ch=*format++){
        switch (ch)
        {
        case 'd':
            sum += va_arg(ap, int);
            break;
        case 's':
            sum += atoi(va_arg(ap, char*));
            break;
        default:
            break;
        }
    }
    return sum;
}

int main()
{
    printf("sum = %d\n", add("%d %d", 11, 22));
    printf("sum = %d\n", add("%s %s", "11", "22"));
    printf("sum = %d\n", add("%d %s", 11, "22"));
    return 0;
}
