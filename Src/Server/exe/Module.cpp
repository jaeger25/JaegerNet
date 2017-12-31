#include "Module.h"
#include "Session.h"

using namespace JaegerNet;
using namespace std;

int main(int /*argc*/, char** /*argv*/)
{
    SessionDetails details{ 31337 };
    Session session(details);

    session.Server().Run(false);

    return 0;
}
