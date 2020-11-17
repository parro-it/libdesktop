#include "dsktest.h"
#include "libdesktop.h"

#include <gtk/gtk.h>

UIHandle dsk_new_container_platform_ui_control() {
	return  gtk_fixed_new();
} 
