#include "OrderManager.h"
#include "Actuators/ActuatorValues.h"

OrderManager::OrderManager():
        hookList(HookList()),
        orderData(OrderData()),
        sensorMgr(SensorMgr::Instance()),
        motionControlSystem(MCS::Instance()),
        actuatorsMgr(ActuatorsMgr::Instance()),
        highLevel(ComMgr::Instance())
{
    memset(readMessage, 0, RX_BUFFER_SIZE);
    isSendingUS = true;
    hooksEnabled = true;
    HLWaiting = false;
    highLevel.printfln(DEBUG_HEADER,"Communications ready");
}


void OrderManager::communicate() {
    if (highLevel.read(readMessage)) {
        // TODO: debug only
        highLevel.printfln(DEBUG_HEADER, "(%lli) Received '''%s'''\n", messageCount, readMessage);
        messageCount++;
        execute(readMessage);
    }

    memset(readMessage, 0, RX_BUFFER_SIZE);

    static Metro checkMovement = Metro(10);
    static Metro checkArms = Metro(70);
    static Metro rebootXLs = Metro(100);
    static Metro checkHooksTimer = Metro(20);
    static Metro sendPos = Metro(50);

    if (checkMovement.check())
    {
        /* FIXME if (!motionControlSystem.sentMoveAbnormal() && motionControlSystem.isMoveAbnormal()) {//Si on est bloqué et qu'on n'a pas encore prévenu
            motionControlSystem.setMoveAbnormalSent(true);
            highLevel.sendEvent("unableToMove p");
        }
        else if (motionControlSystem.sentMoveAbnormal() && !motionControlSystem.isMoveAbnormal()) {//Si on est plus bloqué et qu'on avait prévenu
            motionControlSystem.setMoveAbnormalSent(false);
        }*/
    }
    
    if(rebootXLs.check()) {
        actuatorsMgr.rebootArms();
    }

    if(checkArms.check())
    {
        actuatorsMgr.checkArmMovements();
    }

    if (checkHooksTimer.check() && hooksEnabled)
    {
        checkHooks();
        executeHooks();
    }

    if (sendPos.check()) {
        motionControlSystem.sendPositionUpdate();
    }
 }

 void OrderManager::execute(const char* orderToExecute)
 {
     char str_order[RX_BUFFER_SIZE];
     char orderBuffer[RX_BUFFER_SIZE];
     bool requiresConfirmation = orderToExecute[0] == '!';
     if(requiresConfirmation) {
         highLevel.printfln(DEBUG_HEADER, "Confirmation requested for %s", orderToExecute);
         strcpy(orderBuffer, &orderToExecute[1]);
     } else {
         strcpy(orderBuffer, orderToExecute);
     }

     int8_t n_param = split(orderBuffer, orderData,
                            SEPARATOR);        //Sépare l'ordre en plusieurs mots, n_param=nombre de paramètres

     if (n_param >= 0) {
         strcpy(str_order, orderData.at(0));

         //Serial.println(orderToExecute);

         auto it = orders.find(str_order);
         if(it != orders.end()) {
             highLevel.printfln(DEBUG_HEADER, "Received order str: '%s'", orderBuffer);
             it->second->exec(orderData);
             if(requiresConfirmation){
                 highLevel.printfln(DEBUG_HEADER, "Sending confirmation for '%s'", orderBuffer);
                 highLevel.printfln(EVENT_HEADER, "confirmOrder %s", orderBuffer);
             }
         }
         else
         {
             highLevel.printfln(STD_HEADER,"wat. '%s'", orderToExecute);
             highLevel.printfln(DEBUG_HEADER,"T'es un déchêt");
         }
     }
     checkHooks();
 }

 /**
 *	Sépare une courte chaîne de caractères(RX_BUFFER_SIZE) selon un séparateur, dans un tableau output (au plus 4 mots)
 */

int8_t OrderManager::split(char* input, OrderData& output, const char* separator) {
    char *token;
    int8_t i = -1;
    output.clear();
    token = strtok(input, separator);
    if (token != nullptr) {
        output.push_back(token);
        i++;
    }
    do {
        token = strtok(nullptr, separator);
        if (token != nullptr) {
            output.push_back(token);
            ++i;
        }
    } while (token != nullptr && i < RX_WORD_COUNT);

    return i;
}

int OrderManager::parseInt(const char* s) {
    return strtol(s, nullptr, DEC);
}

float OrderManager::parseFloat(const char* s) {
    return strtof(s, nullptr);
}

bool OrderManager::isHLWaiting() {
    return(HLWaiting);
}

void OrderManager::checkJumper() {
    if(sensorMgr.isReadyToGo() && HLWaiting)
    {
        highLevel.startMatch();
        HLWaiting = false;
    }
}

void OrderManager::checkHooks() {
    hookList.check(motionControlSystem.getX(), motionControlSystem.getY(),motionControlSystem.getAngle());
}

void OrderManager::executeHooks() {
    std::vector<String> orders = hookList.executeHooks();

    for(String &order : orders)
    {
        execute(order.c_str());
    }
}

void OrderManager::registerOrder(String id, AbstractOrder *order) {
    orders.insert( { id, order });
}

void OrderManager::init() {
    orders = allOrders;
}
