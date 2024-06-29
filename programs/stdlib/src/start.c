#include "peachos.h"

extern int main(int argc, char** argv);

/** @brief the function starting point of program
 * and call main function by providing processs arguments
 * 
 * @note this function is standard library function
 * therefore, somebody must provide as main function
*/
void c_start()
{
    struct process_arguments arguments;
    maeros_process_get_arguments(&arguments);

    int res = main(arguments.argc, arguments.argv);
    if (res == 0)
    {
        
    }
}