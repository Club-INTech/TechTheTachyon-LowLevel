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

static HardwareSerial& DebugSerial = Serial2;

// Définition des différentes positions
static float positionDroit[3] = {180.0f,180.0f,180.0f};
static float positionAccelerateur[3] = {186.0f, 55.0f, 141.0f};
static float positionStockage[3] = {174.50f, 255.64f, 80.87f};
static float positionIntermediaire[3] = {195.18f, 249.83f, 82.19f};
static float positionPreDistributeur[3] = {329.41f, 270.77f, 90.57f};
static float positionDistributeur[3] = {295.77f, 216.30f, 195.18f};
static float positionSol[3] = { /*Valeur de test:*/ 278.0f /*272.0f*/, 97.0f, 189.0f};
static float* stepsFromStorageToDistrib[] = {
        positionStockage, positionIntermediaire, positionPreDistributeur, positionDistributeur
};
static float* stepsFromDistribToStorage[] = {
        positionDistributeur, positionPreDistributeur, positionIntermediaire, positionStockage
};

#endif //LL_ACTUTORVALUES_H
