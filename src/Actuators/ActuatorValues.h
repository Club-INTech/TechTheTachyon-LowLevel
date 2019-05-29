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

static Stream& DebugSerial = Serial2;//SerialUSB;

// Définition des différentes positions
const static float positionDroit[3] = {180.0f,180.0f,180.0f};
const static float positionRecule[3] = {302.0f,280.0f,247.0f};
const static float positionAccPoussePalet[3] = {286.0f, 247.0f, 219.0f};

const static float positionAccelerateur[3] = {292.0f, 272.0f, 244.0f};
const static float positionAccelerateurMiddle[3] = {200.0f, 90.0f, 135.0f+10};
const static float positionAccelerateurMiddle2[3] = {210.0f, 90.0f, 135.0f};
const static float positionAccelerateurMiddle3[3] = {215.0f, 80.0f, 135.0f};
const static float positionAccelerateurMiddleDepot[3] = {220.0f, 80.0f, 135.0f};


const static float positionAccelerateur7Middle3[3] = {200.0f, 90.0f, 130.0f+10};
//const static float positionAccelerateur7MiddleDepot[3] = {220.0f, 83.0f, 120.0f};
const static float positionAccelerateur7MiddleDepot[3] = {210.151993f, 84.351997f, 145.000f};

const static float positionAccelerateur8Middle3[3] = {200.0f, 80.0f, 110.0f+10};
const static float positionAccelerateur8MiddleDepot[3] = {200.0f, 80.0f, 130.0f};

const static float positionAccBIS[3] = {193.0f, 235.0f, 90.0f};
const static float positionAccTER[3] = {287.0f, 290.0f+5.0f, 185.0f-5.0f};
const static float positionStockageDepot[3] = { 195.0f, 250.0f, 95.0f};//{170.0f, 232.0f, 55.5f};
const static float positionStockagePrise[3] = {175.0f, 245.0f, 75.0f};
const static float positionIntermediaire[3] = {195.18f, 249.83f-10, 82.19f+10};
const static float positionIntermediaireDist2Stock[3] = {190.0f, 85.0f, 135.0f};

const static float positionPrePreDistributeur[3] = {211.464005f, 76.736000f, 37.840000f};//{254.50f, 230.21f, 122.23f};
const static float positionPreDistributeur[3] = {235.664001f, 77.000000f, 103.575996f};//{246.46f, 85.00f, 100.00f};
const static float positionDistributeur[3] = {250.00f, 115.00f, 135.0f};

const static float positionBalance[3]={182.6f,101.6f,181.7f};
const static float positionHoldBlue[3]={188.584000f, 101.463997f, 181.016006f};

const static float positionDeposeZoneDepart[3]={196.944000f, 108.503998f, 252.559998f};
const static float positionDeposeZoneDepartIntermediaireVersAscenceur[3]={196.944000f, 108.503998f, 75.0f};

const static float positionBalancePrincipal[3]={220.0f,150.0f,181.7f}; // dépot du palet bleu de X6
const static float positionGoldonium[3]={213.4f,101.8f,151.4f};
const static float positionSolIntermediaire[3] = { /*Valeur de test:*/ 278.0f-5 /*272.0f*/, 97.0f+5, 190.0f};
const static float positionSolGauche[3] = { /*Valeur de test:*/ 278.0f+10 /*272.0f*/, 97.0f+18, 190.0f};
const static float positionSolDroit[3] = { /*Valeur de test:*/ 283.45f /*272.0f*/, 106.57f, 179.69f};
const static float coupleSolseuil[3][4] = {
        {103,102,101,100},
        {114,113,112,111},
        {124,123,122,121}
};
/** Ligne référence le XL et Colonne le type de Palet du plus lourd au plus léger (bleu, vert, rouge, vide)*/
const static float coupleDistributeurseuil[3][4] = {{103,102,101,100},{114,113,112,111},{124,123,122,121}};

const static float positionLiberationAsc[3] = {170.895996f, 172.479996f, 91.695999f};

const static float xlOffsets[6] = {
        -4.0f, +5.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
};

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
        positionStockagePrise, positionIntermediaire, positionPreDistributeur, positionDistributeur
};
const static float* stepsFromDistribToStorage[] = {
        positionDistributeur, positionPreDistributeur, positionIntermediaire, positionStockageDepot
};

#endif //LL_ACTUTORVALUES_H
