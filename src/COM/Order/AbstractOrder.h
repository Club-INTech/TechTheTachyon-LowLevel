//
// Created by asphox on 29/04/18.
//

#ifndef TECHTHETOWN_LOWLEVEL_ABSTRACTORDER_H
#define TECHTHETOWN_LOWLEVEL_ABSTRACTORDER_H

#include <Arduino.h>
#include "OrderData.h"
#include "Config/defines.h"
#include "Actuators/ActuatorValues.h"
#include "Utils/Singleton.hpp"
#include <map>

typedef OrderData& Args;

class OrderManager;

static uint32_t actuatorOrderIndex;

class AbstractOrder
{
public:
    explicit AbstractOrder(uint8_t nbr_args = 0);
    virtual ~AbstractOrder() = default;
    bool exec(Args args);
    virtual void impl(Args args) = 0;

    static uint32_t nextActuatorOrderIndex();


    inline bool operator()(Args args){ return exec(args); }

protected:
    uint8_t                 nbr_args;
    OrderManager&           orderManager;
};

#define ORDER(name,nbrparam)                                                        \
static struct ORDER_##name : public AbstractOrder, public Singleton<ORDER_##name>   \
{                                                                                   \
    ORDER_##name() : AbstractOrder()                                                \
    {                                                                               \
        this->nbr_args = nbrparam;                                                  \
        allOrders.insert({#name, this});                                            \
    }                                                                               \
    void impl(Args);                                                                \
} __ORDER_##name;


static std::map<String, AbstractOrder*> allOrders;

#endif //TECHTHETOWN_LOWLEVEL_ABSTRACTORDER_H
