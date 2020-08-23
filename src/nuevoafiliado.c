#include <gtk/gtk.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"

    GtkWidget *wNuevoAfiliado;
    GtkEntry *txtNombre;
    GtkEntry *txtPerfil;
    GtkEntry *txtDni;

void NuevoAfiliado ()
{
     GtkBuilder *builder; 
    
    builder = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "glade/nuevoafiliado2.glade", NULL);
    gtk_builder_connect_signals (builder, NULL);
    wNuevoAfiliado = GTK_WIDGET (gtk_builder_get_object (builder, "w_nuevoafiliado"));
    txtPerfil = GTK_ENTRY (gtk_builder_get_object (builder, "txt_perfil"));
    txtNombre = GTK_ENTRY (gtk_builder_get_object (builder, "txt_nombre"));
    txtDni = GTK_ENTRY (gtk_builder_get_object (builder, "txt_dni"));
    
    g_object_unref (builder);
    gtk_widget_show (wNuevoAfiliado);
}

void on_btn_cancelar_clicked ()
{
    gtk_widget_hide (wNuevoAfiliado);
}

void on_cmb_imgperfil_file_set ()
{
     
}

void on_btn_guardar_clicked ()
{
     FILE *pOrigen;
     gchar *opcion;
     gchar *archPerfiles="";

     archPerfiles = obtenerOpcion("dirConfiguracion");
     printf("%s\n",archPerfiles);
      
    if ( access( archPerfiles, F_OK ) != -1 )
    {
        //abro archivo para append datos
        pOrigen = fopen (archPerfiles, "a");

         if ( pOrigen == NULL ) 
         {
             printf ("ERROR ARCHIVO INEXISTENTE");
         }
         else
         {
             opcion = g_strconcat ("perfil ", gtk_entry_get_text (txtPerfil), "\n");
             g_fprintf (pOrigen, opcion);
             opcion = g_strconcat ("nombre ", gtk_entry_get_text (txtNombre), "\n");
             g_fprintf (pOrigen, opcion);
             opcion = g_strconcat("dni ", gtk_entry_get_text (txtDni), "\n");
             g_fprintf (pOrigen, opcion);
         }
    }
    else
    {
        pOrigen = fopen (archPerfiles, "w");
        if ( pOrigen == NULL )
        {
             printf ("ERROR ARCHIVO INEXISTENTE");
        }
         else
         {
             opcion = g_strconcat("perfil ", gtk_entry_get_text (txtPerfil), "\n");
             g_fprintf (pOrigen, opcion);
             opcion = g_strconcat("nombre ", gtk_entry_get_text (txtNombre), "\n");
             g_fprintf (pOrigen, opcion);
             opcion = g_strconcat("dni ", gtk_entry_get_text (txtDni), "\n");
             g_fprintf (pOrigen, opcion);
         }         
    }
    
    pclose (pOrigen);

    gtk_widget_hide (wNuevoAfiliado);
    Cargo_afiliados ();
}