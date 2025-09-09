// Assuming the original function looks something like this:
float constrainAngle(float angle) {
    while (angle < -180.0) angle += 360.0;
    while (angle > 180.0) angle -= 360.0;
    return angle;
}

// Modified function to produce angles in the range of 0-360°
float constrainAngle(float angle) {
    while (angle < 0.0) angle += 360.0;
    while (angle >= 360.0) angle -= 360.0;
    return angle;
}