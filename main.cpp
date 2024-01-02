#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
#include <ctime>
#include <fstream>
#include <chrono>
#include <thread>


using namespace std;

const string FILE_NAME = "Goals.txt";

class SavingsGoal
{
public:
    int GoalNumber;
    string name;
    double TargetAmount;
    double CurrentAmount;
    double DailyContribution;
    bool completed;

    SavingsGoal(int number, const string &n, double amount, double dailyContribution) : GoalNumber(number), name(n), TargetAmount(amount), CurrentAmount(0.0), DailyContribution(dailyContribution) , completed(false) {}

    int EstimatedTime() const{
        if (DailyContribution <= 0.0){
            return -1;
        } else{
            double remainingTime = (TargetAmount - CurrentAmount) / DailyContribution;
            return static_cast<int>(remainingTime);
        }
    }
};

vector<SavingsGoal> savingGoals;

void SAVE_FILE() {
    ofstream File(FILE_NAME);

    if (File.is_open()) {
        for (const auto &goal : savingGoals) {
            File << goal.GoalNumber << " " << goal.name << " " << goal.TargetAmount << " " << goal.CurrentAmount << " " << goal.DailyContribution << " " << goal.completed << endl;
        }
        File.close();
        cout << "Saved!" << endl;
    } else {
        cout << "Can't access the last save file." << endl;
    }
}

void LOAD_FILE() {

    savingGoals.clear();

    ifstream File(FILE_NAME);

    if (File.is_open()) {
        int GoalNumber;
        string name;
        double TargetAmount;
        double CurrentAmount;
        double DailyContribution;
        bool completed;

        while (File >> GoalNumber >> name >> TargetAmount >> CurrentAmount >> DailyContribution >> completed) {
            savingGoals.push_back(SavingsGoal(GoalNumber, name, TargetAmount, DailyContribution));
            savingGoals.back().CurrentAmount = CurrentAmount;
            savingGoals.back().completed = completed;
        }
        File.close();
        cout << "Goals loaded from the file!" << endl;
    } else {
        cout << "Can't access the last save file. Creating a new file." << endl;
        ofstream newFile(FILE_NAME);
        newFile.close();
    }
}

void AddGoal() {
    string name;
    double TargetAmount;
    double DailyContribution;

    cout << "Goal name: " << endl;
    cin.ignore();
    getline(cin, name);

    cout << "Target amount: " << endl;
    cin >> TargetAmount;

    cout << "Daily contribution: " << endl;
    cin >> DailyContribution;

    int GoalNumber = savingGoals.size() + 1;

    savingGoals.push_back(SavingsGoal(GoalNumber, name, TargetAmount, DailyContribution));

    cout << "Added a new goal!" << endl;

    SAVE_FILE();
}


void DisplayGoals() {
    LOAD_FILE();

    if (savingGoals.empty()) {
        cout << "No set goals!" << endl;
        return;
    }

    cout << "Your goals: " << endl;

    for (const auto &goal : savingGoals) {
        cout << "Number: " << goal.GoalNumber 
        << ", Goal: " << goal.name 
        << ", Target amount: " << goal.TargetAmount
        << ", Current amount: " << goal.CurrentAmount 
        << ", Estimated time to achieve: " << goal.EstimatedTime() << " days "
        << ", Status: " << (goal.completed ? "completed" : "Not completed") << endl;
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
    double newDailyContribution;
    char updateDailyContribution;

    DisplayGoals();

    cout << "Enter the goal number to update: " << endl;
    cin >> GoalNumber;

    if (GoalNumber >= 1 && GoalNumber <= savingGoals.size()) {
        cout << "Enter a new amount for goal " << GoalNumber << ": ";
        cin >> newAmount;

        cout << "Would you like to update daily contribution for goal" << GoalNumber << "? (Y/N) " << endl;
        cin >> updateDailyContribution;

        if (updateDailyContribution == 'Y' || updateDailyContribution == 'y'){
            cout << "Enter a new value to contribute daily for goal " << GoalNumber << ": ";
            cin >> newDailyContribution;
            savingGoals[GoalNumber - 1].DailyContribution = newDailyContribution;
        }

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

bool ConsentNotification(bool &consent, bool &savedConsent) {

    if(consent){
        cout << "You already agreed to recieve notifications." << endl;
        return true;
    }

    if(savedConsent){
        consent = true;
        return true;
    }

    char response;

    cout << "Do you consent to daily notifications? (Y/N): " << endl;
    cin >> response;

    if(response == 'Y' || response == 'y'){
        consent = true;
        savedConsent = true;

        SAVE_FILE();

        cout << "Consent has been granted and saved! Thank you." << endl;
        return true;
    } else{
        cout << "Consent has not beed granted." << endl;
        return false;
    }


}

void ChangeNotificationConsent(bool &consent) {

    consent = !consent;
    cout << "Consent for notifications has been " << (consent ? "granted." : "withdrawn.") << endl;
}

int main() {
    LOAD_FILE();

    bool consentToNotifications = false;
    bool savedConsent = !savingGoals.empty();  

    if (!savedConsent) {
        ConsentNotification(consentToNotifications, savedConsent);
    }

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
            SAVE_FILE();
            cout << "Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice." << endl;
        }
    } while (choice != 0);

    return 0;
}