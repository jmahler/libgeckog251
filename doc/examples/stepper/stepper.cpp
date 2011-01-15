
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "geckog251.h"

/**
 *
 * The purpose of this program is to provide a simple way
 * to test a stepper motor.
 * It allows varying of the number of steps and direction of rotation.
 *
 * 
 * 
 * USAGE:
 *
 * Rotate 200 steps clock wise then 400 steps counter clock wise
 * and then repeat this 6 times.
 *
 *   user$ ./simple /dev/parport0 200 400 6
 *
 */

int
main(int argc, char* argv[])
{
    char *path;
    char *axis;
    int nsteps;
    int nstepsb;
    int repeat;
    Axis *axis_i;
//    Axis *axis_x;
//    Axis *axis_y;
//    Axis *axis_z;

    int r, j;

    if (argc == 6) {
	    path = argv[1];
	    axis = argv[2];
        nsteps = atoi(argv[3]);
        nstepsb = atoi(argv[4]);
        repeat = atoi(argv[5]);
    } else {
	    printf("usage: a.out </dev/parport0> <axis> <nsteps> <nsteps back> <repeat>\n");
	    return 1;
    }

    Port port(path);

    if (strcmp(axis, "x") == 0) {
        axis_i = port.openAxis(AXIS_X);
    } else if (strcmp(axis, "y") == 0) {
        axis_i = port.openAxis(AXIS_Y);
//    } else if (strcmp(axis, "z") == 0) {
//        axis_i = port.openAxis(AXIS_Z);
    } else {
        printf("unknown axis");
        return 1;
    }

    //axis_x = port.openAxis(AXIS_X);
    //axis_y = port.openAxis(AXIS_Y);
    //axis_z = port.openAxis(AXIS_Z);

    /*
    for (j = 0; j < nsteps; j++) {
        printf(".");
        axis_x->enable();
        axis_y->enable();
        axis_z->enable();

        axis_x->disable();
        axis_y->disable();
        axis_z->disable();
    }
    */
//    printf("\n");


    printf("steps = %d, %d\n", nsteps, nstepsb);

    //axis_i->enable();
    for (r = 0; r < repeat; r++) {

        if (nsteps) {
            axis_i->enable();
            printf("clock-wise\n");
            axis_i->dir_cw();

            for (j = 0; j < nsteps; j++) {
                axis_i->step();
            }
            axis_i->disable();
        }

        if (nstepsb) {
            axis_i->enable();
            printf("counter-clock-wise\n");
            axis_i->dir_ccw();

            for (j = 0; j < nstepsb; j++) {
                axis_i->step();
            }
            axis_i->disable();
        }
    }
    //axis_i->disable();


    return 0;
}

