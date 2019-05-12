//
// Created by trotfunky on 19/02/18.
//

#ifndef LL_ACTUTORVALUES_H
#define LL_ACTUTORVALUES_H

/*
 * AX12-IDs
 */

constexpr int frontArmGroup_ID = 0;
constexpr int frontLeftAX12_ID = 1;
constexpr int frontRightAX12_ID = 2;

constexpr int backArmGroup_ID = 1;
constexpr int backLeftAX12_ID = 4;
constexpr int backRightAX12_ID = 5;

constexpr int frontDoor_ID = 3;
constexpr int backDoor_ID = 6;


constexpr int armBaseSpeed = 250;

/*
 * Bras avant
 */

constexpr int frontLowCubes = 90;
constexpr int frontLowBee = 120;
constexpr int frontFolded = 182;

/*
 * Bras arrière
 */

constexpr int backLowCubes = 205;
constexpr int backLowBee = 180;
constexpr int backFolded = 120;

/*
 * Portes
 */

constexpr int doorBaseSpeed = 300;
constexpr int doorSmallOpeningAngle = 10;

constexpr int frontDoorOpen = 240;
constexpr int frontDoorClosed = 155;

constexpr int backDoorOpen = 150;
constexpr int backDoorClosed = 245;

static usb_serial_class& DebugSerial = SerialUSB;

// Définition des différentes positions
static float positionDroit[3] = {180.0f,180.0f,180.0f};
static float positionAccelerateur[3] = {186.0f, 55.0f, 141.0f};
static float positionStockage[3] = {174.50f, 255.64f, 80.87f};
static float positionStockageSecondaire[3] = { 140.00f, 75.00f, 55.00f};
static float positionIntermediaire[3] = {195.18f, 249.83f, 82.19f};
static float positionIntermediaireSecondaire[3] = {150.00f, 80.00f, 60.00f};
static float positionIntermediaireSecondaire2[3] = {249.00f, 70.00f, 55.00f};
static float positionPreDistributeur[3] = {329.41f, 270.77f, 90.57f};
static float positionDistributeur[3] = {295.77f, 216.30f, 195.18f};
static float positionDistributeurSecondaire[3] = {249.0f, 163.0f, 130.0f};
static float positionBalance[3]={160.0f,210.0f,160.0f};
static float positionMusclor[3]={240.0f, 60.0f,150.0f};
static float positionGoldonium[3]={205.0f, 190.0f, 140.0f};
static float positionSolIntermediaire[3] = { /*Valeur de test:*/ 278.0f-5 /*272.0f*/, 97.0f+5, 189.0f};
static float positionSol[3] = { /*Valeur de test:*/ 278.0f /*272.0f*/, 97.0f, 189.0f};
static float positionRecule[3] = {0,0,0};
static float coupleSolseuil[3][4] = {{103,102,101,100},{114,113,112,111},{124,123,122,121}};
/** Ligne référence le XL et Colonne le type de Palet du plus lourd au plus léger (bleu, vert, rouge, vide)*/
const static float coupleDistributeurseuil[3][4] = {{103,102,101,100},{114,113,112,111},{124,123,122,121}};

/** Couleurs des plalets */
enum class PaletColor
{
    // ordonné par masse décroissante
    GOLD,
    BLUE,
    GREEN,
    RED,
    NONE
};

static String PaletColorToString( const PaletColor color )
{
    switch(color)
    {
        case PaletColor::BLUE  : return "bleu";
        case PaletColor::GREEN : return "vert";
        case PaletColor::RED   : return "rouge";
        case PaletColor::GOLD  : return "goldenium";
        default                : return "inconnu";
    }
}

const static float* stepsFromStorageToDistrib[] = {
        positionStockage, positionIntermediaire, positionPreDistributeur, positionDistributeur
};
const static float* stepsFromDistribToStorage[] = {
        positionDistributeur, positionPreDistributeur, positionIntermediaire, positionStockage
};

#endif //LL_ACTUTORVALUES_H
