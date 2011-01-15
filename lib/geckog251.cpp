
/*
 * Copyright (C) 2011 Jeremiah Mahler <jmmahler@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/ppdev.h>
#include <linux/parport.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "geckog251.h"

// {{{ Port::Port
Port::Port(const char* pathname)
{
    unsigned char byte;

    if ((ppfd = open(pathname, O_RDWR)) < 0) {
        throw ppError("open error");
    }

    if (ioctl(ppfd, PPCLAIM)) {
        close(ppfd);
        throw ppError("PPCLAIM error");
    	//perror("PPCLAIM error");
    }

    // store axis objects
    axis_x = new AxisX(this);
    axis_y = new AxisY(this);


    // initialize CONTROL and DATA ports

    // CONTROL
    //byte = 0x0;
    //if (ioctl(ppfd, PPWCONTROL, &byte))
    //    throw ppError("PPWCONTROL error");

    // DATA
    byte = 0x0;
    if (ioctl(ppfd, PPWDATA, &byte))
        throw ppError("PPWDATA error");
}
// }}}

// {{{ Port::openAxis()

Axis *Port::openAxis(int axis) {

    if (axis == AXIS_X) {
        return axis_x;
    } else if (axis == AXIS_Y) {
        return axis_y;
    }
    // else
    return axis_x;
    //return axis_z;
}

// }}}

// {{{ Port::getfd()

int Port::getfd() {

    return ppfd;
}
// }}}

// {{{ Port::~Port()

Port::~Port() {
    delete axis_x;
    delete axis_y;
    //delete axis_z;
}
// }}}

// {{{ set_udelay  (local function)
namespace {  /* local functions */

    void set_udelay(int* step_udelay, int* chdir_udelay) {
        //*step_udelay = 75;  // OK
        //*chdir_udelay = 20;
        *step_udelay = 83;  // OK, 10% greater than minimum
        *chdir_udelay = 0 ;
        //*step_udelay = 75;  // OK
        //*chdir_udelay = 0;
        //*step_udelay = 60; // XXX
        //*chdir_udelay = 50;
     }
}
// }}}

// {{{ AxisX

#define AXIS_X_DIR (1 << 0)
#define AXIS_X_STEP (1 << 1)
#define AXIS_X_DISABLE (1 << 2)

// {{{ AxisX::AxisX

AxisX::AxisX(Port *_port)
{
    port = _port;

    set_udelay(&step_udelay, &chdir_udelay);
}
// }}}

// {{{ AxisX::enable

void AxisX::enable()
{
    unsigned char byte;
    int ppfd = port->getfd();

    if (ioctl(ppfd, PPRDATA, &byte))
        throw ppError("PPRDATA error");

    byte |= AXIS_X_DISABLE; // set bit 
    if (ioctl(ppfd, PPWDATA, &byte))
        throw ppError("PPWDATA error");

}
// }}}

// {{{ AxisX::disable

void AxisX::disable()
{
    unsigned char byte;
    int ppfd = port->getfd();

    if (ioctl(ppfd, PPRDATA, &byte))
        throw ppError("PPRDATA error");

    byte &= ~(AXIS_X_DISABLE); // clear bit at offset 2
    if (ioctl(ppfd, PPWDATA, &byte))
        throw ppError("PPWDATA error");

}
// }}}

// {{{ AxisX::step
/*
 * Steps on transition from clear to set.
 *
 */
void AxisX::step()
{
    unsigned char byte;
    int ppfd = port->getfd();

    if (ioctl(ppfd, PPRDATA, &byte))
        throw ppError("PPRDATA error");

    // clear it if it is not already
    if (byte & AXIS_X_STEP) {
        byte &= ~AXIS_X_STEP;  // clear
        if (ioctl(ppfd, PPWDATA, &byte))
            throw ppError("PPWDATA error");

        usleep(step_udelay);
    }

    byte |= AXIS_X_STEP; // set
    if (ioctl(ppfd, PPWDATA, &byte))
        throw ppError("PPWDATA error");

    usleep(step_udelay);
}
// }}}

// {{{ AxisX::dir_cw()
void AxisX::dir_cw() {
    unsigned char byte;
    int ppfd = port->getfd();

    if (ioctl(ppfd, PPRDATA, &byte))
        throw ppError("PPRDATA error");

    // if it is already clear, leave it
    if ( !(byte & AXIS_X_DIR))
        return;

    byte &= ~AXIS_X_DIR; // clear
    if (ioctl(ppfd, PPWDATA, &byte))
        throw ppError("PPWDATA error");

    usleep(chdir_udelay);
}
// }}}

// {{{ AxisX::dir_ccw()
void AxisX::dir_ccw() {
    unsigned char byte;
    int ppfd = port->getfd();

    if (ioctl(ppfd, PPRDATA, &byte))
        throw ppError("PPRDATA error");

    // if it is already set, leave it
    if (byte & AXIS_X_DIR)
        return;

    byte |= AXIS_X_DIR; // set
    if (ioctl(ppfd, PPWDATA, &byte))
        throw ppError("PPWDATA error");

    usleep(chdir_udelay);
}
// }}}

// {{{ AxisX::dir_rev()
void AxisX::dir_rev() {
    unsigned char byte;
    int ppfd = port->getfd();

    if (ioctl(ppfd, PPRDATA, &byte))
        throw ppError("PPRDATA error");

    // reverse direction
    byte ^= AXIS_X_DIR;
    if (ioctl(ppfd, PPWDATA, &byte))
        throw ppError("PPWDATA error");

    usleep(chdir_udelay);
}
// }}}

// }}}

// {{{ AxisY

/*
 * AxisY is identical to AxisX except the bits are different.
 *
 */

#define AXIS_Y_DIR (1 << 3)
#define AXIS_Y_STEP (1 << 4)
#define AXIS_Y_DISABLE (1 << 5)

// {{{ AxisY::AxisY

AxisY::AxisY(Port *_port)
{
    port = _port;

    set_udelay(&step_udelay, &chdir_udelay);
}
// }}}

// {{{ AxisY::enable

void AxisY::enable()
{
    unsigned char byte;
    int ppfd = port->getfd();

    if (ioctl(ppfd, PPRDATA, &byte))
        throw ppError("PPRDATA error");

    byte |= AXIS_X_DISABLE; // set bit 
    if (ioctl(ppfd, PPWDATA, &byte))
        throw ppError("PPWDATA error");

}
// }}}

// {{{ AxisY::disable

void AxisY::disable()
{
    unsigned char byte;
    int ppfd = port->getfd();

    if (ioctl(ppfd, PPRDATA, &byte))
        throw ppError("PPRDATA error");

    byte &= ~(AXIS_X_DISABLE); // clear bit at offset 2
    if (ioctl(ppfd, PPWDATA, &byte))
        throw ppError("PPWDATA error");

}
// }}}

// {{{ AxisY::step
/*
 * Steps on transition from clear to set.
 *
 */
void AxisY::step()
{
    unsigned char byte;
    int ppfd = port->getfd();

    if (ioctl(ppfd, PPRDATA, &byte))
        throw ppError("PPRDATA error");

    // clear it if it is not already
    if (byte & AXIS_X_STEP) {
        byte &= ~AXIS_X_STEP;  // clear
        if (ioctl(ppfd, PPWDATA, &byte))
            throw ppError("PPWDATA error");

        usleep(step_udelay);
    }

    byte |= AXIS_X_STEP; // set
    if (ioctl(ppfd, PPWDATA, &byte))
        throw ppError("PPWDATA error");

    usleep(step_udelay);
}
// }}}

// {{{ AxisY::dir_cw()
void AxisY::dir_cw() {
    unsigned char byte;
    int ppfd = port->getfd();

    if (ioctl(ppfd, PPRDATA, &byte))
        throw ppError("PPRDATA error");

    // if it is already clear, leave it
    if ( !(byte & AXIS_X_DIR))
        return;

    byte &= ~AXIS_X_DIR; // clear
    if (ioctl(ppfd, PPWDATA, &byte))
        throw ppError("PPWDATA error");

    usleep(chdir_udelay);
}
// }}}

// {{{ AxisY::dir_ccw()
void AxisY::dir_ccw() {
    unsigned char byte;
    int ppfd = port->getfd();

    if (ioctl(ppfd, PPRDATA, &byte))
        throw ppError("PPRDATA error");

    // if it is already set, leave it
    if (byte & AXIS_X_DIR)
        return;

    byte |= AXIS_X_DIR; // set
    if (ioctl(ppfd, PPWDATA, &byte))
        throw ppError("PPWDATA error");

    usleep(chdir_udelay);
}
// }}}

// {{{ AxisY::dir_rev()
void AxisY::dir_rev() {
    unsigned char byte;
    int ppfd = port->getfd();

    if (ioctl(ppfd, PPRDATA, &byte))
        throw ppError("PPRDATA error");

    // reverse direction
    byte ^= AXIS_X_DIR;
    if (ioctl(ppfd, PPWDATA, &byte))
        throw ppError("PPWDATA error");

    usleep(chdir_udelay);
}
// }}}

// }}}
