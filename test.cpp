#include "component.hpp"

using namespace std;

int main () {
    sinuca::EngineDebugComponent* debug = new sinuca::EngineDebugComponent();
    sinuca::EngineDebugComponent* otherComponent = new sinuca::EngineDebugComponent();

    debug->otherComponent = otherComponent;
    debug->connectionID = otherComponent->ConnectToComponent(5);

    for (int i = 0; i < 5; ++i) {
        debug->Clock();
        otherComponent->Clock();
    }

    delete debug;
    delete otherComponent;

    return 0;
}