#include "Bubblesort.h"

void Bubblesort(int* a, int n, comparer& compare, swaper& swap) {
    for (int i = n - 1; i >= 0; i--) {
        for (int k = 0; k < i; k++) {
            if (compare(a[k], a[k + 1]) > 0) {
                swap(a[k], a[k + 1]);
            }
        }
    }
}
