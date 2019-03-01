#ifndef _PIN_MAPPING_h
#define _PIN_MAPPING_h

/* Ethernet */
constexpr uint8_t CS = 9;
constexpr uint8_t RST = 29;
constexpr uint8_t LED_DEBUG_ETH = 2;

/* Moteurs */
constexpr uint8_t INA_LEFT = 3;
constexpr uint8_t INB_LEFT = 4;
constexpr uint8_t PIN_PWM_LEFT = 35;

constexpr uint8_t INA_RIGHT = 25;
constexpr uint8_t INB_RIGHT = 32;
constexpr uint8_t PIN_PWM_RIGHT = 8;



/* CAPTEURS */
constexpr uint8_t PIN_JMPR = 99;
constexpr uint8_t PIN_CONT1 = 99;
constexpr uint8_t SICK_PINS[NBR_OF_DISTANCE_SENSOR] = {
        A22, // 1er
        A22, // 2e
        A22, // 3e
        A22, // 4e
        A22, // 5e
        A22, // 6e
};

// TechTheTachyon
/* Ascenceurs */
constexpr uint8_t STEP_PIN_RIGHT = 99; //vitesse
constexpr uint8_t RST_PIN_RIGHT = 99; //reset
constexpr uint8_t DIR_PIN_RIGHT = 99; //direction

constexpr uint8_t STEP_PIN_LEFT = 99; //vitesse
constexpr uint8_t DIR_PIN_LEFT = 99; //direction
constexpr uint8_t RST_PIN_LEFT = 99; //reset

constexpr uint8_t RIGHT_VALVE_PIN = 99; // électrovanne droite
constexpr uint8_t LEFT_VALVE_PIN = 99;  // électrovanne gauche

constexpr uint8_t LEFT_PUMP_PIN = 99;
constexpr uint8_t RIGHT_PUMP_PIN = 99;




#endif //_PIN_MAPPING_h
