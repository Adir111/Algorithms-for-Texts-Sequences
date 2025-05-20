#include "main.hpp"

using namespace std;
using namespace Config;
using namespace Utils;
using namespace MCS;
using namespace RandomText;
using namespace NaiveSearch;
using namespace SearchWordsGenerator;
using namespace FiltersMap;
using namespace StandardMCSSearch;
using namespace PositionalMCS;

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
            handle_operation(mcs_creation, choice);
            break;
        case 2:
            handle_operation(positional_mcs_creation, choice);
            break;
        case 3:
            handle_operation(generate_text_and_save, choice);
            break;
        case 4:
            handle_operation(generate_search_words, choice);
            break;
        case 5:
            handle_operation(create_filters_map, choice);
            break;
        case 6:
            handle_operation(perform_naive_search, choice);
            break;
        case 7:
            handle_operation(run_standard_mcs_search, choice);
            break;
        case 8:
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