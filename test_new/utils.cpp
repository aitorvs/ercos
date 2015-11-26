/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez, Pablo Parra Espada
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

#include "utils.h"

/*
 * Functions to do center-weighted averaging
 */
 
int 	centeravg_max;

Pr_Time	centeravg_array[OVERHEADAVG_LOOPS];

int		centeravg_cur;

float	centeravg_tailpct;

void centeravg_reset(int _numpoints, float _tailpct)
{
	centeravg_max = _numpoints;
	centeravg_cur = 0;
	centeravg_tailpct = _tailpct;
}

void centeravg_add(Pr_Time _dat)
{
	centeravg_array[centeravg_cur++] = _dat;
}

void centeravg_done(Pr_Time *_time_measure)
{
	int i, tailsize;
	Pr_Time tmp;

	/* Handle case where fewer datapoints are entered */
	centeravg_max = centeravg_cur;
	
	/* Sort the data points */
	bubble_sort(centeravg_array, centeravg_max);
	
	/* Now average the middle (1-centeravg_tailpct)*100 percent */
	tailsize = (int)(((float)centeravg_max)*centeravg_tailpct);

	tmp = 0.0;

	for (i = tailsize; i < centeravg_max - tailsize; i++)
		tmp += centeravg_array[i];
	tmp = tmp.Secns() / (float)(centeravg_max - (2 * tailsize));

	centeravg_max = centeravg_cur = 0;
	*_time_measure = tmp;
}

void bubble_sort(Pr_Time _value_list[], int _length)
{
	int i, j, finished;
	Pr_Time temp;

	for(i = _length - 1; i > 0; i--)
	{
		finished = 1;
    	for(j = 0; j < i; j++)
    	{
      		if(_value_list[j] > _value_list[j+1])
      		{
        		temp = _value_list[j];
        		_value_list[j] = _value_list[j+1];
        		_value_list[j+1] = temp;
        		finished = 0;
      		}
    	}
		if(finished) break;
  	}
}
