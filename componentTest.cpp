#include <cstdio>
#include <cstdlib>
#include "component.hpp"

using namespace std;

int main () {
    Component<int>* comp = new Component<int>;

    comp->enqueue(5);
    comp->enqueue(10);
    comp->enqueue(15);
    comp->enqueue(20);
    comp->enqueue(25);

    comp->print();

    comp->flushQueue();

    delete comp;

    return 0;
}