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
 * crossover (Done)
 * mutate (Done)
 */
struct GeneticAlgorithm {
    struct Chromosome {
        vector<int> data;
        int maxValue;

        Chromosome(int nSize, int maxValue) : maxValue(maxValue), data(nSize) {
            uniform_int_distribution<int> uid(0, maxValue);
            for (int &v : data)
                v = uid(g_RNG);
        }

        void mutate() {
            uniform_int_distribution<int> mutateIdx(0, data.size() - 1);
            uniform_int_distribution<int> mutateValue(0, maxValue);
            data[mutateIdx(g_RNG)] = mutateValue(g_RNG);
        }

        Chromosome combineWith(const Chromosome &other) const {
            Chromosome ret = *this;
            uniform_int_distribution<int> mutateOffset(0, data.size());
            int off = mutateOffset(g_RNG);
            for (int i = 0; i < off; ++i) {
                ret.data[i] = other.data[i];
            }
            return ret;
        }

    };

    const vector<int> &weaponInstances;
    const vector<vector<double>> &success;
    const vector<int> &threat;
    vector<pair<Chromosome, double>> population;

    GeneticAlgorithm(const vector<int> &wi,
                     const vector<int> &th,
                     const vector<vector<double>> &su, int populationSize)
            : weaponInstances(wi), threat(th), success(su), population(populationSize) {

        for (int i = 0; i < populationSize; ++i) {
            Chromosome curr(wi.size(), su[0].size());
            population.emplace_back(curr, getFitness(curr));
        }
    }

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

    void nextGeneration() {
        double totalFitness = 0.0;
        vector<double> probs;
        for (auto &i : population) totalFitness += i.second;
        for (auto &i : population) probs.push_back(i.second /= totalFitness);

        discrete_distribution<> rouletteWheel(probs.begin(),probs.end());
        Chromosome firstParent = population[rouletteWheel(g_RNG)].first;
        Chromosome secondParent = population[rouletteWheel(g_RNG)].first;

        // TODO: Here you have 2 parents mutate them and generate the next generation and fill it in the population
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
    GeneticAlgorithm ga(weaponInstances, threat, success, 5);
    GeneticAlgorithm::Chromosome a(5, 5);
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
