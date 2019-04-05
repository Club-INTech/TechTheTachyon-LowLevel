#! /bin/bash

function installPlatformio {
	echo "Installation de platformio"
	sudo pip2 install platformio
	if [ -z "$(which pio)" ]; then
		echo "Échec de l'installation de platformio, abandon."
	else
		echo "Installation réussie, mise à jour de platformio"
		pio update

		echo "Téléchargement de la règle udev nécessaire au flash"
		sudo wget https://raw.githubusercontent.com/platformio/platformio-core/develop/scripts/99-platformio-udev.rules -O /etc/udev/rules.d/99-platformio-udev.rules

		if [ -z "$(sudo which service)" ]; then
			sudo udevadm control --reload-rules
			sudo udevadm trigger
		else
			sudo service udev restart
			sudo groupadd dialout
			sudo usermod $(id -un) -aG dialout
		fi

		echo "Installation terminée !"
	fi
}

echo "Initialisation du dépôt du bas niveau"
echo "Clonage des submodule"

git submodule update --init Dynamixel-Com
git submodule update --init TeensyStep

echo "Fin du clonage"
echo "Initialisation avec platformio"

if [ -z "$(which platformio)" ]; then
	echo "Platformio n'est pas installé !"

	echo -n "Voulez vous tenter d'installer platformio ? [y/n] "
	read answer
	
	if [[ "$answer" =~ [yY] ]]; then
		echo ""
		if [ -z "$(which pip2)" ]; then
			echo "Pip2 n'est pas installé ! "

			echo -n "Voulez vous tenter d'installer pip2 ? [y/n] "
			read answer

			if [[ "$answer" =~ [yY] ]]; then
				if [ "$(which apt)" ]; then
					sudo apt install python-pip
					if [ -z "$(which pip2)" ]; then
						echo "Échec de l'installation de pip2, abandon."
					else
						installPlatformio
					fi
				elif [ "$(which pacman)" ]; then
					sudo pacman -S python2-pip
					if [ -z "$(which pip2)" ]; then
						echo "Échec de l'installation de pip2, abandon."
					else
						installPlatformio
					fi
				else 
					echo "Je ne sais pas comment installer pip2 sur votre système, abandon."
				fi
			else
				echo "Pas d'installation de pip2."
			fi
		else
			installPlatformio
		fi
	else
		echo "Pas d'installation de platformio."
	fi
fi

if [ "$(which platformio)" ]; then
	pio init --ide clion --board teensy35
	echo "Platformio initialisé."
else
	echo "Impossible d'initialiser platformio"
fi
