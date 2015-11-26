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
 * $Id: tkparse.h 605 2006-03-21 15:27:22 +0000 (mar, 21 mar 2006) aitor $
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
 * Scripts de configuración del núcleo.
 *
 */

/*
 * tkparse.h
 */

/*
 * Token types (mostly statement types).
 */

enum e_token
{
    token_UNKNOWN,
    token_bool, 
    token_choice_header,
    token_choice_item,
    token_comment, 
    token_define_bool,
    token_define_hex,
    token_define_int,
    token_define_string,
    token_define_tristate,
    token_dep_bool,
    token_dep_mbool,
    token_dep_tristate,
    token_else, 
    token_endmenu,
    token_fi, 
    token_hex,
    token_if, 
    token_int,
    token_mainmenu_name, 
    token_mainmenu_option, 
    token_source,
    token_string,
    token_then,
    token_tristate, 
    token_unset,
};

/*
 * Operator types for conditionals.
 */

enum operator
{
    op_eq,
    op_neq,
    op_and,
    op_and1,
    op_or,
    op_bang,
    op_lparen,
    op_rparen,
    op_constant,
    op_variable,
    op_true,
    op_false,
    op_nuked
};

/*
 * Conditions come in linked lists.
 * Some operators take strings:
 *
 *   op_constant   "foo"
 *   op_variable   "$ARCH", "$CONFIG_PMAC", "$CONFIG_EXPERIMENTAL"
 *
 * Most "$..." constructs refer to a variable which is defined somewhere
 * in the script.  Note that it is legal to test variables which are never
 * defined, such as variables that are meaningful only on other architectures.
 */

struct condition
{
    struct condition *	next;
    enum operator	op;
    const char *	str;		/* op_constant */
    int			nameindex;	/* op_variable */
};

/*
 * Dependency list for dep_bool, dep_mbool, dep_tristate
 */

struct dependency
{
    char *		name;
    struct dependency *	next;
};

/*
 * A statement from a config.in file
 */

struct kconfig
{
    struct kconfig *	next;
    enum e_token	token;
    int			nameindex;
    char *		label;
    char *		value;
    struct condition *	cond;
    struct dependency *	depend;		/* token_dep_tristate */
    struct kconfig *	cfg_parent;	/* token_choice_item */

    /* used only in tkgen.c */
    int			menu_number;
    int			menu_line;
    struct kconfig *	menu_next;
};

struct variable
{
    char *	name;
    char	defined;
    char	global_written;
};

extern struct variable vartable[];
extern int max_varnum;

/*
 * Prototypes
 */

extern void fix_conditionals ( struct kconfig * scfg );		/* tkcond.c */
extern void dump_tk_script   ( struct kconfig * scfg );		/* tkgen.c  */
extern int get_varnum        ( char * name );			/* tkparse.c */
