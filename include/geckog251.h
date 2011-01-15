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


// {{{ mainpage (documenation)
/**
 * @mainpage
 * @version 0.01
 * @author Jeremiah Mahler <jmmahler@gmail.com>
 * @date  Thu, 13 Jan 2011 23:09:28 -0800
 *
 * This library provides functions for driving Geckodrive G251 stepper
 * motor drivers through a parallel port.
 *
 * USAGE:
 *
 @verbatim
 Axis *axis_x;
 Axis *axis_y;
 int i;
 
 Port port("/dev/parport0");
 
 axis_x = port.openAxis(AXIS_X);
 axis_y = port.openAxis(AXIS_Y);
 
 axis_x->enable()
 axis_y->enable()
 
 // Rotate each axis 1 revolution
 // in opposite directions.
 
 axis_x->dir_cw();
 axis_y->dir_ccw();
 
 for (i = 0; i < 2000; i++) {
   axis_x->step();
   axis_y->step();
 }
 
 axis_x->disable()
 axis_y->disable()
 @endverbatim
 *
 *
 *
 * AXIS PIN ASSIGNMENTS:
 *
 * The terminal (on a G251) which corresponds to a pin
 * (on the parallel port) are hard coded in the library
 * as show in the table below.

<table>
<tr><th>pin</th><th>port</th><th>action</th><th>terminal</th><th>axis</th></tr>
  <tr><td>2</td><td>data0</td><td>direction</td><td>9</td><td>x</td></tr>
  <tr><td>3</td><td>data1</td><td>step<td></td>10</td><td></td></tr>
  <tr><td>4</td><td>data2</td><td>disable</td><td>11</td><td></td></tr>
  <tr><td>18-25</td><td>--</td><td>common</td><td>12</td><td></td></tr>
  <tr><td>&nbsp;</td><td></td><td></td><td></td><td></td></tr>
  <tr><td>5</td><td>data3</td><td>direction</td><td>9</td><td>y</td></tr>
  <tr><td>6</td><td>data4</td><td>step</td><td>10</td><td></td></tr>
  <tr><td>7</td><td>data5</td><td>disable</td><td>11</td><td></td></tr>
  <tr><td>18-25</td><td>--</td><td>common</td><td>12</td><td></td></tr>
</table>
For the common connection it is not necessary to connect all pins on
18-25 but at least one or two should be connected.

 * REFERENCES:
 *
 * Parallel Port<br>
 *    http://yyao.ca/projects/ParallelPortLinux/
 *
 * using PPDEV<br>
 *   http://as6edriver.sourceforge.net/Parallel-Port-Programming-HOWTO/accessing.html<br>
 *
 *   http://people.redhat.com/twaugh/parport/html/x916.html
 *
 */
// }}}

// {{{ ppError
/**
 *  The ppError object is used to throw and exception
 *  when an error involving the parallel port occurs.
 */
struct ppError {
    const char* msg;
    ppError(const char* _msg) { msg = _msg; };
};
// }}}

// {{{ class Port

class Axis;

/**
 * The "Port" object establishes the connection with the
 * parallel port. Then a specific axis can be opened (openAxis()).
 *
 */
class Port {

    int ppfd;  // file descriptor for the parallel port 

    Axis* axis_x;
    Axis* axis_y;
    //Axis* axis_z;

public:
    Port(const char* pathname);
    ~Port();

    /**
     * openAxis acts as a "Abstract Factory" building
     * specific Axis objects that satisfy the generic Axis interface.
     */
    Axis* openAxis(int axis);

    int getfd();
};
// }}}

// {{{ class Axis

enum { AXIS_X, AXIS_Y};
//enum { AXIS_X, AXIS_Y, AXIS_Z };

/**
 *
 * Axis is an abstract class that defines the member
 * functions that every specific axis must provide.
 *
 */
class Axis { // abstract class
public:
    /**
     * Enable the stepper motor.
     */
    virtual void enable() = 0;

    /**
     * Disable the stepper motor.
     * (step() commands will be ignored)
     */
    virtual void disable() = 0;

    /**
     *
     * Step the motor 1 position.
     *
     */
    virtual void step() = 0;

    /**
     * Set the motor rotation direction to clock wise.
     */
    virtual void dir_cw() = 0;

    /**
     * Set the motor rotation direction to counter clock wise.
     */
    virtual void dir_ccw() = 0;

    /**
     * Reverse the motor rotation direction.
     */
    virtual void dir_rev() = 0;
};
// }}}

// {{{ class AxisX

/**
 *
 * AxisX defines the first port.
 *
 * Refer to axis pin assigments for axis X.
 *
 */
class AxisX : public Axis {

    Port *port;
    int step_udelay;
    int chdir_udelay;

public:
    AxisX(Port *port);
    void enable();
    void disable();
    void step();
    void dir_cw();
    void dir_ccw();
    void dir_rev();
};
// }}}

// {{{ class AxisY
/**
 *
 * AxisY defines the first port.
 *
 * Refer to axis pin assigments for axis Y.
 *
 */
class AxisY : public Axis {

    Port *port;
    int step_udelay;
    int chdir_udelay;

public:
    AxisY(Port *port);
    void step();
    void enable();
    void disable();
    void dir_cw();
    void dir_ccw();
    void dir_rev();
};
// }}}

