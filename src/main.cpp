#include <Arduino.h>
#include <robot_drive.h>
#include "rosa_gamepad.h"
#include "spslib.h"
#include "utils.h"

 //ROSA DEFINITIONS
#define COMMANDS_BAUD_RATE 38400


typedef SPS::Message<> ROSAmens;
 //ROSA COMPONENTS
RobotDrive robot;
ROSAgamepad gamepad;

void handle_serial_port();
void setup()
 {

  Serial.begin(COMMANDS_BAUD_RATE);
  robot.setup();
  gamepad.setup();
 }
 
 void loop()
 {
    handle_serial_port();
    robot.loop();
    gamepad.loop();
    
    robot.enable_move_commands();
    if(gamepad.is_controlling()){ 
        if(gamepad.emergency_stop())robot.emergency_stop();
        else{
            robot.set_velocity( 
                -gamepad.get_ry(), //forward
                -gamepad.get_rx(), //sideward
                gamepad.get_ly()  //rotation
                );
            //char txt[100];
            //sprintf(txt,"%5.2f %5.2f %5.2f",-gamepad.get_ry(),-gamepad.get_rx(),gamepad.get_ly());
            //Serial.println(txt);
        }
        robot.disable_move_commands();//only the gamepad moves the robot
    }

 }

void handle_serial_port(){
static ROSAmens::MsgReader reader; //the reader must be persistent

while (Serial.available()) {
    if (reader.add_uchar(Serial.read())) {
        auto m = reader.getMessage();
        /*switch (m.id) {
            case 10: {
            //message of id 10 interpretation, get values
            m.read_cstring(buffer, 100);
            int value = m.read<int>();
            m.read_array<int>(vector, 3);
            //do something with that values...
        }break;
        case 2:
            //message 2 interpretation
            //...
        break;
        default:
            //unknown message
            ;
        }*/
    }
}//while
}

