#include <locale.h>
#include <glib-unix.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include "config.h"
#include "smtk-app.h"

// Quit through the normal path on SIGTERM/SIGINT, so the keys window can tell
// the privileged cli to stop. The default action would kill only this process
// and leave the cli, which we cannot kill, reading input devices.
static int on_quit_signal(void *data)
{
	smtk_app_quit(SMTK_APP(data));
	return G_SOURCE_CONTINUE;
}

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "");
	// Typically "/usr" "/" "share/locale".
	bindtextdomain(GETTEXT_PACKAGE, PACKAGE_LOCALEDIR);
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	textdomain(GETTEXT_PACKAGE);

	g_autoptr(SmtkApp) app = smtk_app_new();

	unsigned int sigterm_id = g_unix_signal_add(SIGTERM, on_quit_signal, app);
	unsigned int sigint_id = g_unix_signal_add(SIGINT, on_quit_signal, app);
	int status = g_application_run(G_APPLICATION(app), argc, argv);
	g_source_remove(sigterm_id);
	g_source_remove(sigint_id);

	return status;
}
