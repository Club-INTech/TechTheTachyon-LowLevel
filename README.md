# TechTheTachyon-LowLevel
## SETUP
Pour pouvoir utiliser le projet:
- Installez python2, puis platformio
- Clonez ce dépot
- Ouvrez un terminal dans le dossier du dépot, et effectuez la commande \
```pio init --ide IDE board teensy35```\
avec IDE=clion si vous utilisez CLion, ou vscode si vous utilisez Visual Studio Code
- Ouvrez le dossier du dépot avec CLion ou VSCode.
- Gagnez la coupe

## TODO

- [x] Communication Ethernet de base
- [x] Ordres Des Actionneurs
- [x] Asservissement
- [ ] Asservissement encore meilleur
- [x] Capteurs
- [x] Hooks
- [x] Canal de Position (établir la fréquence d'envoie et le header associé avec le HL) & détection de fin de mouvement
- [x] Détection des blocages physiques et transmission sur le canal Event
- [x] Mise en place de la basic Detection (Envoi d'event si obstacle trop proche)
- [x] Fusion de la gestion Série/Ethernet
- [x] Gérer les ordres grâce à une classe pour que ce soit propre
- [ ] Mise en place de cas d'erreur explicites et plus nombreux pour le HL
- [ ] MotionControl point à point(avec réorientation progressive)
- [ ] Ordre propre pour la récupération de données d'asservissement + scripts en conséquence
- [ ] Penser à implémenter les nouveaux ordres

## TABLE DES ORDRES
### ORDRES HL ⇒ LL
#### ORDRES GENERAUX

|        Ordres         |                           Actions                         |
|:---------------------:|:---------------------------------------------------------:|                  
|          ?            |                         Ping le LL                        |
|          j            |           Active l'attente de l'activation du jumper      |
|         sus           |        Switch les US ou choisit leur état (on/off)        |
|          f            |                     Check le mouvement                    |
|        ?xyo           |                   Position + Orientation                  |
|          d            |                      Translate de x mm                    |
|          t            |     Tourne de α rad, dans le sens demandé ou librement    |
|        stop           |                            Stop                           |
|         cx            |                       Set x d'origine                     |
|         cy            |                       Set y d'origine                     |
|         co            |                       Set α d'origine                     |
|        cxyo           |                     Set x,y,α d'origine                   |
|        ctv            |                    Set translation speed                  |
|        crv            |                     Set rotation speed                    |
|        ctrv           |               Set translation+rotation speed              |
|        efm            |                    Enable forced movment                  |
|        dfm            |                   Disable forced movment                  |
|        ct0            |              Désactive l'asserv en translation            |
|        ct1            |               Active l'asserv en translation              |
|        cr0            |               Désactive l'asserv en rotation              |
|        cr1            |                 Active l'asserv en rotation               |
|        cv0            |                Désactive l'asserv en vitesse              |
|        cv1            |                 Active l'asserv en vitesse                |
|       seti2c          |                    Set les capteurs I2C                   |
|        cod            |                Retourne les ticks de codeuse              |
|      pfdebug          |                Info de debug sur la position              |
|      rawpwm           |            Demande un PWM brut aux deux moteurs           |
|      getpwn           |              Retourne le PWM des deux moteurs             |
|      errors           |             Retourne les erreurs d'incertitude            |
|     rawdistance       |             Retourne la distance brute en ticks           |
|      rawspeed         |                   Vitesse brute des roues                 |
|     rawposdata        |             Pos x,y,α; vL,vR, targetvL,targetvR           |
|     montlhery         |                    Mode de présentation                   |
|         av            |                           Avance                          |
|         rc            |                           Recule                          |
|         td            |                       Tourne à droite                     |
|         tg            |                       Tourne à gauche                     |
|       sstop           |                            Arrêt                          |
|         nh            |     Créé un nouveau hook (id,x,y,r,α,tolerance,action)    |
|         eh            |                       Active le hook                      |
|         dh            |                     Désactive le hook                     |
|    emergencyStop      |  Arrète brutalement le robot et ***DESACTIVE*** l'asserv  |
|  resumeEmergencyStop  |          Redemarre l'asserv après un emergencyStop        |

#### ORDRES DE CONTRÔLE D'ACTION

|   Ordres  |                       Actions                      | Arguments      |
|:---------:|:--------------------------------------------------:|:---------------|
|    XLm    |Envoie le XL-430 à un α en °                        |id XL / α       |
|    XLs    |Modifie la vitesse d'un XL-430                      |id XL / speed   |
|    dist   |Envoie le bras à la position "distributeur"         |side(left/right)|
|   grnd    |Envoie le bras à la position "sol"                  |side(left/right)|
|   stock   |Envoie le bras à la position "ascenceur"            |side(left/right)|
|     up    |Monte l'ascenceur                                   |steps           |
|    down   |Descend l'ascenseur                                 |steps           |
|    push   |Pousse le palet dans l'accélérateur                 |side(left/right)|
|    suck   |Active la pompe                                     |side(left/right)|
|  unsuck   |Désactive la pompe                                  |side(left/right)|
|  valveon  |Active l'électrovanne                               |side(left/right)|
|  valveoff |Désactive l'électrovanne                            |side(left/right)|
|   gold    |Envoie le bras à la position "goldonium"            |      /         |
|   bal     |Envoie le bras à la position "balance"              |side(left/right)|
|   elec    |Démarre l'électron                                  | /              |

### ORDRES DE DEBUG
|   Ordres  |                       Actions                      | Arguments      |
|:---------:|:--------------------------------------------------:|:---------------|
|  torque   |Affiche le couple du bras                           |side(left/right)|
|  Ntorque  |Affiche le couple du XL-430 numéro N                |id XL           |





### ORDRES SPECIFIQUES LL

|   Ordres  |                       Actions                      |
|:---------:|:--------------------------------------------------:|
|   toggle  |         Change le mode de réglage d'asserv         |
|  display  |          Retourne les constantes d'asserv          |
|    kpt    |              Set le kp de translation              |
|    kdt    |              Set le kd de translation              |
|    kit    |              Set le ki de translation              |
|    kpr    |                Set le kp de rotation               |
|    kdt    |                Set le kd de rotation               |
|    kit    |                Set le ki de rotation               |
|    kpg    |            Set le kp de vitesse à gauche           |
|    kdg    |            Set le kd de vitesse à gauche           |
|    kig    |            Set le ki de vitesse à gauche           |
|    kpd    |            Set le kd de vitesse à droite           |
|    kdg    |            Set le kd de vitesse à droite           |
|    kig    |            Set le ki de vitesse à droite           |
