#! /bin/bash

pythonGraph () {
	trap "" EXIT
	echo $1 $2 $3
	echo "DATAEND" >> "$1"
	shift
	python3 extraction_asserv.py "$1" "$2"
	exit
}

# Subshell allows cd-ing around without affecting user
(
# Moves to script origin
cd $(dirname "${BASH_SOURCE[0]}")

if [ ! -d ./serialOutput ]; then
	mkdir serialOutput
fi

if [ $# -eq 3 ]; then
	fileName="./serialOutput/serialOutput-"$1","$2","$3".csv"
else
	fileName="./serialOutput/serialOutput.csv"
fi

if [ -e "$fileName" ]; then
	fileName=".""$(echo $fileName | cut -f 2 -d".")""-$(date +'%Y-%m-%d @ %Hh%M').csv"
fi

touch "$fileName"
outFile=$(echo $fileName | cut -f 3 -d"/")


if [ ! -e /dev/ttyACM0 ]; then
	echo "Waiting for serial /dev/ttyACM0"
	echo "Retrying every 100ms"

	while [ ! -e /dev/ttyACM0 ]; do
		sleep 0.1
	done
fi


# Useless to trap signals if nothing has been setup/nothing could have been read
trap "pythonGraph \"$fileName\" \"$outFile\" $1" INT
trap "pythonGraph \"$fileName\" \"$outFile\" $1" TERM
trap "pythonGraph \"$fileName\" \"$outFile\" $1" EXIT

echo > /dev/ttyACM0
stty -F /dev/ttyACM0 115200 raw -echo -echoe -echok

while true; do
	read line < /dev/ttyACM0
	echo $line >> "$fileName"
	case "$line" in 
   		*"DATAEND"*)
   			 break;;
	esac
done

pythonGraph "$fileName" "$outFile" "$1"

)