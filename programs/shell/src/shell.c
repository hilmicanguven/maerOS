#include "shell.h"
#include "stdio.h"
#include "stdlib.h"
#include "peachos.h"
int main(int argc, char** argv)
{
    print("MaerOS v1.0.0\n");
    while(1)
    {
        print(argv[0]);
        for (int i = 0; i < 1000000; i++)
        {
            
        }
    }
    // while(1) 
    // {
    //     // print("> MaerOS v1.0.0");
    //     // char buf[1024];
    //     // peachos_terminal_readline(buf, sizeof(buf), true);
    //     // print("\n");
    //     // // maeros_process_load_start(buf);
    //     // maeros_system_run(buf);
        
    //     print("\n");
    // }
    return 0;
}