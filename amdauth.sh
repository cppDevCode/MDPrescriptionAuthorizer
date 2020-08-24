#!/bin/bash

#Version 20190316_1145
#Uso:
#amdauth perfil auto codigo cantidad archivo
#Ejecutables
WHOAMI=$(which whoami)
SENDEMAIL=$(which sendemail)
DIRECTORIO=$(which xdg-user-dir)
MAIL_FROM="from@gmail.com"
MAIL_CC="cc@gmail.com"
MAIL_TO="to@gmail.com"
SMTP_SERVER="smtp.gmail.com:587"
MAIL_USER="from"
MAIL_PASS="passwd"

#Variables
USER=$($WHOAMI)
CONFIG="/home/$USER/.config/amdauth/config"
IMAGENES=$($DIRECTORIO PICTURES)
PRESCRIPCION="$IMAGENES/amdauth"


function addperfil ()
{
	echo "es: $1"
	echo "perfil $1">>$CONFIG
	echo "Creando carpeta de prescipciones del perfil..."
	mkdir $PRESCRIPCION/$1
	echo "Escriba Nombre completo:"
	read nombre
	echo "nombre $nombre">>$CONFIG
	echo "Escriba DNI:"
	read dni
	echo "dni $dni">>$CONFIG
	exit

}
if [ ! -d $PRESCRIPCION ]; then
	echo -e "Primera ejecucion de aamed...\nCreando directorio amdauth..."
	mkdir $PRESCRIPCION
	echo "Directorio Creado en $PRESCRIPCION"
	echo "Escriba nombre de Perfil Inicial..."
	read perfil
	mkdir /home/$USER/.config/amdauth
	echo "perfil $perfil">>$CONFIG
	echo "Escriba Nombre completo:"
	read nombre
	echo "nombre $nombre">>$CONFIG
	echo "Escriba DNI:"
	read dni
	echo "dni $dni">>$CONFIG
	echo "Creando carpeta de prescipciones del perfil..."
	mkdir $PRESCRIPCION/$perfil
	echo "Perfil Creado :)"
fi

if [ "$1" == "-h" ]; then
	echo -e "Agregar nuevo perfil: amdauth.sh add PERFIL"
elif [ "$1" == "add" ]; then
	 perfil = "$2"
	 addperfil $2 

fi

#LEO CONFIG
ESPERFIL=0
while read -r name value
		do
	if [ "$name" == "perfil" ] && [ "${value//\"/}" == "$1" ]; then
		ESPERFIL=3
	fi
	if [ $ESPERFIL -gt 0 ]; then
		if [ "$name" == "nombre" ]; then
				NOMBRE=${value//\"/}
																										else
		if [ "$name" == "dni" ]; then
				DNI=${value//\"/}
		fi
	fi
	ESPERFIL=$(($ESPERFIL - 1))
fi
done < $CONFIG

#ejemplo fecha 11/10/2018
#SCAN=111018.jpg

if [ "$2" != "auto" ]; then
	SCAN="$(date +%d%m%y).jpg"
	if [ ! -f $IMAGENES/amdauth/$1/$SCAN ]; then
			echo "No hay scan:$IMAGENES/amdauth/$1/$SCAN"
			exit
	fi 	
	if [ "$NOMBRE" != "" ]; then
		echo "Que practica quiere autorizar?"
		read practica
		enviado=$practica
		echo "Que cantidad?"
		read cantidad
		case "$practica" in
			"7-337001")
					practica="7-337001 | Psicopedagogia"
					;;
			"7-305107")
					practica="7-305107 | Retinografia digital"
					;;
			"7-299121")
					practica="7-299121 | POLISOMNOGRAFIA NOCT.+OXIMETRIA+TITULACION DE CPAP"
					;;
			"7-335001")
					practica="7-335001 | Psicologia"
		esac
		echo "Esta seguro de autorizar: $practica? [S/N]"
		read opcion
		if [ "$opcion" == "s" ] || [ "$opcion" == "S" ]; then
			sendemail -f $MAIL_FROM -t $MAIL_TO -cc $MAIL_CC -s $SMTP_SERVER -u "DNI $DNI | $NOMBRE | $practica x $cantidad" -m "Buenas tardes, para el afiliado $NOMBRE, DNI $DNI solicito:\nCodigo: $practica\nCantidad: $cantidad\n\nAdjunto prescripcion medica\nCualquier cosa a disposicion\nMuchas gracias\n\nAlejandro Lucas Baldres" -a $IMAGENES/amdauth/$1/$SCAN -xu $MAIL_USER -xp $MAIL_PASS
			mv $IMAGENES/amdauth/$1/$SCAN $IMAGENES/amdauth/$1/$(date +%d%m%y)-$enviado.jpg
		else echo "Cancelado!"
		fi
	fi
else
	sendemail -f $MAIL_FROM -t $6 -cc $MAIL_CC -s $SMTP_SERVER -u "DNI $DNI | $NOMBRE | $3 x $4" -m "Buenas tardes, para el afiliado $NOMBRE, DNI $DNI solicito:\nCodigo: $3\nCantidad: $4\n\nAdjunto prescripcion medica\nCualquier cosa a disposicion\nMuchas gracias\n\nAlejandro Lucas Baldres" -a $5 -xu $MAIL_USER -xp $MAIL_PASS
fi