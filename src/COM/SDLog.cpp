//
// Created by Teo-CD on 22/04/18.
//

#include "SDLog.h"


SDLog::SDLog()
/// Initiliase la carte SD
/**
 * Ouvre la communication avec la carte SD
 * Supprime les fichiers précédents et en crééant de nouveaux, vides
 * Si l'initialisation échoue, on bloque les écritures
 */
{
    if(!SD.begin(BUILTIN_SDCARD))
    {
        Serial.println("ERROR::SDCARD::Could not initialize SD card");
        sdStatus = false;
    }
    else
    {
        sdStatus = true;
        for(uint8_t i = 0;i<fileCount;i++)
        {
            SD.remove(fileList[i]);
            fileStreams[i] = SD.open(fileList[i],FILE_WRITE);
            if(!fileStreams[i])
            {
                Serial.printf("ERROR:SDCARD::Could not create file %s\n", fileList[i]);
            }
        }
    }
    Serial.println("SD card ready");

    startingTime = 0;
}

SDLog::~SDLog()
{
    for (File &fileStream : fileStreams) {
        fileStream.close();
    }
}


bool SDLog::logWrite(const String message)
/// Écrit dans la SD
/**
 * Écrit le message dans le fichier correspondant à son canal et dans
 * le fichier général, ou seulement le fichier général si il n'a pas de header
 * @param message: message à logger
 * @return Vrai si on a bien réussi à écrire, faux sinon ou si la carte SD n'est pas init
 */
{
    if(!sdStatus)
        return(false);

    if(message.length() >= 6)
    {
        if(message.substring(0,5).equals("Reçu: "))
        {
            if(!logToFile(message,1))
            {
                return(false);
            }
        }
    }
    if(message.length() >= 2)
    {
        for(uint8_t i = 0;i<fileCount;i++)
        {
            if(message[0] == headerList[i][0] && message[1] == headerList[i][1])
            {
                if(!logToFile(message.substring(2),i+2))      // Permet de garder le fichier général
                {                                                       // en indice 0 et la lecture en indice 1,
                    return(false);                                      // pour éventuellement rajouter
                }                                                       // d'autres canaux en changeant le minimum
                return(logToFile(message.substring(2),0));
            }
        }
    }
    return(logToFile(message,0));
}

bool SDLog::logWrite(const char* message)
{
    return(logWrite(String(message)));
}

bool SDLog::logWriteReception(const char* message)
/// Écrit les entrées dans la SD
/**
 * Permet d'écrire dans le fichier "input.txt" et le fichier général les messages
 * reçus, en ajoutant un indicateur de la réception
 * @param message : Message reçu a logger
 * @return Vrai si l'écriture a réussi
 */
{
    if(!sdStatus)
        return(false);

    String receptionMessage = String("Reçu: ").append(message);
    if(!logToFile(receptionMessage,1))
    {
        return(false);
    }
    return logToFile(receptionMessage,0);
}

bool SDLog::logToFile(const String &message,uint8_t fileIndex)
/**
 * Gère l'écriture dans le fichier, permet de savoir si elle a été réussie et
 * ajoute le timestamp à la ligne de log
 * @param message : message à logger
 * @param logFile : fichier de log correspondant
 * @return Vrai si on a réussi à écrire dans le fichier
 */
{
    File& outputFile = fileStreams[fileIndex];
    if(!outputFile)
    {
        Serial.printf("ERROR::SDCARD::Could not write to file %s\n",fileList[fileIndex]);
        return(false);
    }
    outputFile.printf("[Time: %lu] ",millis()-startingTime);
    outputFile.println(message);
    outputFile.flush();
    return(true);
}

void SDLog::setStartingTime()
{
    startingTime = millis();
}

SDLog::operator bool()
{
    return(sdStatus);
}