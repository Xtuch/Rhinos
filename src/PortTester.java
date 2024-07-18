package org.firstinspires.ftc.teamcode.Eliud;



import com.qualcomm.robotcore.eventloop.opmode.LinearOpMode;
import com.qualcomm.robotcore.eventloop.opmode.TeleOp;
import com.qualcomm.robotcore.hardware.DcMotor;
import com.qualcomm.robotcore.hardware.Servo;



@TeleOp(name="portTester", group="Linear Opmode")
public class PortTester extends LinearOpMode {



    //Motors
    private DcMotor C0;
    private DcMotor C1;
    private DcMotor C2;
    private DcMotor C3;
    private DcMotor E0;
    private DcMotor E1;
    private DcMotor E2;
    private DcMotor E3;



    //Servos
    private Servo SC0;
    private Servo SC1;
    private Servo SC2;
    private Servo SC3;
    private Servo SC4;
    private Servo SC5;
    private Servo SE0;
    private Servo SE1;
    private Servo SE2;
    private Servo SE3;
    private Servo SE4;
    private Servo SE5;



    //Arrays & variables

    int number = 0;
    String type = "motor";
    String hub = "control";



    @Override
    public void runOpMode() {

        //Motores



        C0 = hardwareMap.get(DcMotor.class, "C0");
        C1 = hardwareMap.get(DcMotor.class, "C1");
        C2 = hardwareMap.get(DcMotor.class, "C2");
        C3 = hardwareMap.get(DcMotor.class, "C3");



        E0 = hardwareMap.get(DcMotor.class, "E0");
        E1 = hardwareMap.get(DcMotor.class, "E1");
        E2 = hardwareMap.get(DcMotor.class, "E2");
        E3 = hardwareMap.get(DcMotor.class, "E3");



        SC0 = hardwareMap.get(Servo.class, "SC0");
        SC1 = hardwareMap.get(Servo.class, "SC1");
        SC2 = hardwareMap.get(Servo.class, "SC2");
        SC3 = hardwareMap.get(Servo.class, "SC3");
        SC4 = hardwareMap.get(Servo.class, "SC4");
        SC5 = hardwareMap.get(Servo.class, "SC5");



        SE0 = hardwareMap.get(Servo.class, "SE0");
        SE1 = hardwareMap.get(Servo.class, "SE1");
        SE2 = hardwareMap.get(Servo.class, "SE2");
        SE3 = hardwareMap.get(Servo.class, "SE3");
        SE4 = hardwareMap.get(Servo.class, "SE4");
        SE5 = hardwareMap.get(Servo.class, "SE5");



        waitForStart();



        while (opModeIsActive()) {



            telemetry.addLine("broadcasting on: " + hub);
            telemetry.addLine(type + ": " + number);
            telemetry.update();

            if(gamepad1.a){number++; sleep(100);}

            if(gamepad1.b){number--; sleep(100);}

            if(type == "motor"){if(number > 4){number = 4;}}

            if(type == "servo"){if(number > 7){number = 7;}}

            if(number < 0){number = 0;}



            if(hub.equals("control")){

                if(type.equals("motor")){

                if(number == 0){C0.setPower(gamepad1.left_stick_y);}
                if(number == 1){C1.setPower(gamepad1.left_stick_y);}
                if(number == 2){C2.setPower(gamepad1.left_stick_y);}
                if(number == 3){C3.setPower(gamepad1.left_stick_y);}

                }

                if(type.equals("servo")){

                if(number == 0){SC0.setPosition(gamepad1.left_stick_y + 0.5);}
                if(number == 1){SC1.setPosition(gamepad1.left_stick_y + 0.5);}
                if(number == 2){SC2.setPosition(gamepad1.left_stick_y + 0.5);}
                if(number == 3){SC3.setPosition(gamepad1.left_stick_y + 0.5);}
                if(number == 4){SC4.setPosition(gamepad1.left_stick_y + 0.5);}
                if(number == 5){SC5.setPosition(gamepad1.left_stick_y + 0.5);}

                }

            }

            if(hub.equals("expansion")){

                if(type.equals("motor")){

                if(number == 0){E0.setPower(gamepad1.left_stick_y);}
                if(number == 1){E1.setPower(gamepad1.left_stick_y);}
                if(number == 2){E2.setPower(gamepad1.left_stick_y);}
                if(number == 3){E3.setPower(gamepad1.left_stick_y);}

                }

                if(type.equals("servo")){

                if(number == 0){SE0.setPosition(gamepad1.left_stick_y + 0.5);}
                if(number == 1){SE1.setPosition(gamepad1.left_stick_y + 0.5);}
                if(number == 2){SE2.setPosition(gamepad1.left_stick_y + 0.5);}
                if(number == 3){SE3.setPosition(gamepad1.left_stick_y + 0.5);}
                if(number == 4){SE4.setPosition(gamepad1.left_stick_y + 0.5);}
                if(number == 5){SE5.setPosition(gamepad1.left_stick_y + 0.5);}

                }

            }
        }
    }
}

/* Code that aids port identification

press X to change between servos and motors

press Y to change hub

press B to change to next port

press A to change to past port

use gamepad1.left_stick_y to move

 */
