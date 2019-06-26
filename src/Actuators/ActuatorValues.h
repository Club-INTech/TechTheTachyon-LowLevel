//
// Created by trotfunky on 19/02/18.
//

#ifndef LL_ACTUTORVALUES_H
#define LL_ACTUTORVALUES_H

// TODO : Envoi des déplacements de bras par le HL
//        Utilité que ce soit là plutôt qu'avec le reste de la config ?

static Stream& DebugSerial = Serial2;//SerialUSB;

// Définition des différentes positions
const static float positionDroit[3] = {180.0f,180.0f,180.0f};

// Please help
#if defined(MAIN)

const static float positionRecule[3] = {302.0f,280.0f,247.0f};
const static float positionAccPoussePalet[3] = {286.0f, 247.0f, 219.0f};

const static float positionAccelerateur[3] = {292.0f, 272.0f, 244.0f};
const static float positionAccelerateurMiddle[3] = {200.0f, 90.0f, 135.0f+10};
const static float positionAccelerateurMiddle2[3] = {210.0f, 90.0f, 135.0f};
const static float positionAccelerateurMiddle3[3] = {215.0f, 80.0f, 135.0f};
const static float positionAccelerateurMiddleDepot[3] = {220.0f, 80.0f, 135.0f};


const static float positionAccelerateur7Middle3[3] = {200.0f, 90.0f, 130.0f+10};
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
const static float positionPostDistributeur[3] = {230.00f, 100.00f, 135.0f};

const static float positionBalance[3]={182.6f,101.6f,181.7f};
const static float positionHoldBlue[3]={188.584000f, 101.463997f, 181.016006f};

const static float positionDeposeZoneDepart[3]={196.944000f, 108.503998f, 252.559998f};
const static float positionDeposeZoneDepartIntermediaireVersAscenceur[3]={196.944000f, 108.503998f, 75.0f};

const static float positionBalancePrincipal[3]={220.0f,150.0f,181.7f}; // dépot du palet bleu de X6
const static float positionGoldonium[3]={213.4f,101.8f,151.4f};
const static float positionSolIntermediaire[3] = { /*Valeur de test:*/ 278.0f-5 /*272.0f*/, 97.0f+5, 190.0f};
const static float positionSolGauche[3] = { /*Valeur de test:*/ 278.0f+10 /*272.0f*/, 97.0f+8, 184.0f};
const static float positionSolDroit[3] = { /*Valeur de test:*/ 283.45f /*272.0f*/, 106.57f, 179.69f};

#elif defined(SLAVE)

static float positionAccelerateur[3] = {186.0f, 55.0f, 141.0f};
static float positionStockage[3] = {174.50f, 255.64f, 80.87f};
static float positionIntermediaire[3] = {195.18f, 249.83f, 82.19f};
static float positionPreDistributeur[3] = {329.41f, 270.77f, 90.57f};
static float positionDistributeur[3] = {295.77f, 216.30f, 195.18f};

static float positionSolIntermediaire[3] = { 282.303986f, 180.0f, 220.0f};
static float positionSol[3] = {300.0, 160.0f, 220.0f};//{ 301.84f, 157.25f, 213.66f};
static float positionRecule[3] = {234.520004f, 80.695999f, 113.959999f};

const static float cst=30;  // constante pour passer de XL (home à 180) à AX (home à 150)

static float positionAccelerateurSecondaire[3] = {213.4f+cst, 360-(180.0f+cst),151.4f+cst};
static float positionAccelerateurSecondaire2[3]= {291.368011f, 245.080002f, 219.912003f};//{259.600006f,168.608002f,177.231995f};
static float positionStockageSecondaire[3] = { 140.00f+cst, 360-(75.00f+cst), 55.00f+cst};
static float positionIntermediaireSecondaire[3] = {150.00f+cst, 360-(80.00f+cst), 60.00f+cst};
static float positionIntermediaireSecondaire2[3] = {249.00f+cst, 360-(70.00f+cst), 55.00f+cst};
static float positionDistributeurSecondaire[3] = {269.0f, 150.0f, 150.0f};
static float positionDistributeurSecondairePreRecule[3] = {245.0f, 100.935997f, 125.0f};
static float positionLetOust[3] = {210.0f, 240.0f, 85.0f};

static float positionBalance[3]={160.0f+cst, 360-(210.0f+cst), 160.0f+cst};
static float positionPreBalance[3]={237.160004f, 260.127991f, 94.776001f};
static float positionPreBalance2[3]={249.479996f, 187.968002f, 107.624001f};
static float positionMusclor[3]={285.648010f, 278.695984f, 98.648003f};//{250.0f+cst, 360-(60.0f+cst), 150.0f+cst};
static float positionMusclor2[3]={288.639984f, 277.992004f, 84.304001f};//{250.0f+cst, 360-(60.0f+cst), 150.0f+cst};
static float positionFinMusclor[3]={253.352005f, 279.488007f, 91.695999f};//{250.0f+cst, 360-(60.0f+cst), 150.0f+cst};
static float positionMusclor3[3]={269.544006f, 250.184006f, 121.264000f};//{203.279999f, 109.384003f, 82.720001f};//{250.0f+cst, 360-(47.0f+cst), 140.0f+cst};
static float positionMusclor4[3]={235.0f+cst, 360-(47.0f+cst), 140.0f+cst};
static float positionMusclor5[3]={250.0f+cst, 360-(47.0f+cst), 90.0f+cst};
static float positionGoldonium[3]={285.648010f, 278.695984f, 269.928009f};//{212,89,147};
static float positionPostGoldonium[3]={279.648010f, 265.695984f, 260.0f};//{212,89,147};
static float positionPreGoldonium[3]={271.303986f, 281.687988f, 279.664001f};//{291.015991f, 278.432007f, 86.503998f};//{180.0f+cst, 360-(230.0f+cst), 130.0f+cst};
static float positionPostPostGoldonium[3]={275.088013f, 248.687988f, 235.744003f};//{291.015991f, 278.432007f, 86.503998f};//{180.0f+cst, 360-(230.0f+cst), 130.0f+cst};

static float positionLetRedBeGood[3]={190.0f, 220.0f, 121.0f};//{291.015991f, 278.432007f, 86.503998f};//{180.0f+cst, 360-(230.0f+cst), 130.0f+cst};



static float positionAccelerateurBleu[3] ={281.07f,228.88f,220.09f};
static float positionPreAccelerateurDepotPalet[3] ={280.0f,233.0f,216.0f};
static float positionAccelerateurDepotPalet[3] ={284.33f,243.14f,231.53f};
static float positionPostAccelerateurDepotPalet[3] ={295.0f,260.0f,231.53f};

#endif

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

#if defined(MAIN)

const static float* stepsFromStorageToDistrib[] = {
        positionStockagePrise, positionIntermediaire, positionPreDistributeur, positionDistributeur
};
const static float* stepsFromDistribToStorage[] = {
        positionDistributeur, positionPreDistributeur, positionIntermediaire, positionStockageDepot
};

#elif defined(SLAVE)

const static float* stepsFromStorageToDistrib[] = {
        positionStockage, positionIntermediaire, positionPreDistributeur, positionDistributeur
};
const static float* stepsFromDistribToStorage[] = {
        positionDistributeur, positionPreDistributeur, positionIntermediaire, positionStockage
};

#endif

#endif //LL_ACTUTORVALUES_H
