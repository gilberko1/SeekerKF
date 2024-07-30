#include <iostream>
#include <fstream>
#include <random>
#define M_PI 3.14159265359
#define DEG2RAD M_PI/180
static uint32_t counter = 0;
class KalmanFilter {
public:
    KalmanFilter() {
        x[0] = 0.0; // angle
        x[1] = 0.0; // angle rate

        P[0][0] = 1.0; P[0][1] = 0.0;
        P[1][0] = 0.0; P[1][1] = 1.0;

        Q[0][0] = 0.001; Q[0][1] = 0.0;
        Q[1][0] = 0.0; Q[1][1] = 0.003;

        R_angle_rate = 0.1;
    }

    double wrapAngle(double angle) {
    while (angle > M_PI) angle -= 2.0 * M_PI;
    while (angle < -M_PI) angle += 2.0 * M_PI;
    return angle;
    }

    void predict(double angle, double dt) {
        double F[2][2] = {
            {1, dt},
            {0, 1}
        };

        double x_pred[2];
        x_pred[0] = angle; 
        x_pred[1] = (angle - x[0]) / dt; 

        double P_pred[2][2];
        P_pred[0][0] = F[0][0] * P[0][0] + F[0][1] * P[1][0];
        P_pred[0][1] = F[0][0] * P[0][1] + F[0][1] * P[1][1];
        P_pred[1][0] = F[1][0] * P[0][0] + F[1][1] * P[1][0];
        P_pred[1][1] = F[1][0] * P[0][1] + F[1][1] * P[1][1];

        P_pred[0][0] += Q[0][0];
        P_pred[0][1] += Q[0][1];
        P_pred[1][0] += Q[1][0];
        P_pred[1][1] += Q[1][1];

        x[0] = x_pred[0];
        x[1] = x_pred[1];
        P[0][0] = P_pred[0][0];
        P[0][1] = P_pred[0][1];
        P[1][0] = P_pred[1][0];
        P[1][1] = P_pred[1][1];
    }

    void updateAngleRate(double angle_rate) 
    {
        double K[2]; 
        double temp[2][2];
        double y = angle_rate - x[1]; 
        double S = P[1][1] + R_angle_rate; 
        double ChiSquare = y * S * y;
        if (ChiSquare > 0.0001)
        {
            counter++;
            return;
        }
        
        K[0] = P[0][1] / S;
        K[1] = P[1][1] / S;

        x[0] += K[0] * y;
        x[1] += K[1] * y;

        //Joseph form
        double I_KH[2][2] = {
            {1 - K[0] * 0, -K[0] * 1},
            {-K[1] * 0, 1 - K[1] * 1}
        };
        
        temp[0][0] = I_KH[0][0] * P[0][0] + I_KH[0][1] * P[1][0];
        temp[0][1] = I_KH[0][0] * P[0][1] + I_KH[0][1] * P[1][1];
        temp[1][0] = I_KH[1][0] * P[0][0] + I_KH[1][1] * P[1][0];
        temp[1][1] = I_KH[1][0] * P[0][1] + I_KH[1][1] * P[1][1];

        P[0][0] = temp[0][0] + K[0] * R_angle_rate * K[0];
        P[0][1] = temp[0][1] + K[0] * R_angle_rate * K[1];
        P[1][0] = temp[1][0] + K[1] * R_angle_rate * K[0];
        P[1][1] = temp[1][1] + K[1] * R_angle_rate * K[1];
    }

    double getAngle() const {
        return x[0];
    }

    double getAngleRate() const {
        return x[1];
    }

private:
    double x[2]; // State vector: [angle, angle rate]
    double P[2][2]; // Estimate covariance matrix
    double Q[2][2]; // Process noise covariance matrix
    double R_angle_rate; // Measurement noise covariance for angle rate
};

int main() {
    KalmanFilter kf;

    double dt = 0.02; // 50Hz sampling period

    // Noise generation setup
    std::default_random_engine generator;
    std::normal_distribution<double> angle_noise(0.0, 0.01);
    std::normal_distribution<double> angle_rate_noise(0.0, 0.01);

    std::ofstream file("kalman_output.csv");
    file << "Time,Correct Angle,Correct Angle Rate,Predicted Angle,Predicted Angle Rate\n";
    static double correct_angle = 10 * DEG2RAD;

    for (int i = 0; i < 10000; ++i) {
        double correct_angle_rate = 10 * DEG2RAD;
        correct_angle += correct_angle_rate * dt;
        correct_angle = kf.wrapAngle(correct_angle);
        double noisy_angle = correct_angle + angle_noise(generator);

        kf.predict(noisy_angle, dt);
        if (i % 50 == 0) {
            double noisy_angle_rate = correct_angle_rate + angle_rate_noise(generator);
            kf.updateAngleRate(noisy_angle_rate);
        }
        double predicted_angle = kf.getAngle();
        double predicted_angle_rate = kf.getAngleRate();

        file << i * dt << "," << correct_angle << "," << correct_angle_rate << "," << predicted_angle << "," << predicted_angle_rate << "\n";
    }
    printf("Counter is %u \n", counter);
    file.close();
    std::cout << "Data has been written to kalman_output.csv" << std::endl;
    const char* pythonScriptPath = "PlotRes.py";
    std::string command = "python3 " + std::string(pythonScriptPath);
    int result = system(command.c_str());
    if (result == 0) {
        std::cout << "Python script executed successfully." << std::endl;
    } else {
        std::cout << "Error executing Python script." << std::endl;
    }
    return 0;
}
