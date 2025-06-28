## 🔍 Masked Common Substring (MCS) Search Project

This project builds on the previous MCS search methods by introducing tree-based filter structures for even faster and more scalable substring searching. It improves upon standard and positional MCS filters by enabling hierarchical and memory-efficient lookups.

Key enhancements include:

- Tree-Based MCS Filters: Use tree data structures to represent masks efficiently.
- Integrated Search Word Embedding: Generates text containing search terms for more realistic testing.

---

### 📁 Project Structure

```graphql
Lab2-MCS-Search-With-Trees/
├── main.cpp                        # Entry point with menu interface
├── config.*                        # Global constants and settings
├── create_text_with_search_words.* # Embeds search words into generated text
├── MatchPos.hpp                    # Struct to hold match position info
├── mcs_creation.*                  # Standard MCS filter creation
├── create_mcs_tree.*               # Builds tree-based MCS filters
├── mcs_tree_search.*               # Search using tree-based MCS filters
├── final_summary.*                 # Summarizes search results
├── generated_text.txt              # Generated input text with search words
├── mcs_output.txt                  # MCS matches and log output
└── README.md                       # You’re reading this :)
```

---

### 🧠 Architecture Overview
```pgsql
+-------------------------+
|        main.cpp         |
| (Menu + Execution Flow) |
+-----------+-------------+
            |
            v
+----------------------------+
| Config + Setup + Utilities |
+----------------------------+
            |
            v
+------------------------------------+
| Text & Word Generator              |
| create_text_with_search_words.cpp  |
+------------------------------------+
            |
            v
+---------------------------+       +--------------------------+
| MCS Filter Creation       |<----->| Tree-Based Filter Logic  |
| mcs_creation.cpp          |       | create_mcs_tree.cpp      |
+---------------------------+       +--------------------------+
            |
            v
+---------------------------+
| Tree-Based Search Logic   |
| mcs_tree_search.cpp       |
+---------------------------+
            |
            v
+---------------------------+
| Final Summary + Output    |
| final_summary.cpp         |
+---------------------------+
```

---

### 🚀 How to Run

This project runs from `main.cpp`, offering a menu-based interface to:

- Generate random text with embedded search words
- Create tree-based MCS filters
- Perform MCS tree-based searches
- View final summary output
- Exit

To run:

```bash
# Open in Visual Studio
# Set main.cpp as the startup file
# Press F5 to build and run
```

---

### Features
- 📄 Realistic test generation with embedded search words
- 🌲 Tree-based MCS filters for fast and scalable search
- 📊 Detailed match tracking with summaries
- 💬 Easy-to-use interactive interface

---

### 📦 Dependencies

This is a native C++ project using Visual Studio. It does not rely on external libraries.

---

### 👨‍💻 Authors

- Developed by Adir David as a continuation of a university lab series on text search optimization.
