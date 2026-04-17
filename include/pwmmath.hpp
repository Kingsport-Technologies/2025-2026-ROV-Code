#include <iostream>
#include <cmath>
#include <string>
#include <QObject>

class PwmMath : public QObject
{   
    Q_OBJECT
    public:
        PwmMath();
        //gpio pin numbers for each thruster
        int horiz_front_left;
        int horiz_front_right;
        int horiz_back_left;
        int horiz_back_right;
        int vert_left;
        int vert_right;

        //gpio pins for each servo
        int left_claw;
        int left_claw_rotate;
        int right_claw;
        int right_claw_rotate;

        //joystick value variables
        double left_x;
        double left_y;
        double right_x;
        double right_y;

        int pi_num; //the value returned by pigpio_start, needed to use set_servo_pulsewidth

        double angle; //angle of the joystick's position in degrees (in unit circle angle notation, 0-360) 
        double magnitude; //magnitude of the length between origin position and joystick's current position
        double max_magnitude; //the maximum possible length the joystick could have at the current angle
        double multiplier_value; //the value to multiply thruster values if it isn't maximum magnitude
        void initialize_thrusters();
        void calibrate_values();
        double get_angle();
        double get_magnitude();
        double get_maxmagnitude();
        double get_multipliervalue();
        bool lower_range_half();
        double get_b_angle(bool FL_BR, bool neg_to_pos, bool lower_range_half);
        double get_b_magnitude(double a_angle, double b_angle);
        double to_thruster_microseconds(double value);
        void assign_to_thrusters();
        void run_loop();
    slots:
        void incomingControllerData();
    signals:
        void outgoingPWMData(int hfl, int hbl, int hfr, int hbr, int vl, int vf);
};