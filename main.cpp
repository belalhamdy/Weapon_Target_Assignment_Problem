#include <iostream>
#include <vector>

using namespace std;

int main() {
    cout << "Enter the weapon types and the number of instances of each type: (Enter 'x' when you’re done)" << endl;
    int nWeapons = 0;
    while (true) {
        string name;
        int count;
        cin >> name;
        if (name == "x") break;
        cin >> count;
        //do something with name and count
        ++nWeapons;
    }
    cout << "Enter the number of targets:" << endl;
    int nTargets;
    cin >> nTargets;
    cout << "Enter the threat coefficient of each target:" << endl;
    vector<int> threat(nTargets);
    for (int &cur : threat)
        cin >> cur;

    cout << "Enter the weapons’ success probabilities matrix:" << endl;
    vector<vector<double>> success(nWeapons, vector<double>(nTargets));
    for (int i = 0; i < nWeapons; ++i)
        for (int j = 0; j < nTargets; ++j)
            cin >> success[i][j];

    cout << "Please wait while running the GA..." << endl;

    //Do processing here

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