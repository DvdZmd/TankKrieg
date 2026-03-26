#include "Krieg.h"

/**
 * @brief Starts the game application lifecycle.
 * @param argc Number of command-line arguments supplied by the host process.
 * @param argv Command-line argument array supplied by the host process.
 * @return Zero when the game shuts down normally; otherwise -1.
 */
int main(int argc, char** argv)
{
    Krieg krieg;
    if (!krieg.Initialize()) return -1;

    krieg.Run();
    krieg.Shutdown();
    return 0;
}
