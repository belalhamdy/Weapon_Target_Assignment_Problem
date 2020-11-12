#include <iostream>
#include <vector>
#include <random>
#include <chrono>

using namespace std;

mt19937 g_RNG(chrono::steady_clock::now().time_since_epoch().count());
/*
 * Generate Random Chromosomes (Done)
 * Calculate fitness foreach (Done)
 * pick the parents to mate
 * crossover
 * mutate (Done)
 */
struct GeneticAlgorithm {
    const vector<int> &weaponInstances;
    const vector<vector<double>> &success;
    const vector<int> &threat;

    GeneticAlgorithm(const vector<int> &wi,
                     const vector<int> &th,
                     const vector<vector<double>> &su)
            : weaponInstances(wi), threat(th), success(su) {
    }

    struct Chromosome {
        vector<int> data;
        int maxValue;
        Chromosome(int nSize, int maxValue) : maxValue(maxValue) {
            uniform_int_distribution<int> uid(0, maxValue);
            data.resize(nSize);
            for (int &v : data) {
                v = uid(g_RNG);
            }
        }
        void mutate() {
            uniform_int_distribution<int> mutateIdx(0, data.size() - 1);
            uniform_int_distribution<int> mutateValue(0, maxValue);
            data[mutateIdx(g_RNG)] = mutateValue(g_RNG);
        }
        Chromosome combineWith(const Chromosome &other) {
            //TODO
            return Chromosome(0,0);
        }
    };
    double getFitness(const Chromosome &ch) {
        vector<double> survive;
        survive.assign(threat.size(), 1.0);
        for (int x = 0; x < weaponInstances.size(); ++x) {
            int i = weaponInstances[x];
            int j = ch.data[x];
            survive[j] *= 1.0 - success[i][j];
        }
        double ans = 0;
        for (int i = 0; i < survive.size(); ++i) {
            ans += survive[i] * threat[i];
        }
        return ans;
    }
};

int main() {
    cout << "Enter the weapon types and the number of instances of each type: (Enter 'x' when you're done)" << endl;
    vector<string> weapons;         //nWeapons
    vector<int> weaponInstances;    //nWeaponInstances

    while (true) {
        string name;
        int count;
        cin >> name;
        if (name == "x") break;
        weapons.push_back(name);
        cin >> count;
        while (count--)
            weaponInstances.push_back(weapons.size() - 1);
    }

    cout << "Enter the number of targets:" << endl;
    int nTargets;
    cin >> nTargets;

    cout << "Enter the threat coefficient of each target:" << endl;
    vector<int> threat(nTargets);
    for (int &cur : threat)
        cin >> cur;

    cout << "Enter the weapons' success probabilities matrix:" << endl;
    vector<vector<double>> success(weapons.size(), vector<double>(nTargets));
    for (int i = 0; i < weapons.size(); ++i)
        for (int j = 0; j < nTargets; ++j)
            cin >> success[i][j];

    cout << "Please wait while running the GA..." << endl;
    GeneticAlgorithm ga(weaponInstances,threat,success);
    GeneticAlgorithm::Chromosome a(5,5);
    a.data[0] = 0;
    a.data[1] = 2;
    a.data[2] = 0;
    a.data[3] = 1;
    a.data[4] = 1;
    //Do processing here
    cout << ga.getFitness(a) << endl;
    cout << "The final WTA solution is:" << endl;
    /*
    Tank #1 is assigned to target #1,
    Tank #2 is assigned to target #3,
    Aircraft #1 is assigned to target #1,
    Grenade #1 is assigned to target #2,
    Grenade #2 is assigned to target #2,
    */
    double expectedThreat = 0;
    cout << "The expected total threat of the surviving targets is " << expectedThreat << endl;

}