//
// Created by josh on 2/16/19.
//

#include <stdio.h>
#include <time.h>

#include "serial.c"
#include "screen.c"

int main() {
    Display *d = XOpenDisplay((char *) NULL);

    time_t start = clock();

    unsigned char * values = im(d);

    time_t end = clock();

    printf("%f\n", ((double) end - start) / CLOCKS_PER_SEC);

    printf("LEFT: ");
    for (int i = 0; i < 15 * 3; i ++) {
        printf("%d,", values[i]);
        if (i % 3 == 2) printf("|");
    }

    printf("\nTOP: ");
    for (int i = 15 * 3; i < 45* 3; i ++) {
        printf("%d,", values[i]);
        if (i % 3 == 2) printf("|");
    }

    printf("\nRIGHT: ");
    for (int i = 45 * 3; i < 60 * 3; i ++) {
        printf("%d,", values[i]);
        if (i % 3 == 2) printf("|");
    }
    printf("\n");
}