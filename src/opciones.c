#include <gtk/gtk.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

GtkWidget *wOpciones;
GtkEntry *txtConfig;
GtkEntry *txtComando;
GtkEntry *txtRutascan;
GtkEntry *txtBdInternados;
GtkEntry *txtBdAmbulatorios;



void AbrirOpciones()
{
    GtkBuilder      *builder; 
    
    builder = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "glade/opciones.glade", NULL);
    gtk_builder_connect_signals (builder, NULL);
    wOpciones = GTK_WIDGET (gtk_builder_get_object (builder, "w_opciones"));
    txtBdAmbulatorios = GTK_ENTRY (gtk_builder_get_object (builder, "txt_BDambulatorios"));
    txtBdInternados = GTK_ENTRY (gtk_builder_get_object (builder, "txt_BDinternados"));
    txtComando = GTK_ENTRY (gtk_builder_get_object (builder, "txt_comando"));
    txtConfig = GTK_ENTRY (gtk_builder_get_object (builder, "txt_config"));
    txtRutascan = GTK_ENTRY (gtk_builder_get_object (builder, "txt_rutascan"));

    g_object_unref (builder);


    
    gtk_dialog_run (wOpciones);
    
}

void on_btnCancelar_clicked ()
{
    gtk_widget_hide (wOpciones);
}

void on_w_opciones_show ()
{
    FILE *pArchivo=NULL;
    char buffer2[1035];
    char homedir[1035];
    char *aux;
    char *archPreferencias;
    
    gtk_entry_set_text (txtBdInternados,  "");
    strncpy(homedir, getenv("HOME"), strlen(getenv("HOME")));
    archPreferencias= g_strjoin(NULL,homedir,"/.config/amedife/preferencias",(gchar*)(char*)NULL);
    if ( access (archPreferencias, F_OK) != -1 )
    {
        pArchivo = fopen (archPreferencias, "r");
        if ( pArchivo == NULL ) 
        {
             printf ("ERROR ARCHIVO INEXISTENTE");
        }
        else
         {
            fgets (buffer2, sizeof (buffer2) - 1, pArchivo);
            strncpy (buffer2, buffer2 + 15, 255);
            buffer2[strlen (buffer2) - 1] = '\0';
            gtk_entry_set_text (txtBdAmbulatorios, buffer2);
            fgets (buffer2, sizeof (buffer2) - 1, pArchivo);
            strncpy (buffer2, buffer2 + 13, 255);
            buffer2[strlen (buffer2) - 1] = '\0';
            gtk_entry_set_text (txtBdInternados, buffer2);
            fgets (buffer2, sizeof (buffer2) - 1, pArchivo);
            strncpy (buffer2 ,buffer2 + 13, 255);
            buffer2[strlen (buffer2) - 1] = '\0';
            gtk_entry_set_text (txtComando, buffer2);
            fgets (buffer2, sizeof (buffer2) - 1, pArchivo);
            strncpy (buffer2, buffer2 + 14, 255);
            buffer2[strlen (buffer2) - 1] = '\0';
            gtk_entry_set_text (txtConfig, buffer2);
            fgets (buffer2, sizeof (buffer2) - 1, pArchivo);
            strncpy (buffer2, buffer2 + 5, 255);
            buffer2[strlen (buffer2) - 1] = '\0';
            gtk_entry_set_text (txtRutascan, buffer2);
         }
         fclose(pArchivo);
    }
    else
    {
        strcat(homedir, "/.config/amedife/bd_ambulatorio");
        gtk_entry_set_text (txtBdAmbulatorios, homedir);
        strncpy(homedir, getenv("HOME"), 255);
        strcat(homedir,"/.config/amedife/bd_internacion");
        gtk_entry_set_text (txtBdInternados,  homedir);
        strncpy(homedir, "bash ", 255);
        strcat(homedir,getenv("HOME"));
        strcat(homedir,"/bin/amedife.sh ");
        gtk_entry_set_text (txtComando, homedir);
        strncpy(homedir, getenv("HOME"), 255);
        strcat(homedir,"/.config/amedife/config");
        gtk_entry_set_text (txtConfig, homedir);
        gtk_entry_set_text (txtRutascan, "/tmp/scan.jpg");

    }
    
}

void on_btnGuardar_clicked ()
{
    FILE *pArchivo=NULL;
    gchar *texto="";
    char *archPreferencias = getenv("HOME");
    archPreferencias = g_strjoin(NULL,archPreferencias,"/.config/amedife/preferencias",(gchar*)(char*)NULL);
    pArchivo = fopen (archPreferencias, "w");
    if ( pArchivo == NULL ) {
             printf ("ERROR ARCHIVO INEXISTENTE");
    }
    else
         {
             texto = g_strconcat ("BDAmbulatorios ", gtk_entry_get_text (txtBdAmbulatorios), "\n");
             g_fprintf (pArchivo, texto);
             texto = g_strconcat ("BDinternados ", gtk_entry_get_text (txtBdInternados), "\n");
             g_fprintf (pArchivo, texto);
             texto = g_strconcat ("Comando ", gtk_entry_get_text (txtComando), "\n");
             g_fprintf (pArchivo, texto);
             texto = g_strconcat ("Configuracion ", gtk_entry_get_text (txtConfig), "\n");
             g_fprintf (pArchivo, texto);
             texto = g_strconcat ("Scan ", gtk_entry_get_text (txtRutascan), "\n");
             g_fprintf (pArchivo, texto);
         }
    fclose (pArchivo);
    gtk_widget_hide (wOpciones);
}
