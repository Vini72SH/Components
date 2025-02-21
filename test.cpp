#include "buffer.hpp"
#include <iostream>

using namespace std;

int main () {
    Buffer b;

    b.allocate(5, sizeof(int));

    int x;
    int *y;
    for (int i = 0; i < 5; ++i) {
        x = i + 10;
        b.enqueue(&x);
        cout << "Fim do Buffer: " << b.getEnd() << '\n';
    }

    for (int i = 0; i < 8; ++i) {
        y = (int *)b.dequeue();

        if (y) {
            cout << "Elemento: " << *y << '\n';
        }
        cout << "Começo do Buffer: " << b.getStart() << '\n';
    }

    return 0;
}