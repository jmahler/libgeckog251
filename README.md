
NAME
----

libgeckog251 - C++ library for driving the Geckodrive G251 stepper motor driver using a parallel port.

INTRODUCTION
------------

This library can be used to drive a [Geckodrive G251][g251] stepper motor
driver by using the parallel port.

  [g251]: http://www.geckodrive.com/product.aspx?c=3&i=14471

SYNOPSIS
--------

    #include "geckog251.h"

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


AUTHOR
------

Jeremiah Mahler <jmmahler@gmail.com>  
<http://www.google.com/profiles/jmmahler#about>

COPYRIGHT
---------

Copyright &copy; 2011, Jeremiah Mahler.  All Rights Reserved.<br>
This project is free software and released under
the [GNU General Public License][gpl].

 [gpl]: http://www.gnu.org/licenses/gpl.html

