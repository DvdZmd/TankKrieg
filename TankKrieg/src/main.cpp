#include "Krieg.h"

int main(int argc, char** argv)
{
    Krieg krieg;
    if (!krieg.Initialize()) return -1;

    krieg.Run();
    krieg.Shutdown();
    return 0;
}
