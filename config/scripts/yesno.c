/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT on ERC-32.
*/

/*
 * File:    $Source$
 * 
 * $Id: yesno.c 605 2006-03-21 15:27:22 +0000 (mar, 21 mar 2006) aitor $
 *  
 * $Revision: 605 $
 * $Date: 2006-03-21 15:27:22 +0000 (mar, 21 mar 2006) $
 * Last CVS commit by $Author: aitor $
 * 
 * Registro de cambios
 * 
 * $Log$
 * Revision 1.1  2005/09/19 11:38:04  aitor
 * Restoring the project.
 *
 * Revision 1.1  2005/06/21 10:28:15  aitor
 * Alta del mini sistema ERCOS-RT
 *
 * Revision 1.1  2005/03/04 12:35:27  aitor
 * Scripts de configuraci�n del n�cleo.
 *
 */

/*
 *  yesno.c -- implements the yes/no box
 *
 *  ORIGINAL AUTHOR: Savio Lam (lam836@cs.cuhk.hk)
 *  MODIFIED FOR LINUX KERNEL CONFIG BY: William Roadcap (roadcap@cfw.com)
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "dialog.h"

/*
 * Display termination buttons
 */
static void
print_buttons(WINDOW *dialog, int height, int width, int selected)
{
    int x = width / 2 - 10;
    int y = height - 2;

    print_button (dialog, " Yes ", y, x, selected == 0);
    print_button (dialog, "  No  ", y, x + 13, selected == 1);

    wmove(dialog, y, x+1 + 13*selected );
    wrefresh (dialog);
}

/*
 * Display a dialog box with two buttons - Yes and No
 */
int
dialog_yesno (const char *title, const char *prompt, int height, int width)
{
    int i, x, y, key = 0, button = 0;
    WINDOW *dialog;

    /* center dialog box on screen */
    x = (COLS - width) / 2;
    y = (LINES - height) / 2;

    draw_shadow (stdscr, y, x, height, width);

    dialog = newwin (height, width, y, x);
    keypad (dialog, TRUE);

    draw_box (dialog, 0, 0, height, width, dialog_attr, border_attr);
    wattrset (dialog, border_attr);
    mvwaddch (dialog, height-3, 0, ACS_LTEE);
    for (i = 0; i < width - 2; i++)
	waddch (dialog, ACS_HLINE);
    wattrset (dialog, dialog_attr);
    waddch (dialog, ACS_RTEE);

    if (title != NULL && strlen(title) >= width-2 ) {
	/* truncate long title -- mec */
	char * title2 = malloc(width-2+1);
	memcpy( title2, title, width-2 );
	title2[width-2] = '\0';
	title = title2;
    }

    if (title != NULL) {
	wattrset (dialog, title_attr);
	mvwaddch (dialog, 0, (width - strlen(title))/2 - 1, ' ');
	waddstr (dialog, (char *)title);
	waddch (dialog, ' ');
    }

    wattrset (dialog, dialog_attr);
    print_autowrap (dialog, prompt, width - 2, 1, 3);

    print_buttons(dialog, height, width, 0);

    while (key != ESC) {
	key = wgetch (dialog);
	switch (key) {
	case 'Y':
	case 'y':
	    delwin (dialog);
	    return 0;
	case 'N':
	case 'n':
	    delwin (dialog);
	    return 1;

	case TAB:
	case KEY_LEFT:
	case KEY_RIGHT:
	    button = ((key == KEY_LEFT ? --button : ++button) < 0)
			? 1 : (button > 1 ? 0 : button);

	    print_buttons(dialog, height, width, button);
	    wrefresh (dialog);
	    break;
	case ' ':
	case '\n':
	    delwin (dialog);
	    return button;
	case ESC:
	    break;
	}
    }

    delwin (dialog);
    return -1;			/* ESC pressed */
}
