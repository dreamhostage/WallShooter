#include "Framework.h"


int main(int argc, char* argv[])
{
    std::unique_ptr<FrameworkClass> Framework = std::make_unique<FrameworkClass>();

    Framework->Run();

    return 0;
}
