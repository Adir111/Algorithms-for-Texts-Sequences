#include "main.hpp"

using namespace std;
using namespace Config;
using namespace Utils;
using namespace MCS;
using namespace RandomText;
using namespace NaiveSearch;
using namespace SearchWordsGenerator;

int main() {
    int choice;
    validate_config();
    print_config();

    while (true) {
        print_menu();
        cin >> choice;
        cout << "\n";

        switch (choice) {
        case 1:
            handle_operation(msc_creation, "MSC Creation");
            break;
        case 2:
            handle_operation(generate_text_and_save, "Random Text Generation");
            break;
        case 3:
            handle_operation(generate_search_words, "Search Words Generation");
            break;
        case 4:
            handle_operation(perform_naive_search, "Naive Search");
            break;
        case 5:
            cout << "Option " << choice << " not implemented yet.\n";
            break;
        case 6:
            cout << "Option " << choice << " not implemented yet.\n";
            break;
        case 0:
            cout << "Exiting program. Goodbye!\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }

    cout << "Error - program exited unexpected";
    return -1;
}