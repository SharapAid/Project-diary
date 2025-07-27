```markdown
╭────────────────────╮
│     ~ Diary ~      │
╰────────────────────╯
```

📓 **Project Diary** — Console-Based C++ Journal App

Project Diary is a minimalist, fast, and local journal application written in pure C++, using SQLite3 as its database. No frameworks, no web dependencies — just clean and modular C++ code.

---

## 🚀 Features

- 👤 User registration and login  
- 📝 Write diary entries  
- 📖 Read all entries  
- ✏️ Change username and password  
- ❌ Delete user profile  
- 🔐 Credential verification  

---

## 🧰 Tech Stack

| Language | Database | Components                                  |
|----------|----------|---------------------------------------------|
| C++      | SQLite3  | Application, User, UserProfile, UserManager |

---

## 🔧 How to Run

```bash
# 1. Clone the repository
git clone https://github.com/SharapAid/Project-diary.git
cd Project-diary
```

```bash
# 2. Compile with g++
g++ -std=c++17 *.cpp -lsqlite3 -o diary_app
```

```bash
# 3. Run the application
./diary_app
```
---

## 📌 Notes

  All user data is stored locally in a .sqlite database file

  Passwords must be at least 6 characters and are verified during login/change/delete

  Interface is entirely terminal-based using std::cin and std::cout
    
---

## 👩‍💻 Author

Created by — SharapAid

---

## 🖼️ Program Example

```markdown
Welcome to the Diary App!

_______________MENU_____________
What do you want to do?
1 - Sign in
2 - Register
0 - Exit
Enter your choice: 1

_______________SIGN IN____________
Enter name*: Vika
Enter password*: ********
Login successful!

__________MENU USER: SharapAid__________
What would you like to do?
1 - Write a diary entry
2 - Read all entries
3 - Change name
4 - Change password
5 - Delete profile
0 - Exit to main menu
Enter your choice: 1

__________WRITE DIARY__________
Enter text of diary*: Today I discovered a minimalist C++ diary app!

Entry saved successfully.

Returning to user menu...
```