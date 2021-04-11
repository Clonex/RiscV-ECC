#include <hal.h>
#include <sendfn.h>

int main(void)
{
    hal_setup(CLOCK_FAST);
    send_start();
    send_string("strings", "You can send \"escaped\"\n strings!");
    send_string("strings", "Hey bro!");
    send_stop();
    return 0;
}
