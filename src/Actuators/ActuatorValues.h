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

static Stream& DebugSerial = Serial2;

// Définition des différentes positions
static float positionDroit[3] = {180.0f,180.0f,180.0f};
static float positionAccelerateur[3] = {186.0f, 55.0f, 141.0f};
static float positionStockage[3] = {174.50f, 255.64f, 80.87f};
static float positionIntermediaire[3] = {195.18f, 249.83f, 82.19f};
static float positionPreDistributeur[3] = {329.41f, 270.77f, 90.57f};
static float positionDistributeur[3] = {295.77f, 216.30f, 195.18f};

//old: static float positionGoldonium[3]={250.0f, 190.0f, 140.0f};

static float positionSolIntermediaire[3] = { 282.303986f, 128.744003f, 212.783997f};
static float positionSol[3] = {304.656006f, 162.447998f, 199.847992f};//{ 301.84f, 157.25f, 213.66f};
static float positionRecule[3] = {0,0,0};
static float coupleSolseuil[3][4] = {{103,102,101,100},{114,113,112,111},{124,123,122,121}};



/**
 * positions du secondaire pour les AXs
 */
/*
static float positionAccelerateurSecondaire[3] = {213.4f,180.0f,151.4f};
static float positionStockageSecondaire[3] = { 140.00f, 75.00f, 55.00f};
static float positionIntermediaireSecondaire[3] = {150.00f, 80.00f, 60.00f};
static float positionIntermediaireSecondaire2[3] = {249.00f, 70.00f, 55.00f};
static float positionDistributeurSecondaire[3] = {249.0f, 163.0f, 130.0f};

static float positionBalance[3]={160.0f,210.0f,160.0f};
static float positionMusclor[3]={250.0f, 60.0f,150.0f};
static float positionMusclor2[3]={250.0f, 47.0f,150.0f};
static float positionMusclor3[3]={250.0f, 47.0f,140.0f};
static float positionMusclor4[3]={235.0f, 47.0f,140.0f};
static float positionMusclor5[3]={250.0f, 47.0f,90.0f};
static float positionGoldonium[3]={250.0f, 95.0f,189.0f};
static float positionPreGoldonium[3]={180.0f,230.0f,130.0f};
*/


/**
 * positions du secondaire pour des XLs
 */

const static float cst=30;  // constante pour passer de XL (home à 180) à AX (home à 150)

static float positionAccelerateurSecondaire[3] = {213.4f+cst, 360-(180.0f+cst),151.4f+cst};
static float positionStockageSecondaire[3] = { 140.00f+cst, 360-(75.00f+cst), 55.00f+cst};
static float positionIntermediaireSecondaire[3] = {150.00f+cst, 360-(80.00f+cst), 60.00f+cst};
static float positionIntermediaireSecondaire2[3] = {249.00f+cst, 360-(70.00f+cst), 55.00f+cst};
static float positionDistributeurSecondaire[3] = {249.0f+cst, 360-(163.0f+cst), 130.0f+cst};

static float positionBalance[3]={160.0f+cst, 360-(210.0f+cst), 160.0f+cst};
static float positionMusclor[3]={285.648010f, 278.695984f, 98.648003f};//{250.0f+cst, 360-(60.0f+cst), 150.0f+cst};
static float positionMusclor2[3]={288.639984f, 277.992004f, 84.304001f};//{250.0f+cst, 360-(60.0f+cst), 150.0f+cst};
//static float positionMusclor2[3]={285.648010f, 278.695984f, 98.648003f};//{250.0f+cst, 360-(60.0f+cst), 150.0f+cst};
//static float positionMusclor[3]={285.648010f, 278.695984f, 279.928009f};//{250.0f+cst, 360-(60.0f+cst), 150.0f+cst};
//static float positionMusclor2[3]={257.928009f, 269.455994f, 83.599998f};//{250.0f+cst, 360-(60.0f+cst), 150.0f+cst};
//static float positionMusclor2[3]={250.0f+cst, 360-(47.0f+cst), 150.0f+cst};
static float positionMusclor3[3]={269.544006f, 250.184006f, 121.264000f};//{203.279999f, 109.384003f, 82.720001f};//{250.0f+cst, 360-(47.0f+cst), 140.0f+cst};
static float positionMusclor4[3]={235.0f+cst, 360-(47.0f+cst), 140.0f+cst};
static float positionMusclor5[3]={250.0f+cst, 360-(47.0f+cst), 90.0f+cst};
//static float positionGoldonium[3]={250.0f+cst, 360-(95.0f+cst), 189.0f+cst};
static float positionGoldonium[3]={285.648010f, 278.695984f, 279.928009f};//{212,89,147};
static float positionPreGoldonium[3]={271.303986f, 281.687988f, 279.664001f};//{291.015991f, 278.432007f, 86.503998f};//{180.0f+cst, 360-(230.0f+cst), 130.0f+cst};

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
