## 🔍 Masked Common Substring (MCS) Search Project

This project explores efficient substring search algorithms by improving the time complexity of naive text searching. 
It provides two advanced alternatives to the naive sliding window approach:

- Standard MCS Search: Uses masked filters to quickly identify candidate matches.
- Positional MCS Search: Enhances the standard MCS by incorporating character position information for higher accuracy and speed.

These techniques are particularly useful when handling large-scale textual data with repeated pattern queries.

---

### 📁 Project Structure

```graphql
Lab1-MCS_Search/
├── main.cpp                  # Entry point with interactive menu
├── config.*                  # Constants and configuration values
├── utils.*                   # File I/O and helper utilities
├── WordMatch.hpp             # Structure to hold matched word & positions
├── random_text.*             # Generates and stores random text
├── search_words_generator.*  # Generates search words
├── naive_search.*            # Naive algorithm: brute-force search
├── mcs_creation.*            # MCS filter creation logic
├── standard_mcs_search.*     # Search using standard MCS filters
├── positional_mcs_creation.* # Positional MCS filter creation
├── positional_mcs_search.*   # Search using positional MCS filters
├── create_filters_map.*      # Builds precomputed map of filters:positions
├── final_summary.*           # Summarizes and saves final results
├── Summary.hpp               # Interface for reporting results
└── README.md                 # You’re reading this :)
```

---

### 🧠 Architecture Overview
```pgsql
+----------------------+
|   main.cpp           |
| (User Menu + Config) |
+---------+------------+
          |
          v
+---------------------------+
| Config + Utils            |
| (params, file I/O, etc.)  |
+---------------------------+
          |
          v
+----------------------------+
| Text & Search Word Gen     |
| random_text.cpp            |
| search_words_generator.cpp |
+----------------------------+
          |
          v
+--------------------------+       +------------------------+
| Filter Creation          |       | Filters Map            |
| mcs_creation.cpp         |<----->| create_filters_map.cpp |
| positional_mcs_creation  |       +------------------------+
+--------------------------+
          |
          v
+-------------------------------------------+
| Search Algorithms                         |
| - naive_search.cpp                        |
| - standard_mcs_search.cpp                 |
| - positional_mcs_search.cpp               |
+-------------------------------------------+
          |
          v
+--------------------+
| Summary & Results  |
| final_summary.cpp  |
+--------------------+

```

---

### 🚀 How to Run

This project is run via main.cpp, which presents an interactive menu. It allows the user to:

- Generate random text
- Create search words
- Generate MCS filters
- Perform searches (naive, standard, positional)
- View summary
- Exit

The configuration (like file names and constants) is shown at launch and defined in config.hpp.
To run:
```bash
# Open the project in Visual Studio
# Set main.cpp as startup and run (F5)
```
No arguments or extra setup needed.

---

### Features
- 📜 Full-text random text and word generation
- 🔎 Naive vs. optimized MCS search algorithms
- 💡 Positional filters for greater precision
- 💾 Search results and summaries saved to files
- 📈 Progress reporting in long operations

---

### 📦 Dependencies

This is a C++ project built using Visual Studio (solution file included). No external libraries are required.

---

### 👨‍💻 Authors

- Developed by Adir David as part of the university lab course.
