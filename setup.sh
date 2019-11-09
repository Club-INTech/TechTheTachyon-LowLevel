#! /bin/bash

function installPlatformio {
	echo "Installation de platformio"
	sudo pip3 install platformio
	if [ -z "$(command -v pio)" ]; then
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
			sudo usermod "$(id -un)" -aG dialout
		fi

		echo "Installation terminée !"
	fi
}

echo "Initialisation du dépôt du bas niveau"
echo "Clonage des submodule"

git submodule update --init

echo "Fin du clonage"
echo "Initialisation avec platformio"

if [ -z "$(command -v platformio)" ]; then
	echo "Platformio n'est pas installé !"

	echo -n "Voulez vous tenter d'installer platformio ? [o/N] "
	read answer
	
	if [[ "$answer" =~ [yYoO] ]]; then
		echo ""
		if [ -z "$(command -v pip2)" ]; then
			echo "Pip3 n'est pas installé ! "

			echo -n "Voulez vous tenter d'installer pip3 ? [o/N] "
			read answer

			if [[ "$answer" =~ [yYoO] ]]; then
				if [ "$(command -v apt)" ]; then
					sudo apt install python3-pip
					if [ -z "$(command -v pip3)" ]; then
						echo "Échec de l'installation de pip3, abandon."
					else
						installPlatformio
					fi
				elif [ "$(command -v pacman)" ]; then
					sudo pacman -S python3-pip
					if [ -z "$(command -v pip3)" ]; then
						echo "Échec de l'installation de pip3, abandon."
					else
						installPlatformio
					fi
				else 
					echo "Je ne sais pas comment installer pip3 sur votre système, abandon."
				fi
			else
				echo "Pas d'installation de pip3."
			fi
		else
			installPlatformio
		fi
	else
		echo "Pas d'installation de platformio."
	fi
fi

if [ "$(command -v platformio)" ]; then
	pio init --ide clion --board teensy35
	echo "Platformio initialisé."
else
	echo "Impossible d'initialiser platformio"
fi
