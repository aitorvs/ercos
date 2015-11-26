/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez, Pablo Parra Espada
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/


#ifndef _UTILS_H_
#define _UTILS_H_

#include "edroombp.h"
#include <stdio.h> // quitar este include en la version final.

#define OVERHEADAVG_LOOPS 20
#define OVERHEADAVG_TAILS 0.2

/*
 * Function bubble_sort()
 * It sorts an array of time_values in increasing order using
 * bubble sort algorithm
 */

void bubble_sort(Pr_Time _value_list[], int _length);

void centeravg_reset(int _numpoints, float _tailpct);

void centeravg_add(Pr_Time _dat);

void centeravg_done(Pr_Time *_time_measure);

#endif



