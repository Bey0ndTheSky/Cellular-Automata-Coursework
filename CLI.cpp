#include <iostream>
#include <string>
#include <memory>
#include "Question.h"
#include "Question1.h"
#include "Question2.h"
#include "Question5.h"
#include "FileManager.h"
#include "FileLoader.h"


class CLI {
public:
    CLI();
    ~CLI();
    void start();

private:
    void showMenu();
    void loadFile(bool pattern);
    std::unique_ptr<Question> createQuestion(int number);
};

CLI::CLI() {}

CLI::~CLI() {}

using namespace std;

void CLI::showMenu() {
    cout << "Select a question (1-5), 9 to load a previous simulation from disk, or 0 to exit:" << endl;
    for (int i = 1; i <= 5; ++i) {
        cout << i << ". Question " << i << endl;
    }
    cout << "9. Load a previous simulation" << endl;
    cout << "0. Exit" << endl;
}

std::unique_ptr<Question> CLI::createQuestion(int number) {
    switch (number) {
    case 1: return std::make_unique<Question1>();
    case 2: return std::make_unique<Question2>(std::initializer_list<std::string>{"block", "beehive"});
    case 3: return std::make_unique<Question2>(std::initializer_list<std::string>{"blinker", "toad"});
    case 4: return std::make_unique<Question2>(std::initializer_list<std::string>{"glider", "lwss", "mwss","hwss"});
    case 5: return std::make_unique<Question5>();
    default: return nullptr;
    }
}

void CLI::loadFile(bool pattern) {
    string filename;
    int reset;
    cout << "Enter the filename of the saved simulation: ";
    cin >> filename;

    cout << "1. Load the simulation at the saved state" << endl;
    cout << "2. Reload the simulation from starting conditions" << endl;
    cin >> reset;
    reset--;
    if (reset != 0 && reset != 1) { return;}
    std::unique_ptr<FileLoader> loader = FileManager::getInstance().loadSimFromFile(filename, reset);
    if (loader == nullptr) { return;}
    if (!pattern)loader->run(0);
    else loader->runUntilPattern(std::initializer_list<std::string>{"toad"});
}

void CLI::start() {
    int choice = -1;

    while (choice != 0) {
        showMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5: {
            std::unique_ptr<Question> question = createQuestion(choice);
            if (question) {
                question->run();
            }
            break;
        }
        case 8:
            loadFile(true);
            break;
        case 9:
            loadFile(false);
            break;
        case 0:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice, please try again." << endl;
            break;
        }
    }
}

int main() {
    CLI cli;
    cli.start();
    return 0;
}