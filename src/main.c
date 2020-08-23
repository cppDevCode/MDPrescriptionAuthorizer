#include <gtk/gtk.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "nuevoafiliado.h"
#include "opciones.h"
#include <stdbool.h>


//Construyo label
GtkLabel *label;
GtkLabel *error;
GtkLabel *labEstado;
GtkComboBox *wCmbAfiliado;
GtkComboBoxText *wCmbPractica;
GtkSpinButton *wCntCantidad;
GtkFileChooser *wSelArchivo;
GtkToggleButton *wChbAuto;
GtkButton *enviar;
GtkCheckButton *wChbInternacion;
GtkEntry *wEntrada;
GtkWidget *wError;
GtkWidget *wAcerca;
GtkButton *btnError;


char *mail = "autorizaciones@medife.com.ar";
char *ArchConfig = "/home/raquel/.config/amedife/config";
char *RutaScript = "bash /home/alejandro/bin/amedife.sh ";
char *RutaScan = "/tmp/scan.jpg";
char *BdAmbulatorio = "/home/raquel/.config/amedife/bd_ambulatorio";
char *BdInternacion = "";//"/home/raquel/.config/amedife/bd_internacion";


void Cargo_afiliados ();
bool CargoOpciones (gchar **config, char **rutaComando, char **orden, char **baseAmbulatorio, char **baseInternados);



int main (int argc, char *argv[])
{
    GtkBuilder      *builder; 
    GtkBuilder      *builder1; 
    GtkWindow       *window;
    bool agregoNulo;
    

 
 
    gtk_init (&argc, &argv);
 
    builder = gtk_builder_new ();
    builder1 = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "glade/main.glade", NULL);
    gtk_builder_add_from_file (builder1, "glade/error.glade", NULL);
 
    window = GTK_WINDOW (gtk_builder_get_object (builder, "main"));
    gtk_builder_connect_signals (builder, NULL);

    label=GTK_LABEL (gtk_builder_get_object (builder, "label"));
    labEstado=GTK_LABEL(gtk_builder_get_object (builder, "lab_estado"));
    wCmbAfiliado = GTK_COMBO_BOX (gtk_builder_get_object (builder, "cmb_afiliado"));
    wCmbPractica = GTK_COMBO_BOX_TEXT (gtk_builder_get_object (builder, "cmb_practica"));
    wCntCantidad = GTK_SPIN_BUTTON (gtk_builder_get_object (builder, "cnt_cantidad"));
    wSelArchivo = GTK_FILE_CHOOSER (gtk_builder_get_object (builder, "sel_archivo"));
    wChbAuto = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "cbx_auto"));
    wChbInternacion = GTK_CHECK_BUTTON (gtk_builder_get_object (builder, "chb_internacion"));
    enviar = GTK_BUTTON (gtk_builder_get_object (builder, "enviar"));
    wEntrada = GTK_ENTRY (gtk_builder_get_object (builder,"entrada"));
    wError = GTK_WIDGET (gtk_builder_get_object (builder1, "error"));
    
    
    
    error = GTK_LABEL (gtk_builder_get_object (builder1, "lab_error"));
    btnError = GTK_BUTTON (gtk_builder_get_object (builder1, "btn_error"));
    
    //g_signal_connect(window, "key_press_event", G_CALLBACK(check_escape), NULL);
           
    g_object_unref (builder);
    g_object_unref (builder1);
    
    
   agregoNulo = CargoOpciones (&ArchConfig, &RutaScript, &RutaScan, &BdAmbulatorio, &BdInternacion);
   if (agregoNulo) {
    ArchConfig[strlen(ArchConfig) - 1] = '\0';
    RutaScript[strlen(RutaScript) - 1] = '\0';
    RutaScan[strlen(RutaScan) - 1] = '\0';
    BdAmbulatorio[strlen(BdAmbulatorio) - 1] = '\0';
    BdInternacion[strlen(BdInternacion) - 1] = '\0';
   }
      
   printf("%s\n",ArchConfig);
   printf("%s\n",RutaScript);
   printf("%s\n",RutaScan);
   printf("%s\n",BdAmbulatorio);
   printf("%s\n",BdInternacion);
   if ( access ( ArchConfig, F_OK ) == -1 )
    {
       NuevoAfiliado ();
    }
    Cargo_afiliados ();
    on_cmb_practica_show ();
    gtk_main ();
   
    return 0;
}



//clic en enviar
void btn_enviar()
{
    
    guint32 cantidad = 0;
    gchar out_str[100] = {0}; // buffer for string
    //mensaje de error inicializadores
        
    gchar *aux = RutaScript;
    gchar *msg_error = "No se pudo ejecutar la orden";
    gchar *dir = RutaScan;
    
    
    gtk_button_set_label (enviar, "ENVIANDO AUTORIZACION...");
        
     
    
    //obtengo cantidad
    cantidad = gtk_spin_button_get_value_as_int (wCntCantidad);
    g_snprintf (out_str, sizeof (out_str), "%d", cantidad);

    gtk_button_set_label (enviar, "Enviar");
    if (gtk_toggle_button_get_active (wChbAuto))
    {     
        aux = g_strjoin (NULL, aux, gtk_combo_box_get_active_id (wCmbAfiliado), " auto ", "\"", gtk_combo_box_text_get_active_text (wCmbPractica), "\"", " ", out_str, " ", dir, mail, (gchar*)(char*)NULL);
    }
    else
        {
        if (gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (wSelArchivo)) == NULL)
        {
            gtk_widget_show (wError);
            gtk_label_set_text (error, "Debe seleccionar la orden de autorizacion!!!");    
        }
        else
        {
            aux = g_strjoin(" ", aux, gtk_combo_box_get_active_id (wCmbAfiliado), " auto ", "\"", gtk_combo_box_text_get_active_text (wCmbPractica), "\"", " ", out_str, " ", gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (wSelArchivo)), mail, (gchar*)(char*)NULL);  
        }
        }
    
    FILE *fp;
    char path[1035];    
    
    fp = popen (aux, "r");
    if ( fp == NULL ) {   
        gtk_widget_show (GTK_WIDGET (wError));
        gtk_label_set_text (error, msg_error);
        exit (1);
    }
    while (fgets (path, sizeof (path) - 1, fp) != NULL) {
        gtk_label_set_text (labEstado, path);    
    }
    pclose (fp);
}
//clic en el boton salir
void btn_salir ()
{
    gtk_main_quit ();
}
// called when window is closed
void on_window_main_destroy ()
{
        gtk_main_quit ();
}

//Cierro dialogo de error
void on_btncerrar_button_press_event ()
{
    gtk_widget_hide (GTK_WIDGET (wError));
}


void on_cbx_auto_toggled ()
{
  if (gtk_toggle_button_get_active (wChbAuto) == FALSE)
  {
        gtk_widget_show (GTK_WIDGET (wSelArchivo));
        gtk_widget_show (GTK_WIDGET (label));
        gtk_label_set_text (labEstado, "Recuerde: SELECCIONAR EL ARCHIVO");
  }
  else
  {
        gtk_label_set_text (labEstado, "Ponga la foto en: /tmp/scan.jpg");
        gtk_widget_hide (GTK_WIDGET (wSelArchivo));
        gtk_widget_hide (GTK_WIDGET (label));
  }
}

void on_chb_internacion_toggled ()
{
    FILE *file;
    char practica[1035];
    gint item = 0;  
    gtk_list_store_clear (GTK_LIST_STORE (gtk_combo_box_get_model (GTK_COMBO_BOX (wCmbPractica))));        
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (wChbInternacion)) == TRUE)
    {
        mail = "internaciones@medife.com.ar";
        file = fopen (BdInternacion, "r");
    }
else
    {
        mail = "autorizaciones@medife.com.ar";
        file = fopen (BdAmbulatorio, "r");
    }
    while (fgets (practica, sizeof (practica) - 1, file) != NULL) 
    {
        practica[strlen (practica) - 1] = '\0';
        gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (wCmbPractica), practica);
        gtk_combo_box_set_active (GTK_COMBO_BOX (wCmbPractica), item);    
    }
    fclose (file);
if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (wChbInternacion)) == FALSE)
    {
        gtk_combo_box_set_active (GTK_COMBO_BOX (wCmbPractica),367);
    }
}




void on_cmb_practica_show ()
{
    FILE *file;
    char practica[1035];
    gint item=0;  
        
   

    file = fopen (BdAmbulatorio, "r");
    
    while (fgets (practica, sizeof (practica) - 1, file) != NULL) 
    {
        practica[strlen (practica) - 1] = '\0';
        gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (wCmbPractica), practica);
        gtk_combo_box_set_active (GTK_COMBO_BOX (wCmbPractica), item);    
    }
    fclose (file);
    gtk_combo_box_set_active (GTK_COMBO_BOX (wCmbPractica), 367);
}

gboolean on_error_key_press_event (GtkWidget *widget, GdkEventKey  *event, gpointer   user_data)
{
  if ( event->keyval == GDK_KEY_Escape) 
  {
     gtk_main_quit();
     
     return TRUE;    
  }
  return FALSE;
}

void on_menu_acerca_activate ()
{
    GtkBuilder      *builder; 

    builder = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "glade/acerca.glade", NULL);
    gtk_builder_connect_signals (builder, NULL);
    wAcerca = GTK_WIDGET (gtk_builder_get_object(builder, "acerca"));
    
    g_object_unref (builder);
    gtk_dialog_run (wAcerca);
 }

void close_cb (GtkAboutDialog *about)
{
    gtk_widget_hide (wAcerca);
}

void on_btnNuevoAfiliado_clicked ()
{
    NuevoAfiliado ();
}

void Cargo_afiliados ()
{
    FILE *fp;
    gchar perfil[1035];    
    gchar afiliado[1035];
    char temp[1035];
    char temp2[1035];
    GtkListStore *listaAlmacenamiento;
    GtkCellRenderer *columna;
    
    listaAlmacenamiento = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_STRING);
    fp = fopen (ArchConfig, "r");
    if ( fp == NULL ) 
    {   
        printf ("ERROR");
    }
    else
    {  
        while (fgets (perfil, sizeof(perfil) - 1, fp) != NULL) 
        {
            fgets (afiliado, sizeof (afiliado) - 1, fp);
            strncpy (temp, perfil + 6, 200);
            temp[strlen (temp) - 1] = '\0';
            strncpy (temp2, afiliado + 6, 200);
            temp2[strlen (temp2) - 1] = '\0';
            gtk_list_store_insert_with_values (listaAlmacenamiento, NULL, -1, 0, temp2, 1, temp, -1);      
            fgets(afiliado,sizeof (afiliado) - 1, fp);
        }
    }
    fclose (fp);
    gtk_combo_box_set_model(wCmbAfiliado, GTK_TREE_MODEL (listaAlmacenamiento));
    g_object_unref (listaAlmacenamiento);

    columna = gtk_cell_renderer_text_new ();
    gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (wCmbAfiliado), columna, TRUE);
    //gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(w_cmb_afiliado), columna,
     //                              "cell-background", 0,
       //                            "text", 1,
         //                          NULL);

    gtk_combo_box_set_active (GTK_COMBO_BOX (wCmbAfiliado), 0);

    

}

void on_m_opciones_activate ()
{
AbrirOpciones ();
}

bool CargoOpciones (gchar **config, char **rutaComando, char **orden, char **baseAmbulatorio, char **baseInternados)
{
FILE *pArchivo=NULL;
    char buffer2[1035];
    char aux[255];
    gchar *casadir="";
    
    
    gchar *archPreferencias = "";
    
    casadir=getenv("HOME");
    //casadir[strlen (casadir) - 1 ] = '\0';
    
    archPreferencias= g_strjoin(NULL,casadir,"/.config/amedife/preferencias",(gchar*)(char*)NULL);
    printf("%s\n",archPreferencias);
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
            strncpy (aux, buffer2 + 15, sizeof(aux));
            
            *baseAmbulatorio = g_strjoin(NULL, aux, (gchar*)(char*)NULL);    
            
            fgets (buffer2, sizeof (buffer2) - 1, pArchivo);
            strncpy (aux, buffer2 + 13, sizeof(aux));
            *baseInternados = g_strjoin(NULL, aux, (gchar*)(char*)NULL);    ;
        
            fgets (buffer2, sizeof (buffer2) - 1, pArchivo);
            strncpy (aux, buffer2 + 13, sizeof(aux));
            *rutaComando = g_strjoin(NULL,aux,(gchar*)(char*)NULL); 
            fgets (buffer2, sizeof (buffer2) - 1, pArchivo);
            strncpy (aux, buffer2 + 14, sizeof(aux));
            *config = g_strjoin(NULL,aux,(gchar*)(char*)NULL);    
            fgets (buffer2, sizeof (buffer2) - 1, pArchivo);
            strncpy (aux, buffer2 + 5, sizeof(aux));
            *orden = g_strjoin(NULL,aux,(gchar*)(char*)NULL);    
         }
         fclose(pArchivo);
         return TRUE;
    }
    else
    {
        *baseAmbulatorio = g_strjoin(NULL,casadir,"/.config/amedife/bd_ambulatorio",(gchar*)(char*)NULL);    
        *baseInternados = g_strjoin(NULL,casadir,"/.config/amedife/bd_internacion",(gchar*)(char*)NULL);      
        *rutaComando = g_strjoin(NULL,"bash ",casadir,"/bin/amedife.sh ",(gchar*)(char*)NULL);      
        *config = g_strjoin(NULL,casadir,"/.config/amedife/config",(gchar*)(char*)NULL);      
        *orden="/tmp/scan.jpg";
        return FALSE;
    }
    
}

char *obtenerOpcion (char *opcion)
{
    if (strcmp (opcion,"dirConfiguracion") == 0)
    {
        return ArchConfig;
    }
    else
    {
        return "FALSE";
    }
    
}