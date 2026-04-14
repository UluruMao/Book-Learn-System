# Book-Learn-System
This is an individual course project.  This is a terminal C program for students to read summarized text materials for C programming chapter 5 and 6 and track study time. 

## Features

* **Account System:** Register and log in with a Student ID and Username.
* **Study Materials:** Read text files loaded directly from local folders.
* **Time Tracking:** The program records your start time, end time, and total study duration in seconds.
* **Data Storage:** User accounts and study logs are saved to local text and `.dat` files.

## Folder Structure

You must have these specific folders in the same directory as your program before running it.

| Folder | Description |
| :--- | :--- |
| `book/` | Put chapter folders and section `.txt` files here. |
| `account-info/` | Stores the `info.txt` file containing user login data. |
| `logs/` | Stores the `log.txt` file with complete study history. |
| `learn-time/` | Stores `.dat` files for individual study sessions. |

## System Limits

* **Max Students:** 100 users.
* **Max Chapters:** 20 chapters per book.
* **Max Sections:** 20 sections per chapter.

## How to Run

### Prerequisites
Ensure you have a C compiler installed on your system (e.g., `gcc` for Linux/macOS or MinGW for Windows) and `git`.

### 1) Clone the Repository
Open your terminal and run:
```bash
git clone [https://github.com/UluruMao/Book-Learn-System.git](https://github.com/UluruMao/Book-Learn-System.git)
cd Book-Learn-System
```
### 2) Compile the Code
If the repository includes a Makefile, you can compile the project by simply running:

Bash
```
make
```
If there is no Makefile, compile the source files manually using gcc (adjust main.c if your primary file is named differently or if there are multiple .c files):

Bash
```
gcc main.c -o book_learn_system
```
### 3) Run the Application
Once compiled, start the study system:

Linux / macOS:

Bash
```
./book_learn_system
```
Windows (Command Prompt / PowerShell):

PowerShell
```
.\book_learn_system.exe
```
Note: Since the application uses local binary and text file I/O to track study sessions, ensure the program has permission to read/write files in the directory where it is executed.

## Video Presentation
https://youtu.be/8w7lUCVCzMY
