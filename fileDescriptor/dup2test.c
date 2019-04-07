#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char* argv[])
{
    int fd = open("hello.file", O_CREAT|O_RDWR|O_TRUNC,S_IRUSR|S_IWUSR);
    if(fd < 0)
    {
        printf("Open Error!!\n");
        return 0;
    }


    int nfd = dup2(fd, STDOUT_FILENO);
    if(nfd < 0)
    {
        printf("Error!!\n");
        return 0;
    }


    char buf[5];
    int n;


    while((n = read(STDIN_FILENO, buf, 5)) > 0)
        if(write(STDOUT_FILENO, buf, n) != n)
        {
            printf("Write Error!!\n");
            return 0;
        }
    return 0;
}
