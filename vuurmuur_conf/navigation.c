/***************************************************************************
 *   Copyright (C) 2003-2019 by Victor Julien                              *
 *   victor@vuurmuur.org                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "main.h"

/*  nav_field_comment

    Needs the field label to be "comment".

    This is a special function because it needs to handle a edit field

    F5,F6,F10,F12 or ESC make the function return -1, all others return 0.
*/
int nav_field_comment(FORM *form, int key)
{
    switch (key) {
        case 9:
            return (0);
        case KEY_F(5):
        case KEY_F(6):
        case KEY_F(10):
        case KEY_F(12):
        case 27: // esc:
            form_driver_wrap(form,
                    REQ_NEXT_FIELD); // this is to make sure the field is saved
            return (0);
        case 32: // space
            form_driver_wrap(form, key);
            break;
        case 10: // enter
            // go line-by-line trough the field, when reaching the end, go to
            // next field.
            if (form_driver_wrap(form, REQ_NEXT_LINE) < 0) {
                form_driver_wrap(form, REQ_NEXT_FIELD);
            }
            form_driver_wrap(form, REQ_BEG_LINE);
            break;
        case KEY_DOWN:
            // move down in the field, until we reach the end,
            // them move to the next field
            if (form_driver_wrap(form, REQ_DOWN_CHAR) < 0) {
                form_driver_wrap(form, REQ_NEXT_FIELD);
                form_driver_wrap(form, REQ_BEG_LINE);
            }
            break;
        case KEY_UP:
            // move up in the field, until we reach the end,
            // them move to the previous field
            if (form_driver_wrap(form, REQ_UP_CHAR) < 0) {
                form_driver_wrap(form, REQ_PREV_FIELD);
                form_driver_wrap(form, REQ_BEG_LINE);
            }
            break;
        case KEY_RIGHT:
            if (form_driver_wrap(form, REQ_RIGHT_CHAR) < 0) {
                if (form_driver_wrap(form, REQ_DOWN_CHAR) < 0)
                    form_driver_wrap(form, REQ_BEG_FIELD);
                else
                    form_driver_wrap(form, REQ_BEG_LINE);
            }
            break;
        case KEY_LEFT:
            if (form_driver_wrap(form, REQ_LEFT_CHAR) < 0) {
                if (form_driver_wrap(form, REQ_UP_CHAR) < 0)
                    form_driver_wrap(form, REQ_END_FIELD);
                else
                    form_driver_wrap(form, REQ_END_LINE);
            }
            break;
        case 127: // backspace
        case KEY_BACKSPACE:
            form_driver_wrap(form, REQ_PREV_CHAR);
            form_driver_wrap(form, REQ_DEL_CHAR);
            break;
        case KEY_DC: // delete
            form_driver_wrap(form, REQ_DEL_CHAR);
            break;
        case KEY_HOME: // doesn't seem to work in my kde (3.1.2) setup
            form_driver_wrap(form, REQ_BEG_LINE);
            break;
        case KEY_END:
            form_driver_wrap(form, REQ_END_LINE);
            break;
        default:
            // If this is a normal character, it gets printed
            form_driver_wrap(form, key);
            break;
    }

    return (1);
}

int nav_field_simpletext(FORM *form, int key)
{
    vrmr_debug(LOW, "key %d", key);
    switch (key) {
        case 9: /* tab */
            return (0);
        case KEY_F(5):  /* f5  */
        case KEY_F(6):  /* f6  */
        case KEY_F(10): /* f10 */
        case KEY_F(12): /* f12 for help */
        case 27:        /* esc */
            form_driver_wrap(form,
                    REQ_NEXT_FIELD); // this is to make sure the field is saved
            form_driver_wrap(
                    form, REQ_PREV_FIELD); /* But we don't want to move down */
            return (0);

        case 32: // space
            form_driver_wrap(form, key);
            break;
        case KEY_DOWN:
        case 10: // enter
        {
            form_driver_wrap(form, REQ_NEXT_FIELD);
            form_driver_wrap(form, REQ_BEG_LINE);
            break;
        }
        case KEY_UP:
            form_driver_wrap(form, REQ_PREV_FIELD);
            form_driver_wrap(form, REQ_BEG_LINE);
            break;
        case KEY_RIGHT:
            if (form_driver_wrap(form, REQ_RIGHT_CHAR) < 0) {
                form_driver_wrap(form, REQ_SCR_FCHAR);
            }

            break;

        case KEY_LEFT:
            if (form_driver_wrap(form, REQ_LEFT_CHAR) < 0) {
                form_driver_wrap(form, REQ_SCR_BCHAR);
            }
            break;

        case 127: // backspace
        case KEY_BACKSPACE:
            form_driver_wrap(form, REQ_PREV_CHAR);
            form_driver_wrap(form, REQ_DEL_CHAR);
            break;
        case KEY_DC:
            form_driver_wrap(form, REQ_DEL_CHAR);
            break;
        case KEY_HOME: // doesn't seem to work in my kde (3.1.2) setup
            form_driver_wrap(form, REQ_BEG_LINE);
            break;
        case KEY_END:
            form_driver_wrap(form, REQ_END_LINE);
            break;
        default:
            // If this is a normal character, it gets printed
            form_driver_wrap(form, key);
            break;
    }

    return (1);
}

int nav_field_yesno(FORM *form, int key)
{
    switch (key) {
        case 32: // space
        {
            FIELD *cur = current_field(form);

            if (strncasecmp(field_buffer(cur, 0), STR_YES, StrLen(STR_YES)) ==
                    0) {
                set_field_buffer_wrap(cur, 0, STR_NO);
            } else {
                set_field_buffer_wrap(cur, 0, STR_YES);
            }
            break;
        }
        case 'y': {
            FIELD *cur = current_field(form);

            if (strncasecmp(field_buffer(cur, 0), STR_NO, StrLen(STR_NO)) ==
                    0) {
                set_field_buffer_wrap(cur, 0, STR_YES);
            }
            break;
        }
        case 'n': {
            FIELD *cur = current_field(form);

            if (strncasecmp(field_buffer(cur, 0), STR_YES, StrLen(STR_YES)) ==
                    0) {
                set_field_buffer_wrap(cur, 0, STR_NO);
            }
            break;
        }
        default:
            return (0);
    }
    return (1);
}

int nav_field_toggleX(FORM *form, int key)
{
    vrmr_fatal_if_null(form);
    FIELD *cur = current_field(form);
    vrmr_fatal_if_null(cur);

    switch (key) {
        case 32: // space
        {
            if (strncasecmp(field_buffer(cur, 0), "X", 1) == 0) {
                set_field_buffer_wrap(cur, 0, " ");
            } else {
                set_field_buffer_wrap(cur, 0, "X");
            }
            break;
        }
        case 'y': {
            if (strncasecmp(field_buffer(cur, 0), " ", 1) == 0) {
                set_field_buffer_wrap(cur, 0, "X");
            }
            break;
        }
        case 'n': {
            if (strncasecmp(field_buffer(cur, 0), "X", 1) == 0) {
                set_field_buffer_wrap(cur, 0, " ");
            }
            break;
        }
        default:
            return (0);
    }
    return (1);
}

int validate_commentfield(char *fieldbuffer, regex_t *reg_ex ATTR_UNUSED)
{
    vrmr_fatal_if_null(fieldbuffer);

    /* run the regex */
    /*
        if(regexec(reg_ex, fieldbuffer, 0, NULL, 0) != 0)
        {
            vrmr_error(-1, "Error", "comment line contains illegal
       characters."); return(-1);
        }
    */

    for (size_t i = 0; i < StrMemLen(fieldbuffer); i++) {
        if (fieldbuffer[i] == '"') {
            vrmr_error(-1, VR_ERR,
                    gettext("the double quote sign \" is not allowed in the "
                            "commentfield."));
            return (-1);
        } else if (fieldbuffer[i] == '%') {
            vrmr_error(-1, VR_ERR,
                    gettext("the percent sign is not allowed in the "
                            "commentfield."));
            return (-1);
        }
    }

    return (0);
}
