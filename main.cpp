#include <iostream>
#include <vector>
#include <Windows.h>
#include <ctime>
#include <fstream>

using namespace std;

const string FileName = "Goals.txt";

class SavingsGoal
{
public:
    int GoalNumber;
    string name;
    double TargetAmount;
    double CurrentAmount;
    bool completed;

    SavingsGoal(int number, const string &n, double amount) : GoalNumber(number), name(n), TargetAmount(amount), CurrentAmount(0.0), completed(false) {}
};

vector<SavingsGoal> savingGoals;

void SaveFile() {
    ofstream File(FileName);

    if (File.is_open()) {
        for (const auto &goal : savingGoals) {
            File << goal.GoalNumber << " " << goal.name << " " << goal.TargetAmount << " " << goal.CurrentAmount << " " << goal.completed << endl;
        }
        File.close();
        cout << "Goals saved!" << endl;
    } else {
        cout << "Can't access the last save file." << endl;
    }
}

void LoadFile() {

    savingGoals.clear();

    ifstream File(FileName);

    if (File.is_open()) {
        int GoalNumber;
        string name;
        double TargetAmount;
        double CurrentAmount;
        bool completed;

        while (File >> GoalNumber >> name >> TargetAmount >> CurrentAmount >> completed) {
            savingGoals.push_back(SavingsGoal(GoalNumber, name, TargetAmount));
            savingGoals.back().CurrentAmount = CurrentAmount;
            savingGoals.back().completed = completed;
        }
        File.close();
        cout << "Goals loaded from the file!" << endl;
    } else {
        cout << "Can't access the last save file. Creating a new file." << endl;
        ofstream newFile(FileName);
        newFile.close();
    }
}

void AddGoal() {
    string name;
    double TargetAmount;

    cout << "Goal name: " << endl;
    cin >> name;

    cout << "Target amount: " << endl;
    cin >> TargetAmount;

    int GoalNumber = savingGoals.size() + 1;

    savingGoals.push_back(SavingsGoal(GoalNumber, name, TargetAmount));

    cout << "Added a new goal!" << endl;
}

void DisplayGoals() {
    cout << "Your goals: " << endl;

    for (const auto &goal : savingGoals) {
        cout << "Number: " << goal.GoalNumber << ", Goal: " << goal.name << ", Target amount: " << goal.TargetAmount
             << ", Current amount: " << goal.CurrentAmount << ", Status: " << (goal.completed ? "completed" : "Not completed") << endl;
    }
}

void RemoveGoal() {
    if (savingGoals.empty()) {
        cout << endl
             << "No goals to remove." << endl;
        return;
    }

    int GoalNumber;

    DisplayGoals();

    cout << "Enter the goal number to remove: " << endl;
    cin >> GoalNumber;

    if (GoalNumber >= 1 && GoalNumber <= savingGoals.size()) {
        savingGoals.erase(savingGoals.begin() + GoalNumber - 1);
        cout << "Goal removed!" << endl;
    } else {
        cout << "Invalid goal number." << endl;
    }
}

void UpdateGoal() {
    if (savingGoals.empty()) {
        cout << endl
             << "No goals to update." << endl;
        return;
    }

    int GoalNumber;
    double newAmount;

    DisplayGoals();

    cout << "Enter the goal number to update: " << endl;
    cin >> GoalNumber;

    if (GoalNumber >= 1 && GoalNumber <= savingGoals.size()) {
        cout << "Enter a new amount for goal " << GoalNumber << ": ";
        cin >> newAmount;

        savingGoals[GoalNumber - 1].CurrentAmount = newAmount;

        if (savingGoals[GoalNumber - 1].CurrentAmount >= savingGoals[GoalNumber - 1].TargetAmount) {
            savingGoals[GoalNumber - 1].completed = true;
            cout << "Congratulations! The goal has been achieved!" << endl;
        } else {
            cout << "Update successful!" << endl;
        }
    } else {
        cout << "Invalid goal number." << endl;
    }
}

bool ConsentNotification() {
    char response;

    cout << "Do you consent to daily notifications? (Y/N): ";
    cin >> response;

    return (response == 'Y' || response == 'y');
}

void ChangeNotificationConsent(bool &consent) {
    consent = !consent;
    cout << "Consent for notifications has been " << (consent ? "granted." : "withdrawn.") << endl;
}

void SendNotification(const SavingsGoal &goal) {
    LPCWSTR title = L"Reminder about the goal";

    wstring content = L"Reminder about the goal: " + wstring(goal.name.begin(), goal.name.end()) +
                    L"Target amount: " + to_wstring(goal.TargetAmount) +
                    L"\nCurrent amount: " + to_wstring(goal.CurrentAmount);

    MessageBoxW(NULL, content.c_str(), title, MB_ICONINFORMATION | MB_OK);
}

int main() {
    LoadFile();

    bool consentToNotifications = ConsentNotification();
    
    int choice;

    do {
        cout << "Menu: " << endl;
        cout << "1. Add savings goal" << endl;
        cout << "2. Display a list of savings goals" << endl;
        cout << "3. Remove savings goal" << endl;
        cout << "4. Update savings goal" << endl;
        cout << "5. Change notification consent" << endl;
        cout << "0. Exit" << endl;

        cout << "Choose an option:" << endl;
        cin >> choice;

        switch (choice) {
        case 1:
            AddGoal();
            break;
        case 2:
            DisplayGoals();
            break;
        case 3:
            RemoveGoal();
            break;
        case 4:
            UpdateGoal();
            break;
        case 5:
            ChangeNotificationConsent(consentToNotifications);
            break;
        case 0:
            cout << "Saving goals before exiting." << endl;
            SaveFile();
            cout << "Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice." << endl;
        }
        if (consentToNotifications) {
            SendNotification(savingGoals.back());
        }

    } while (choice != 0);

    return 0;
}