#include "pwmmath.hpp"
using namespace std;

//claw side of the robot is defined as the front of the robot


void PwmMath::initialize_thrusters() {
    //send each thruster the 1500 microseconds to initialize them
    horiz_front_left = 1500;
    horiz_front_right = 1500;
    horiz_back_left = 1500;
    horiz_back_right = 1500;
    vert_left = 1500;
    vert_right = 1500;
}

void PwmMath::calibrate_values() {
    //account being slightly off center
    if (abs(left_x) <= 0.05) {
        left_x = 0;
    }
    if (abs(left_y) <= 0.05) {
        left_y = 0;
    }
}

double PwmMath::get_angle() {
    //return the angle of the joystick's position in degrees in unit circle notation
    double wrong_sign_angle = atan2(left_y, left_x) * 180 / M_PI;
    if(wrong_sign_angle < 0) {
    	return wrong_sign_angle + 360;
    } else {
    	return wrong_sign_angle;
    }
}

double PwmMath::get_magnitude() {
    //return the magnitude of the length between origin position and joystick's current position
    return sqrt(left_x*left_x + left_y*left_y);
}

double PwmMath::get_maxmagnitude() {
    //return the maximum possible length the joystick could have at the current angle
    //which would be when x and y values are scaled up until +/-1 or both are equal to +/-1
    double scale_value;
    if(!left_x && !left_y) {
        return 0;
    } else if (abs(left_y) > abs(left_x)) {
        scale_value = 1 / abs(left_y);
        return sqrt(pow(left_x*scale_value, 2) + 1);
    }  else {
        scale_value = 1/abs(left_x);
        return sqrt(1 + pow(left_y*scale_value, 2));
    }
}

double PwmMath::get_multipliervalue() {
    //return the value that a and b magnitudes should be multiplied by to get the smaller magnitudes at the same angle
    //for instances where the joystick is not at the complete outer edge. example: (0.5,0.5) instead of (1,1)
    if(max_magnitude) {
        return magnitude/max_magnitude;
    }
    else {
        return 0;
    }
}

bool PwmMath::lower_range_half() {
    //determine if the angle is in the lower 45 degrees of the 90 degree section
	if((angle - (floor(angle / 90) * 90)) < 45) {
    	return true;
    }
    return false;
}

double PwmMath::get_b_angle(bool FL_BR, bool neg_to_pos, bool lower_range_half) {
    //b angle is always going to be either -45, 45, 135, or -135
    //in each 90 degree section one angle/vector is changing (b_angle/vector) and one angle is not changing (a_angle/vector)
    int b_angle;
    if(FL_BR) { //if the frontleft and backright thruster values are changing in the 90 degree section
        b_angle = 45;
    } else {
        b_angle = 135;
    }
    if(neg_to_pos) { //if the changing thruster value goes from backward/negative to forward/positive from beginning to end of 90 degree section
        if(lower_range_half) {//if in the lower 45 degrees of the 90 degree section
            b_angle -= 180;
        }
    } else {
        if(!lower_range_half) {
            b_angle -= 180;
        }
    }
    return b_angle;
}

double PwmMath::get_b_magnitude(double a_angle, double b_angle) {
	double c_angle = angle * M_PI/180; //c_angle is the joystick angle converted to radians
    a_angle *= M_PI/180; //convert angle from degrees to radians
    b_angle *= M_PI/180; //convert angle from degrees to radians

    return ((cos(a_angle)*sin(c_angle) - sin(a_angle)*cos(c_angle))/(sin(b_angle)*cos(c_angle) - cos(b_angle)*sin(c_angle)));
}

double PwmMath::to_thruster_microseconds(double value) {
    //convert -1 to 1 value to 1300 to 1700 value (range for thruster)
    return value * 200 + 1500;
}

void PwmMath::assign_to_thrusters() {
    double a_value; //-1 to 1 value that converted and sent to the a thruster (the one that is not changing value in the 90 degree section)
    double b_angle; //angle (either -45, 45, 135, or -135) of b vector/thruster (the one that is changing value in the 90 degree section)
    double b_magnitude; //magnitude of the b vector
    double b_value; //-1 to 1 value that is converted and sent to the b thruster
    
    if(!left_x && !left_y) {
        //(0,0) from joystick, so robot should not be moving
        //set thruster pwm values
        horiz_front_left = 1500;
        horiz_front_right = 1500;
        horiz_back_left = 1500;
        horiz_back_right = 1500;
    } else if (0 <= angle && angle < 90) { //[0,90) degree section
        a_value = 1*multiplier_value;
        b_angle = get_b_angle(false, true, lower_range_half());
        b_magnitude = get_b_magnitude(45, b_angle);
        if(b_angle < 0) {
            b_value = -1*b_magnitude*multiplier_value;
        } else {
        	b_value = b_magnitude*multiplier_value;
        }
        //set thruster pwm values
        horiz_front_left = to_thruster_microseconds(a_value);
        horiz_front_right = to_thruster_microseconds(b_value);
        horiz_back_left = to_thruster_microseconds(b_value);
        horiz_back_right = to_thruster_microseconds(a_value);
    } else if (90 <= angle && angle < 180) { //[90,180) degree section
        a_value = 1*multiplier_value;
        b_angle = get_b_angle(true, false, lower_range_half());
        b_magnitude = get_b_magnitude(135, b_angle);
        if(b_angle < 0) {
            b_value = -1*b_magnitude*multiplier_value;
        } else {
        	b_value = b_magnitude*multiplier_value;
        }
        //set thruster pwm values
        horiz_front_left = to_thruster_microseconds(b_value);
        horiz_front_right = to_thruster_microseconds(a_value);
        horiz_back_left = to_thruster_microseconds(a_value);
        horiz_back_right = to_thruster_microseconds(b_value);
    } else if (180 <= angle && angle < 270) { //[180,270) degree section
        a_value = -1*multiplier_value;
        b_angle = get_b_angle(false, false, lower_range_half());
        b_magnitude = get_b_magnitude(225, b_angle);
        if(b_angle < 0) {
            b_value = -1*b_magnitude*multiplier_value;
        } else {
        	b_value = b_magnitude*multiplier_value;
        }
        //set thruster pwm values
        horiz_front_left = to_thruster_microseconds(a_value);
        horiz_front_right = to_thruster_microseconds(b_value);
        horiz_back_left = to_thruster_microseconds(b_value);
        horiz_back_right = to_thruster_microseconds(a_value);
    } else { //[270,360/0) degree section
        a_value = -1*multiplier_value;
        b_angle = get_b_angle(true, true, lower_range_half());
        b_magnitude = get_b_magnitude(315, b_angle);
        if(b_angle < 0) {
            b_value = -1*b_magnitude*multiplier_value;
        } else {
        	b_value = b_magnitude*multiplier_value;
        }
        //set thruster pwm values
        horiz_front_left = to_thruster_microseconds(b_value);
        horiz_front_right = to_thruster_microseconds(a_value);
        horiz_back_left = to_thruster_microseconds(a_value);
        horiz_back_right = to_thruster_microseconds(b_value);
    }
}
PwmMath::PwmMath()
{
    initialize_thrusters();
    emit outgoingPWMData(horiz_front_left, horiz_back_left, horiz_front_right, horiz_back_right, vert_left, vert_right);
}
void PwmMath::run_loop()
{
    calibrate_values();
    angle = get_angle();
    magnitude = get_magnitude();
    max_magnitude = get_maxmagnitude();
    multiplier_value = get_multipliervalue();
    assign_to_thrusters();
}
void PwmMath::incomingControllerData(std::vector<Sint16> data)
{
    left_x = (float)data[0];
    left_y = (float)data[1];
    right_x = (float)data[2];
    right_y = (float)data[3];
    run_loop();
    emit outgoingPWMData(horiz_front_left, horiz_back_left, horiz_front_right, horiz_back_right, vert_left, vert_right);
}
