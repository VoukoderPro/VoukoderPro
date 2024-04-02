#include "Test.h"

#include <iostream>

int main()
{
    boost::function<pluginapi_create_t> factory;
    std::shared_ptr<VoukoderPro::IClient> vkdrpro;

    try
    {
        factory = VOUKODERPRO_CREATE_INSTANCE;
        vkdrpro = factory();

        if (!vkdrpro)
        {
            return -1;
        }
    }
    catch (boost::system::system_error e)
    {
        return -1;
    }

    vkdrpro->init();
    vkdrpro->close();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
