//
// Created by asphox on 29/04/18.
//


#include "Orders.h"
#include "../../MCS/RobotStatus.h"
#include "../../Actuators/ActuatorValues.h"
#include "../../Utils/pin_mapping.h"
#include "../../Actuators/ActuatorsMgr.h"

void ORDER_ping::impl(Args args)
{
    orderManager.highLevel.printfln(EVENT_HEADER,"pong");
}

void ORDER_j::impl(Args args)
{
    orderManager.HLWaiting = true;
}

void ORDER_f::impl(Args args)
{
    /* FIXME orderManager.highLevel.printfln(STD_HEADER,"%d",orderManager.motionControlSystem.isMoving());
    orderManager.highLevel.printfln(STD_HEADER,"%d",orderManager.motionControlSystem.isMoveAbnormal());*/
}

void ORDER_xyo::impl(Args args)
{
    orderManager.highLevel.printfln(STD_HEADER,"%i",orderManager.motionControlSystem.getX());
    orderManager.highLevel.printfln(STD_HEADER,"%i",orderManager.motionControlSystem.getY());
    orderManager.highLevel.printfln(STD_HEADER,"%f",orderManager.motionControlSystem.getAngle());
}

void ORDER_d::impl(Args args)
{
    int16_t deplacement = strtod(args[0], nullptr);
    bool expectedWallImpact = false;
    if(args.nbrParams() == 2) {
        expectedWallImpact = ! strcmp(args[1], "true");
    }
    orderManager.highLevel.printfln(DEBUG_HEADER,"distance : %d %i",deplacement, expectedWallImpact);
    orderManager.motionControlSystem.disableP2P();

    orderManager.motionControlSystem.translate(deplacement);
}

void ORDER_t::impl(Args args)
{
    float angle;
    if (!strcmp(args[0], "pi")) {
        angle = (float) PI;
    } else {
        angle = strtof(args[0], nullptr);
    }
    orderManager.highLevel.printfln(DEBUG_HEADER,"angle : %f", angle);

    orderManager.motionControlSystem.disableP2P();
    orderManager.motionControlSystem.rotate(angle);
}

void ORDER_goto::impl(Args args)
{
    float targetX = strtof(args[0],nullptr);
    float targetY = strtof(args[1],nullptr);
    bool isSequential = false;

    if(args.nbrParams() == 3)
    {
        isSequential = !strcmp(args[2],"true") || !strcmp(args[2],"1");
        Serial.print("On séquentialise : ");
        Serial.println(isSequential);
    }
//                if(-1500 <= targetX && targetX <= 1500 && 0 <= targetY && targetY <= 2000)
//                {
    orderManager.motionControlSystem.gotoPoint2(targetX,targetY);
    orderManager.highLevel.printfln(DEBUG_HEADER, "goto %f %f %b", targetX, targetY, isSequential);
//                }
//                else
//      {
//                highLevel.log("ERREUR::Paramètres incorrects");
//      }
}

void ORDER_followTrajectory::impl(Args args)
{
    if(strtof(args[0], nullptr) == 0)
    {
        // FIXME orderManager.motionControlSystem.followTrajectory(trajectory_S_path[0],trajectory_S_path[1],trajectory_S_size);
    }
    else
    {
        orderManager.highLevel.printfln(DEBUG_HEADER,"ERREUR::Paramètres incorrects");
    }
}

void ORDER_stop::impl(Args args)
{
    orderManager.motionControlSystem.stop();
    orderManager.highLevel.printfln(DEBUG_HEADER,"A priori, je m'arrête");
}

void ORDER_cx::impl(Args args)
{
    orderManager.motionControlSystem.setX(orderManager.parseFloat(args[0]));
}

void ORDER_cy::impl(Args args)
{
    orderManager.motionControlSystem.setY(orderManager.parseFloat(args[0]));
}

void ORDER_co::impl(Args args)
{
    orderManager.motionControlSystem.setAngle(orderManager.parseFloat(args[0]));
}

void ORDER_cxyo::impl(Args args)
{
    orderManager.motionControlSystem.setX(orderManager.parseFloat(args[0]));
    orderManager.motionControlSystem.setY(orderManager.parseFloat(args[1]));
    orderManager.motionControlSystem.setAngle(orderManager.parseFloat(args[2]));

    // Mise à jour de l'offset et du target des codeuses. Faut pas tourner parce que le HL te dit où t'es. Je sais il est pas gentil mais faut l'accepter
    orderManager.motionControlSystem.setAngleOffset(orderManager.parseFloat(args[2]));
    orderManager.motionControlSystem.resetEncoders();
    //orderManager.motionControlSystem.stop();
    orderManager.highLevel.printfln(DEBUG_HEADER, "X,Y,O set to %d %d %f", orderManager.motionControlSystem.getX(),orderManager.motionControlSystem.getY(), orderManager.motionControlSystem.getAngle());
}

void ORDER_ctv::impl(Args args)
{
    orderManager.motionControlSystem.setTranslationSpeed(orderManager.parseFloat(args[0]));
}

void ORDER_crv::impl(Args args)
{
    orderManager.motionControlSystem.setRotationSpeed(orderManager.parseFloat(args[0]));
}

void ORDER_ctrv::impl(Args args)
{
    orderManager.motionControlSystem.setTranslationSpeed(orderManager.parseFloat(args[0]));
    orderManager.motionControlSystem.setRotationSpeed(orderManager.parseFloat(args[1]));
}

void ORDER_efm::impl(Args args)
{
    orderManager.motionControlSystem.setForcedMovement(true);
}

void ORDER_dfm::impl(Args args)
{
    orderManager.motionControlSystem.setForcedMovement(false);
}

void ORDER_ct0::impl(Args args)
{
    orderManager.motionControlSystem.controlledTranslation(false);
    orderManager.highLevel.printfln(DEBUG_HEADER,"Non asservi en translation");
}

void ORDER_ct1::impl(Args args)
{
    orderManager.motionControlSystem.controlledTranslation(true);
    orderManager.highLevel.printfln(DEBUG_HEADER,"Asservi en translation");
}

void ORDER_cr0::impl(Args args)
{
    orderManager.motionControlSystem.controlledRotation(false);
    orderManager.highLevel.printfln(DEBUG_HEADER,"Non asservi en rotation");
}

void ORDER_cr1::impl(Args args)
{
    orderManager.motionControlSystem.controlledRotation(true);
    orderManager.highLevel.printfln(DEBUG_HEADER,"Asservi en rotation");
}

void ORDER_cv0::impl(Args args)
{
    orderManager.motionControlSystem.setControl(false);
    orderManager.highLevel.printfln(DEBUG_HEADER,"Non asservi en vitesse");
}

void ORDER_cv1::impl(Args args)
{
    orderManager.motionControlSystem.setControl(true);
    orderManager.highLevel.printfln(DEBUG_HEADER,"asservi en vitesse");
}

void ORDER_cod::impl(Args args)
{
    orderManager.highLevel.printfln(DEBUG_HEADER,"Gauche:");
    orderManager.highLevel.printfln(DEBUG_HEADER,"%ld", orderManager.motionControlSystem.getLeftTicks());
    orderManager.highLevel.printfln(DEBUG_HEADER,"Droite:");
    orderManager.highLevel.printfln(DEBUG_HEADER,"%ld", orderManager.motionControlSystem.getRightTicks());
}

void ORDER_pfdebug::impl(Args args)
{
    //highLevel.printfln(STD_HEADER,"%d", (int)motionControlSystem.getRightSpeed());
    //highLevel.printfln(STD_HEADER,"%d", (int)motionControlSystem.getRightMotorDir());
    //highLevel.printfln(STD_HEADER,"%d", (int)motionControlSystem.getRightSetPoint());
    //highLevel.printfln(STD_HEADER,"%d", (int)motionControlSystem.getRightMotorPWM());
    //highLevel.printfln(STD_HEADER,"%d", (int)motionControlSystem.getCodD());
}

void ORDER_rawpwm::impl(Args args)
{
    //uint8_t rawpwm = orderManager.parseInt(args[0]);
    // FIXME   orderManager.motionControlSystem.orderRawPwm(Side::LEFT, rawpwm);
    // FIXME  orderManager.motionControlSystem.orderRawPwm(Side::RIGHT, rawpwm);
}

void ORDER_getpwm::impl(Args args)
{
    //int32_t left, right;
    // FIXME   orderManager.motionControlSystem.getPWMS(left, right);
    // FIXME  orderManager.highLevel.printfln(DEBUG_HEADER,"PWMS: %ld - %ld", left, right);
}

void ORDER_errors::impl(Args args)
{
    //float leftProp, leftDer, leftInt, rightProp, rightDer, rightInt;
    // FIXME  orderManager.motionControlSystem.getSpeedErrors(leftProp, leftInt, leftDer, rightProp, rightInt, rightDer);
    // FIXME  orderManager.highLevel.printfln(DEBUG_HEADER,"Prop: %f - %f", leftProp, rightProp);
    // FIXME orderManager.highLevel.printfln(DEBUG_HEADER,"Deriv: %f - %f", leftDer, rightDer);
    // FIXME orderManager.highLevel.printfln(DEBUG_HEADER,"Integ: %f - %f", leftInt, rightInt);
}

void ORDER_rawspeed::impl(Args args)
{
    // int32_t leftsetpoint, rightsetpoint;
    // FIXME motionControlSystem.rawWheelSpeed(parseInt(orderData.at(1)), leftsetpoint, rightsetpoint);
    // FIXME orderManager.highLevel.printfln(DEBUG_HEADER,"Speed set");
    // FIXME orderManager.motionControlSystem.getSpeedSetpoints(leftsetpoint, rightsetpoint);
    // FIXME orderManager.highLevel.printfln(DEBUG_HEADER,"speed setpoints: %ld - %ld", leftsetpoint, rightsetpoint);
}

void ORDER_rawposdata::impl(Args args)
{
    int32_t leftSpeedGoal, rightSpeedGoal;
    orderManager.motionControlSystem.getSpeedGoals(leftSpeedGoal, rightSpeedGoal);

    int16_t xPos = orderManager.motionControlSystem.getX();
    int16_t yPos = orderManager.motionControlSystem.getY();
    float angle = orderManager.motionControlSystem.getAngle();
    float leftSpeed = orderManager.motionControlSystem.getLeftSpeed();
    float rightSpeed = orderManager.motionControlSystem.getRightSpeed();

    /*
    orderManager.highLevel.printfln(DEBUG_HEADER,"%d,%d,%f,%f,%d,%f,%d",
                                    xPos,yPos,angle,leftSpeed, leftSpeedGoal,rightSpeed,rightSpeedGoal);
    */
    Serial.printf("%d,%d,%f,%f,%d,%f,%d\n", xPos,yPos,angle,leftSpeed, leftSpeedGoal,rightSpeed,rightSpeedGoal);
    //int32_t right, left;
    //motionControlSystem.getPWMS(left,right);
    //Serial.println(right);
    //float rotaProp, rotaDer, rotaInt;
    //motionControlSystem.getRotationErrors(rotaProp, rotaInt, rotaDer);
    //Serial.println(rotaInt);
}

void ORDER_reseteth::impl(Args args)
{
    orderManager.highLevel.resetEth();
}

void ORDER_montlhery::impl(Args args)
{
    orderManager.motionControlSystem.controlledRotation(false);
    orderManager.motionControlSystem.controlledTranslation(false);
    orderManager.motionControlSystem.setForcedMovement(true);
    orderManager.highLevel.printfln(DEBUG_HEADER, "monthlery received");
}

void ORDER_maxtr::impl(Args args) {
    orderManager.motionControlSystem.setMaxTranslationSpeed(orderManager.parseFloat(args[0]));
}

void ORDER_maxro::impl(Args args) {
    orderManager.motionControlSystem.setMaxRotationSpeed(orderManager.parseFloat(args[0]));
}

void ORDER_av::impl(Args args)
{
    orderManager.motionControlSystem.speedBasedMovement(MOVEMENT::FORWARD);
    orderManager.highLevel.printfln(DEBUG_HEADER, "av received");
}

void ORDER_rc::impl(Args args)
{
    orderManager.motionControlSystem.speedBasedMovement(MOVEMENT::BACKWARD);
    orderManager.highLevel.printfln(DEBUG_HEADER, "rc received");
}

void ORDER_td::impl(Args args)
{
    orderManager.motionControlSystem.speedBasedMovement(MOVEMENT::ANTITRIGO);
    orderManager.highLevel.printfln(DEBUG_HEADER, "td received");
}

void ORDER_tg::impl(Args args)
{
    orderManager.motionControlSystem.speedBasedMovement(MOVEMENT::TRIGO);
    orderManager.highLevel.printfln(DEBUG_HEADER, "tg received");
}

void ORDER_sstop::impl(Args args)
{
    orderManager.motionControlSystem.speedBasedMovement(MOVEMENT::NONE);
    orderManager.highLevel.printfln(DEBUG_HEADER, "sstop received");
}

void ORDER_toggle::impl(Args args)
{
   /* FIXME orderManager.motionControlSystem.translation = !orderManager.motionControlSystem.translation;   //Bascule entre le réglage d'asserv en translation et en rotation
    if (orderManager.motionControlSystem.translation) {
        orderManager.highLevel.printfln(DEBUG_HEADER, "reglage de la translation");
    } else
        orderManager.highLevel.printfln(DEBUG_HEADER, "reglage de la rotation");
*/
}

void ORDER_displayAsserv::impl(Args args)
{
    /*
    float
            kp_t, ki_t, kd_t,      // Translation
            kp_r, ki_r, kd_r,      // Rotation
            kp_g, ki_g, kd_g,      // Vitesse gauche
            kp_d, ki_d, kd_d;      // Vitesse droite
            */
 /* FIXME   orderManager.motionControlSystem.getTranslationTunings(kp_t, ki_t, kd_t);
    orderManager.motionControlSystem.getRotationTunings(kp_r, ki_r, kd_r);
    orderManager.motionControlSystem.getLeftSpeedTunings(kp_g, ki_g, kd_g);
    orderManager.motionControlSystem.getRightSpeedTunings(kp_d, ki_d, kd_d);
    orderManager.highLevel.printfln(DEBUG_HEADER,"trans : kp= %g ; ki= %g ; kd= %g", kp_t, ki_t, kd_t);
    orderManager.highLevel.printfln(DEBUG_HEADER,"rot   : kp= %g ; ki= %g ; kd= %g", kp_r, ki_r, kd_r);
    orderManager.highLevel.printfln(DEBUG_HEADER,"gauche: kp= %g ; ki= %g ; kd= %g", kp_g, ki_g, kd_g);
    orderManager.highLevel.printfln(DEBUG_HEADER,"droite: kp= %g ; ki= %g ; kd= %g", kp_d, ki_d, kd_d);
*/
}

void ORDER_kpt::impl(Args args)
{
 /* FIXME   float kp, ki, kd;
    orderManager.highLevel.printfln(STD_HEADER,"kp_trans ?");
    orderManager.motionControlSystem.getTranslationTunings(kp, ki, kd);
    kp = orderManager.parseFloat(args[0]);
    orderManager.motionControlSystem.setTranslationTunings(kp, ki, kd);
    orderManager.highLevel.printfln(DEBUG_HEADER,"kp_trans = %g", kp);
*/
}

void ORDER_kdt::impl(Args args)
{
   /* FIXME float kp, ki, kd;
    orderManager.highLevel.printfln(STD_HEADER,"kd_trans ?");
    orderManager.motionControlSystem.getTranslationTunings(kp, ki, kd);
    kd = orderManager.parseFloat(args[0]);
    orderManager.motionControlSystem.setTranslationTunings(kp, ki, kd);
    orderManager.highLevel.printfln(DEBUG_HEADER,"kd_trans = %g", kd);
*/
}

void ORDER_kit::impl(Args args)
{
/* FIXME    float kp, ki, kd;
    orderManager.highLevel.printfln(STD_HEADER,"ki_trans ?");
    orderManager.motionControlSystem.getTranslationTunings(kp, ki, kd);
    ki = orderManager.parseFloat(args[0]);
    orderManager.motionControlSystem.setTranslationTunings(kp, ki, kd);
    orderManager.highLevel.printfln(DEBUG_HEADER,"ki_trans = %g", ki);
*/
}

void ORDER_kpr::impl(Args args)
{
  /* FIXME  float kp, ki, kd;
    orderManager.highLevel.printfln(STD_HEADER,"kp_rot ?");
    orderManager.motionControlSystem.getRotationTunings(kp, ki, kd);
    kp = orderManager.parseFloat(args[0]);
    orderManager.motionControlSystem.setRotationTunings(kp, ki, kd);
    orderManager.highLevel.printfln(DEBUG_HEADER,"kp_rot = %g", kp);
*/
}

void ORDER_kir::impl(Args args)
{
/* FIXME    float kp, ki, kd;
    orderManager.highLevel.printfln(STD_HEADER,"ki_rot ?");
    orderManager.motionControlSystem.getRotationTunings(kp, ki, kd);
    ki = orderManager.parseFloat(args[0]);
    orderManager.motionControlSystem.setRotationTunings(kp, ki, kd);
    orderManager.highLevel.printfln(DEBUG_HEADER,"ki_rot = %g", ki);
*/}

void ORDER_kdr::impl(Args args)
{
/* FIXME    float kp, ki, kd;
    orderManager.highLevel.printfln(STD_HEADER,"kd_rot ?");
    orderManager.motionControlSystem.getRotationTunings(kp, ki, kd);
    kd = orderManager.parseFloat(args[0]);
    orderManager.motionControlSystem.setRotationTunings(kp, ki, kd);
    orderManager.highLevel.printfln(DEBUG_HEADER,"kd_rot = %g", kd);
*/
}

void ORDER_kpg::impl(Args args)
{
/* FIXME    float kp, ki, kd;
    orderManager.highLevel.printfln(STD_HEADER,"kp_gauche ?");
    orderManager.motionControlSystem.getLeftSpeedTunings(kp, ki, kd);
    kp = orderManager.parseFloat(args[0]);
    orderManager.motionControlSystem.setLeftSpeedTunings(kp, ki, kd);
    orderManager.highLevel.printfln(DEBUG_HEADER,"kp_gauche = %g", kp);
*/}

void ORDER_kig::impl(Args args)
{
  /* FIXME  float kp, ki, kd;
    orderManager.highLevel.printfln(STD_HEADER,"ki_gauche ?");
    orderManager.motionControlSystem.getLeftSpeedTunings(kp, ki, kd);
    ki = orderManager.parseFloat(args[0]);
    orderManager.motionControlSystem.setLeftSpeedTunings(kp, ki, kd);
    orderManager.highLevel.printfln(DEBUG_HEADER,"ki_gauche = %g", ki);
*/}

void ORDER_kdg::impl(Args args)
{
 /* FIXME   float kp, ki, kd;
    orderManager.highLevel.printfln(STD_HEADER,"kd_gauche ?");
    orderManager.motionControlSystem.getLeftSpeedTunings(kp, ki, kd);
    kd = orderManager.parseFloat(args[0]);
    orderManager.motionControlSystem.setLeftSpeedTunings(kp, ki, kd);
    orderManager.highLevel.printfln(DEBUG_HEADER,"kd_gauche = %g", kd);
*/}

void ORDER_kpd::impl(Args args)
{
/* FIXME    float kp, ki, kd;
    orderManager.highLevel.printfln(STD_HEADER,"kp_droite ?");
    orderManager.motionControlSystem.getRightSpeedTunings(kp, ki, kd);
    kp = orderManager.parseFloat(args[0]);
    orderManager.motionControlSystem.setRightSpeedTunings(kp, ki, kd);
    orderManager.highLevel.printfln(DEBUG_HEADER,"kp_droite = %g", kp);
*/
}

void ORDER_kid::impl(Args args)
{
  /* FIXME  float kp, ki, kd;
    orderManager.highLevel.printfln(STD_HEADER,"ki_droite ?");
    orderManager.motionControlSystem.getRightSpeedTunings(kp, ki, kd);
    ki = orderManager.parseFloat(args[0]);
    orderManager.motionControlSystem.setRightSpeedTunings(kp, ki, kd);
    orderManager.highLevel.printfln(DEBUG_HEADER,"ki_droite = %g", ki);
*/
}

void ORDER_kdd::impl(Args args)
{
/* FIXME    float kp, ki, kd;
    orderManager.highLevel.printfln(STD_HEADER,"kd_droite ?");
    orderManager.motionControlSystem.getRightSpeedTunings(kp, ki, kd);
    kd = orderManager.parseFloat(args[0]);
    orderManager.motionControlSystem.setRightSpeedTunings(kp, ki, kd);
    orderManager.highLevel.printfln(DEBUG_HEADER,"kd_droite = %g", kd);
*/
}


void ORDER_nh::impl(Args args)
{
    uint8_t id;
    int32_t x;
    uint32_t y, r;
    float angleHook, angleTolerance;
        id = (uint8_t) orderManager.parseInt(args[0]);
        x = (int32_t) orderManager.parseInt(args[1]);
        y = (uint32_t) orderManager.parseInt(args[2]);
        r = (uint32_t) orderManager.parseInt(args[3]);
        angleHook = orderManager.parseFloat(args[4]);
        angleTolerance = orderManager.parseFloat(args[5]);

        char hookOrder[RX_BUFFER_SIZE] = "";

        for (int i = 6; i < nbr_args; i++) {
            strcat(hookOrder, args[i]);
            strcat(hookOrder, " ");
        }
        hookOrder[RX_BUFFER_SIZE - 1] = '\0';

        orderManager.hookList.addHook(id, x, y, r, angleHook, angleTolerance, hookOrder);

        Serial.print("Ordre du hook: ");
        Serial.println(hookOrder);

        //TEST:
        Serial.println(orderManager.hookList.getHook(id).getOrder());
}

void ORDER_eh::impl(Args args)
{
    int hookId = orderManager.parseInt(args[0]);
    if(orderManager.hookList.hookWithId(hookId))
    {
        orderManager.hookList.enableHook((uint8_t)hookId); //Singe proof ?
    }
    else
    {
        orderManager.highLevel.printfln(DEBUG_HEADER,"ERREUR::Activation d'un hook inexistant");
    }
}

void ORDER_dh::impl(Args args) {
    int hookId = orderManager.parseInt(args[0]);

    if (orderManager.hookList.hookWithId(hookId)) {
        orderManager.hookList.disableHook((uint8_t) hookId); //Singe proof ?
    } else {
        orderManager.highLevel.printfln(DEBUG_HEADER, "ERREUR::Activation d'un hook inexistant");
    }
}

void ORDER_demo::impl(Args args) {

}

void ORDER_ptpdemo::impl(Args args)
{
    orderManager.execute("goto 500 -700");
    delay(5000);
    orderManager.execute("goto 1000 -400");
    delay(5000);
    orderManager.execute("goto 750 100");
    delay(5000);
    orderManager.execute("goto 0 0");
}


void ORDER_ptpdemoseq::impl(Args args)
{
    orderManager.execute("goto 500 -700 true");
    delay(5000);
    orderManager.execute("goto 1000 -400 true");
    delay(5000);
    orderManager.execute("goto 750 100 true");
    delay(5000);
    orderManager.execute("goto 0 0 true");
}


// TODO: pour les 2 qui suivent: électrovannes?
void ORDER_suck::impl(Args args)
{
    if (!strcmp(args[0], "right")) {
        digitalWrite(RIGHT_PUMP_PIN, HIGH);
    }

}

void ORDER_unsuck::impl(Args args)
{
    if (!strcmp(args[0], "right")) {
        digitalWrite(RIGHT_PUMP_PIN, LOW);
    }

}

void ORDER_up::impl(Args args)
{
    //int nbPas = 700;

    ActuatorsMgr::Instance().moveRightStepper(1);
    orderManager.highLevel.printf(DEBUG_HEADER, "Monte le stepper droit de 1 unité!\n");

}

void ORDER_down::impl(Args args)
{
    ActuatorsMgr::Instance().moveRightStepper(-1);
    orderManager.highLevel.printf(DEBUG_HEADER, "Descend le stepper droit de 1 unité!\n");

}

void ORDER_updown::impl(Args args)
{
    ActuatorsMgr::Instance().moveRightStepper(1,-1);
    orderManager.highLevel.printf(DEBUG_HEADER, "Monte puis descend le stepper droit d'une unité\n");
}

void ORDER_downup::impl(Args args)
{
    ActuatorsMgr::Instance().moveRightStepper(-1,1);
    orderManager.highLevel.printf(DEBUG_HEADER, "Descend puis monte le stepper droit d'une unité\n");
}

void ORDER_oust::impl(Args args)
{
    ActuatorsMgr& manager = ActuatorsMgr::Instance();
    XL430* mot = manager.motor4;
    mot->setGoalAngle(270);
}

void ORDER_range::impl(Args args)
{
    ActuatorsMgr& manager = ActuatorsMgr::Instance();
    XL430* mot = manager.motor4;
    mot->setGoalAngle(180);
}

void ORDER_dist::impl(Args args)
{
    ActuatorsMgr& manager = ActuatorsMgr::Instance();
    if(strcmp(args[0], "right") != 0) {
        ComMgr::Instance().printfln(DEBUG_HEADER, "'y a pas de bras gauche!");
    }
    Arm<XL430>* arm = manager.rightArm;
    arm->setPosition(positionDistributeur);

}void ORDER_distSecondaire::impl(Args args)
{
    ActuatorsMgr& manager = ActuatorsMgr::Instance();
    if(strcmp(args[0], "right") != 0) {
        ComMgr::Instance().printfln(DEBUG_HEADER, "'y a pas de bras gauche!");
    }
    Arm<XL430>* arm = manager.rightArm;
    arm->setPosition(positionIntermediaireSecondaire);
    arm->setPosition(positionIntermediaireSecondaire2);
    arm->setPosition(positionDistributeurSecondairePreRecule);
    arm->setPosition(positionDistributeurSecondaire);
}

void ORDER_grnd::impl(Args args)
{
    ActuatorsMgr& manager = ActuatorsMgr::Instance();
    if(strcmp(args[0], "right") != 0) {
        ComMgr::Instance().printfln(DEBUG_HEADER, "'y a pas de bras gauche!");
    }
    Arm<XL430>* arm = manager.rightArm;
    arm->setPosition(positionIntermediaire);
    arm->setPosition(positionSolIntermediaire);
    arm->setPosition(positionSol);
}

void ORDER_stock::impl(Args args)
{
    ActuatorsMgr& manager = ActuatorsMgr::Instance();
    if(strcmp(args[0], "right") != 0) {
        ComMgr::Instance().printfln(DEBUG_HEADER, "'y a pas de bras gauche!");
    }
    Arm<XL430>* arm = manager.rightArm;
    arm->setPosition(positionIntermediaire);
    arm->setPosition(positionStockage);
}

void ORDER_stockSecondaire::impl(Args args)
{
    ActuatorsMgr& manager = ActuatorsMgr::Instance();
    if(strcmp(args[0], "right") != 0) {
        ComMgr::Instance().printfln(DEBUG_HEADER, "'y a pas de bras gauche!");
    }
    Arm<XL430>* arm = manager.rightArm;
    arm->setPosition(positionIntermediaireSecondaire2);
    arm->setPosition(positionIntermediaireSecondaire);
    arm->setPosition(positionStockageSecondaire);
}

void ORDER_acc::impl(Args args)
{
    ActuatorsMgr& manager = ActuatorsMgr::Instance();
    if(strcmp(args[0], "right") != 0) {
        ComMgr::Instance().printfln(DEBUG_HEADER, "'y a pas de bras gauche!");
    }
    Arm<XL430>* arm = manager.rightArm;
    arm->setPosition(positionAccelerateur);
}

void ORDER_accSecondaire::impl(Args args)
{
    ActuatorsMgr& manager = ActuatorsMgr::Instance();
    if(strcmp(args[0], "right") != 0) {
        ComMgr::Instance().printfln(DEBUG_HEADER, "'y a pas de bras gauche!");
    }
    Arm<XL430>* arm = manager.rightArm;
    //arm->setPosition(positionIntermediaireSecondaire);
    arm->setPosition(positionIntermediaireSecondaire2);
    arm->setPosition(positionAccelerateurSecondaire);
    //arm->setPosition(positionAccelerateurSecondaire2);
}

void ORDER_accSecondaire2::impl(Args args)
{
    ActuatorsMgr& manager = ActuatorsMgr::Instance();
    if(strcmp(args[0], "right") != 0) {
        ComMgr::Instance().printfln(DEBUG_HEADER, "'y a pas de bras gauche!");
    }
    Arm<XL430>* arm = manager.rightArm;
    arm->setPosition(positionAccelerateurSecondaire);
    //arm->setPosition(positionAccelerateurSecondaire2);
}

void ORDER_bal::impl(Args args)
{
    ActuatorsMgr& manager = ActuatorsMgr::Instance();
    if(strcmp(args[0], "right") != 0) {
        ComMgr::Instance().printfln(DEBUG_HEADER, "'y a pas de bras gauche!");
    }
    Arm<XL430>* arm = manager.rightArm;
    arm->setPosition(positionBalance);
}


void ORDER_gold::impl(Args args)
{
    ActuatorsMgr& manager = ActuatorsMgr::Instance();
    if(strcmp(args[0], "right") != 0) {
        ComMgr::Instance().printfln(DEBUG_HEADER, "'y a pas de bras gauche!");
    }
    Arm<XL430>* arm = manager.rightArm;
    arm->setPositionNoRetry(positionPreGoldonium);
    arm->setPositionNoRetry(positionGoldonium);
    arm->setPositionNoRetry(positionPostGoldonium);

}


void ORDER_musclorRed::impl(Args args)
{
    ActuatorsMgr& manager = ActuatorsMgr::Instance();
    Arm<XL430>* arm = manager.rightArm;
    arm->setPositionNoRetry(positionMusclor);
    arm->setPositionNoRetry(positionMusclor2);
}

void ORDER_musclor::impl(Args args)
{
    ActuatorsMgr& manager = ActuatorsMgr::Instance();
    Arm<XL430>* arm = manager.rightArm;
    arm->setPositionNoRetry(positionMusclor);
    arm->setPositionNoRetry(positionMusclor2);
    arm->setPositionNoRetry(positionFinMusclor);
}

void ORDER_goldDepot::impl(Args args)
{
    ActuatorsMgr& manager = ActuatorsMgr::Instance();
    if(strcmp(args[0], "right") != 0) {
        ComMgr::Instance().printfln(DEBUG_HEADER, "'y a pas de bras gauche!");
    }
    // TODO
    Arm<XL430>* arm = manager.rightArm;
/*    arm->setPosition(positionMusclor4);
    arm->setPosition(positionMusclor3);
    //arm->setPosition(positionMusclor2);
    arm->setPosition(positionMusclor);
    arm->setPosition(positionGoldonium);
*/
    arm->setPositionNoRetry(positionPreGoldonium);

}

void ORDER_brasToutDroit::impl(Args args)
{
    ActuatorsMgr& manager = ActuatorsMgr::Instance();
    if(strcmp(args[0], "right") != 0) {
        ComMgr::Instance().printfln(DEBUG_HEADER, "'y a pas de bras gauche!");
    }
    Arm<XL430>* arm = manager.rightArm;
    arm->setPosition(positionDroit);
}

void ORDER_brasRecule::impl(Args args)
{
    ActuatorsMgr& manager = ActuatorsMgr::Instance();
    if(strcmp(args[0], "right") != 0) {
        ComMgr::Instance().printfln(DEBUG_HEADER, "'y a pas de bras gauche!");
    }
    Arm<XL430>* arm = manager.rightArm;
    arm->setPosition(positionRecule);
}

void ORDER_XLm::impl(Args args)
{
    ActuatorsMgr& manager = ActuatorsMgr::Instance();
    XL430* motor = (XL430*)manager.dynamixelManager->getMotor(orderManager.parseInt(args[0]));
    motor->setGoalAngle(orderManager.parseFloat(args[1]));
}

void ORDER_XLs::impl(Args args)
{
    ActuatorsMgr& manager = ActuatorsMgr::Instance();
    XL430* motor = (XL430*)manager.dynamixelManager->getMotor(orderManager.parseInt(args[0]));
    motor->setGoalVelocity(orderManager.parseFloat(args[1]));
}

void ORDER_posBras::impl(Args args) {
    ActuatorsMgr& manager = ActuatorsMgr::Instance();
    float angles[3];
    MOVE_ARM(args[0],
             arm->fetchAngles(angles);
    )
    orderManager.highLevel.printfln(DEBUG_HEADER, "Angles are %f ; %f ; %f", angles[0], angles[1], angles[2]);
}

void ORDER_valveon::impl(Args args)
{
    if (!strcmp(args[0], "right")) {
        digitalWrite(RIGHT_VALVE_PIN, HIGH);
    }
    else {
        digitalWrite(LEFT_VALVE_PIN, HIGH);
    }
}


void ORDER_valveoff::impl(Args args)
{
    if (!strcmp(args[0], "right")) {
        digitalWrite(RIGHT_VALVE_PIN, LOW);
    }
    else {
        digitalWrite(LEFT_VALVE_PIN, LOW);
    }
}

void ORDER_elec::impl(Args args) {
    // TODO
}

void ORDER_rangeSICK::impl(Args args) {
    uint8_t index = (uint8_t) strtol(args[0], nullptr, DEC);
    uint16_t min = (uint16_t) strtol(args[1], nullptr, DEC);
    uint16_t max = (uint16_t) strtol(args[2], nullptr, DEC);
    if(index < NBR_OF_DISTANCE_SENSOR) {
        SensorMgr::Instance().getDistanceSensor(index).setRange(min, max);
    } else {
        orderManager.highLevel.printf(DEBUG_HEADER, "Aucun SICK à l'indice %i!\n", index);
    }
    orderManager.highLevel.printf(DEBUG_HEADER, "Le SICK %i est maintenant dans l'intervalle [%i; %i]\n", index, min, max);
}

void ORDER_testSICK::impl(Args args) {
    if(args.size() > 0) {
        uint8_t index = (uint8_t) orderManager.parseInt(args[0]);
        if(index < NBR_OF_DISTANCE_SENSOR) {
            orderManager.highLevel.printf(SICK_HEADER, "%i\n", SensorMgr::Instance().getDistanceSensor(index).readDistance());
        } else {
            orderManager.highLevel.printf(DEBUG_HEADER, "Aucun SICK à l'indice %i!\n", index);
        }
    }
}

void ORDER_lectureSICK::impl(Args args) {
    SensorMgr mgr = SensorMgr::Instance();
    if(NBR_OF_DISTANCE_SENSOR == 3) {
        orderManager.highLevel.printfln(SICK_HEADER, "%d %d %d",
                                        mgr.getDistanceSensor(0).readDistance(),
                                        mgr.getDistanceSensor(1).readDistance(),
                                        mgr.getDistanceSensor(2).readDistance());
    } else {
        orderManager.highLevel.printfln(SICK_HEADER, "%d %d %d %d %d %d",
                                        mgr.getDistanceSensor(0).readDistance(),
                                        mgr.getDistanceSensor(1).readDistance(),
                                        mgr.getDistanceSensor(2).readDistance(),
                                        mgr.getDistanceSensor(3).readDistance(),
                                        mgr.getDistanceSensor(4).readDistance(),
                                        mgr.getDistanceSensor(5).readDistance());
    }
}

void ORDER_disableTorque::impl(Args args) {
    ActuatorsMgr &manager = ActuatorsMgr::Instance();
    MOVE_ARM(args[0],
             arm->setTorque(false);
    )
}

void ORDER_enableTorque::impl(Args args) {
    ActuatorsMgr &manager = ActuatorsMgr::Instance();
    MOVE_ARM(args[0],
             arm->setTorque(true);
    )
}

void ORDER_torqueBras::impl(Args args)
{
    ActuatorsMgr& manager = ActuatorsMgr::Instance();
    if(strcmp(args[0], "right") != 0) {
        ComMgr::Instance().printfln(DEBUG_HEADER, "'y a pas de bras gauche!");
    }
    /*float couple[3] = {0, 0, 0};
    if (!strcmp(args[1], "sol"))
    {
        for ( int i = 0 ; i < 3 ; i++ )
        { // Pour chaque XL
            AX12 motor = arm->getXLlist()[i];
            if (motor.getCurrentTorque(couple[i]))
            { // renvoit true si la mesure a été effectuée

                // Pour chaque couleur de la plus lourde à la plus légère
                for (int color = (int)PaletColor::GOLD; color < (int)PaletColor::NONE ; color++ )
                {
                    if (couple[i] > coupleSolseuil[i][color])
                    { //test de chaque palet
                        orderManager.highLevel.printfln(ATOM_COLOR_HEADER, "%s", PaletColorToString((PaletColor)color).c_str());
                        break;
                    }
                }
                orderManager.highLevel.printfln(DEBUG_HEADER, "palet non pris");
            }
            else
            {
                orderManager.highLevel.printfln(DEBUG_HEADER, "torque failed");
            }
        }
    }
    else
    {
        for (int i = 0; i < 3; i++)
        { // Pour chaque XL
            AX12 motor = arm->getXLlist()[i];
            if (motor.getCurrentTorque(couple[i]))
            { // renvoit true si la mesure a été effectuée

                // Pour chaque couleur de la plus lourde à la plus légère
                for (int color = (int)PaletColor::GOLD; color < (int)PaletColor::NONE ; color++ )
                {
                    if (couple[i] > coupleDistributeurseuil[i][color])
                    { //test de chaque palet
                        orderManager.highLevel.printfln(ATOM_COLOR_HEADER, "%s",  PaletColorToString((PaletColor)color).c_str());
                        break;
                    }
                }
                orderManager.highLevel.printfln(DEBUG_HEADER, "palet non pris");
            }
            else
            {
                orderManager.highLevel.printfln(DEBUG_HEADER, "torque failed");
            }
        }
    }*/
}

void ORDER_torqueXL :: impl(Args args){
    /* TODO
    ActuatorsMgr& manager = ActuatorsMgr::Instance();
    AX12* motor = (AX12*)manager.dynamixelManager->getMotor(orderManager.parseInt(args[0]));
    int couple;
    if(motor->getCurrentTorque(couple)) {
        orderManager.highLevel.printfln(SENSOR_HEADER,"%i",couple);
    }
    else{
        orderManager.highLevel.printfln(DEBUG_HEADER,"%s","couple failed");
    }*/
}

void ORDER_waitJumper::impl(Args args) {
    // ============================
    // Commenter pour les tests
    // ============================
    Serial.println("Waiting for jumper...");

    digitalWrite(LED1_1, LOW);

    // attente de front
    while(digitalRead(PIN_JMPR) == HIGH) {
        InterruptStackPrint::Instance().print();
    }
    while(digitalRead(PIN_JMPR) == LOW) {
        InterruptStackPrint::Instance().print();
    }
    ComMgr::Instance().printfln(EVENT_HEADER, "gogogofast");
    digitalWrite(LED1_1, HIGH);
    digitalWrite(LED1_2, LOW);

    // ============================
    // Fin de Commenter pour les tests
    // ============================
}

void ORDER_endMatch::impl(Args args) {
    orderManager.execute("stop");
    orderManager.execute("sstop");
    digitalWrite(LED1_3, HIGH);
    digitalWrite(LED2_1, HIGH);
    digitalWrite(LED2_2, HIGH);
    digitalWrite(LED3_1, HIGH);
    digitalWrite(LED3_2, HIGH);
    while(true) {
        digitalWrite(LED1_1, LOW);
        digitalWrite(LED1_2, LOW);
        digitalWrite(LED2_3, LOW);
        digitalWrite(LED3_3, LOW);
        delay(100);
        digitalWrite(LED1_1, HIGH);
        digitalWrite(LED1_2, HIGH);
        digitalWrite(LED2_3, HIGH);
        digitalWrite(LED3_3, HIGH);
        delay(100);
    }
}