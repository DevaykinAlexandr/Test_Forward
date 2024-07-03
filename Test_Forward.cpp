#include <iostream>
#include <locale.h>
#include <vector>

using namespace std;

class EngineTest {
private:
    double I;
    vector<double> M;
    vector<double> V;
    double T_overheat;
    double HM;
    double HV;
    double C;
    double T_engine;

public:
    EngineTest(double i, vector<double> m, vector<double> v, double t_overheat, double hm, double hv, double c, double environmentTemp) :
        I(i), M(m), V(v), T_overheat(t_overheat), HM(hm), HV(hv), C(c), T_engine(environmentTemp) {}

    double GetM(double speed) {
        double tmpM;
        for (int i = 0; i < V.size() - 1; i++)
        {
            if (speed >= V[i] && speed <= V[i + 1])
            {
                double k = (M[i + 1] - M[i]) / (V[i + 1] - V[i]);
                double b = (V[i + 1] * M[i] - V[i] * M[i + 1]) / (V[i + 1] - V[i]);
                tmpM = k * speed + b;
            }
        }

        return tmpM;
    }

    double calculatePower(double speed, double M) {
        return M * speed / 1000;
    }

    void simulate(double dt, double environmentTemp) {
        double speed = 0.0;
        double simM;

        while (T_engine < T_overheat) {
            simM = GetM(speed);
            double acceleration = simM / I;
            speed += acceleration * dt;

            double heat = simM * HM + speed * speed * HV;
            double cooling = C * (environmentTemp - T_engine);

            T_engine += (heat - cooling) * dt;
        }
    }

    void getMaxPowerSpeed(double& maxPower, double& maxPowerSpeed) {
        double simM;

        for (double speed = 0; speed <= 300; speed++) {
            simM = GetM(speed);
            double power = calculatePower(speed, simM);
            if (power > maxPower) {
                maxPower = power;
                maxPowerSpeed = speed;
            }
        }

    }

    double getTimeToOverheat(double dt, double environmentTemp) {
        double time = 0.;
        double simM;
        double speed = 0.;
        T_engine = environmentTemp;
        while (T_engine < T_overheat) {
            simM = GetM(speed);
            double acceleration = simM / I;
            speed = acceleration * dt;
            double heat = simM * HM + speed * speed * HV;
            double cooling = C * (environmentTemp - T_engine);

            T_engine += (heat - cooling) * dt;
            time += dt;
        }

        return time;
    }

};

int main() {
    setlocale(LC_ALL, "");
    double I = 10.;
    vector<double> M = { 20., 75., 100., 105., 75., 0. };
    vector<double> V = { 0., 75., 150., 200., 250., 300. };
    double T_overheat = 110.;
    double HM = 0.01;
    double HV = 0.0001;
    double C = 0.1;
    double T_engine;

    double environmentTemp;
    cout << "Введите температуру окружающей среды в градусах Цельсия: ";
    cin >> environmentTemp;
    T_engine = environmentTemp;
    EngineTest engineTest(I, M, V, T_overheat, HM, HV, C, T_engine);

    double dt = 0.01; // Шаг времени, при уменьшении получается более точное время перегрева
    double maxPower = 0;
    double maxPowerSpeed = 0;

    engineTest.simulate(dt, environmentTemp); // Симуляция работы двигателя

    cout << "Время, прошедшее с момента старта до перегрева: " << engineTest.getTimeToOverheat(dt, environmentTemp) << " сек" << endl;
    engineTest.getMaxPowerSpeed(maxPower, maxPowerSpeed);
    cout << "Максимальная мощность двигателя: " << maxPower << " кВт" << endl;
    cout << "Скорость коленвала при максимальной мощности: " << maxPowerSpeed << " рад/сек" << endl;

    return 0;
}