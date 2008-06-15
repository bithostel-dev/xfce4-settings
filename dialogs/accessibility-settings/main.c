/*
 *  Copyright (c) 2008 Stephan Arts <stephan@xfce.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <config.h>
#include <string.h>

#include <glib.h>

#if defined(GETTEXT_PACKAGE)
#include <glib/gi18n-lib.h>
#else
#include <glib/gi18n.h>
#endif

#include <gtk/gtk.h>
#include <glade/glade.h>

#include <libxfcegui4/libxfcegui4.h>
#include <xfconf/xfconf.h>
#include "accessibility-dialog.glade.h"

typedef struct {
    GtkWidget *slave;
    XfconfChannel *channel;
} PropertyPair;

gboolean version = FALSE;

static GOptionEntry entries[] =
{
    {    "version", 'v', G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_NONE, &version,
        N_("Version information"),
        NULL
    },
    { NULL }
};

GtkWidget *
accessibility_settings_dialog_new_from_xml (GladeXML *gxml)
{
    XfconfChannel *xsettings_channel = xfconf_channel_new("xsettings");
    XfconfChannel *accessx_channel = xfconf_channel_new("accessx");

    GtkWidget *xkb_accessx_sticky_check = glade_xml_get_widget (gxml, "xkb_accessx_sticky_check");
    GtkWidget *xkb_accessx_sticky_latch_mode = glade_xml_get_widget (gxml, "xkb_accessx_sticky_latch_mode");
    GtkWidget *xkb_accessx_sticky_two_keys_disable_check = glade_xml_get_widget (gxml, "xkb_accessx_sticky_two_keys_disable_check");
    GtkWidget *xkb_accessx_slow_check = glade_xml_get_widget (gxml, "xkb_accessx_slow_check");
    GtkWidget *xkb_accessx_slow_delay_scale = glade_xml_get_widget (gxml, "xkb_accessx_slow_delay_scale");
    GtkWidget *xkb_accessx_bounce_check = glade_xml_get_widget (gxml, "xkb_accessx_bounce_check");
    GtkWidget *xkb_accessx_debounce_delay_scale = glade_xml_get_widget (gxml, "xkb_accessx_debounce_delay_scale");


    /* Bind easy properties */
    xfconf_g_property_bind (accessx_channel, 
                            "/AccessX/StickyKeys",
                            G_TYPE_BOOLEAN,
                            (GObject *)xkb_accessx_sticky_check, "active");
    xfconf_g_property_bind (accessx_channel, 
                            "/AccessX/StickyKeys/LatchToLock",
                            G_TYPE_BOOLEAN,
                            (GObject *)xkb_accessx_sticky_latch_mode, "active");
    xfconf_g_property_bind (accessx_channel, 
                            "/AccessX/StickyKeys/TwoKeysDisable",
                            G_TYPE_BOOLEAN,
                            (GObject *)xkb_accessx_sticky_two_keys_disable_check, "active");



    GtkWidget *dialog = glade_xml_get_widget (gxml, "accessibility-settings-dialog");


    gtk_widget_show_all(dialog);
    gtk_widget_hide(dialog);
    return dialog;
}

int
main(int argc, char **argv)
{
    GladeXML *gxml;
    XfconfChannel *channel;
    GError *cli_error = NULL;
    gchar *data_dirs = NULL;

    #ifdef ENABLE_NLS
    bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain (GETTEXT_PACKAGE);
    #endif

    if(!gtk_init_with_args(&argc, &argv, _("."), entries, PACKAGE, &cli_error))
    {
        if (cli_error != NULL)
        {
            g_print (_("%s: %s\nTry %s --help to see a full list of available command line options.\n"), PACKAGE, cli_error->message, PACKAGE_NAME);
            g_error_free (cli_error);
            return 1;
        }
    }

    if(version)
    {
        g_print("%s\n", PACKAGE_STRING);
        return 0;
    }

    xfconf_init(NULL);

    gxml = glade_xml_new_from_buffer (accessibility_dialog_glade,
                                      accessibility_dialog_glade_length,
                                      NULL, NULL);

    GtkWidget *dialog = accessibility_settings_dialog_new_from_xml (gxml);

    gtk_dialog_run(GTK_DIALOG(dialog));

    xfconf_shutdown();

    return 0;
}
