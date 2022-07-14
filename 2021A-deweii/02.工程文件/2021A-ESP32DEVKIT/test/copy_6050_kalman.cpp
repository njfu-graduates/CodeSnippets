// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include <SimpleKalmanFilter.h>

Adafruit_MPU6050 mpu;
SimpleKalmanFilter MPU_AY(2, 2, 0.01);
// SimpleKalmanFilter MPU_AY(.03, 0.05, 0.01);

void setup(void)
{
    Serial.begin(115200);
    mpu.begin();
    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_94_HZ);
}

void loop()
{

    /* Get new sensor events with the readings */
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    Serial.printf("%.2f,%.2f\n", a.acceleration.y, MPU_AY.updateEstimate(a.acceleration.y));

    delay(100);
}