#include "component.hpp"
#include <cstdio>
#include <cstdlib>

using namespace std;

int main () {
    sinuca::DebugComponent* debugComponent = new sinuca::DebugComponent();
    sinuca::DebugComponent* otherComponent = new sinuca::DebugComponent();

    debugComponent->otherComponent = otherComponent;
    debugComponent->id = otherComponent->Connect(10);

    for(int i = 0; i < 10; ++i) {
        debugComponent->Clock();
        otherComponent->Clock();
    }

    delete otherComponent;                                              
    delete debugComponent;          

    return 0;
}