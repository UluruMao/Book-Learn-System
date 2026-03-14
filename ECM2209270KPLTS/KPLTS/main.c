/* Knowledge point learning tracking system created by ECM2209270 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <time.h>

#define MAX_STUDENTS 100
#define MAX_FILE_NAME_LENGTH 100
#define MAX_CHAPTER_NAME_LENGTH 100
#define MAX_CHAPTER_COUNT 20
#define MAX_SECTION_COUNT 20
#define USERNAME_MAX_LENGTH 100

struct Student {
    char username[USERNAME_MAX_LENGTH];
    char studentID[USERNAME_MAX_LENGTH];
};

struct Node {
    char name[MAX_CHAPTER_NAME_LENGTH];
    char fileName[MAX_FILE_NAME_LENGTH];
    struct Node *sections[MAX_SECTION_COUNT];
    int sectionCount;
};

struct Book {
    struct Node *chapters[MAX_CHAPTER_COUNT];
    int chapterCount;
};

struct LearningRecord {
    char *username;
    char studentID[USERNAME_MAX_LENGTH];
    char *fileName;
    time_t timestamp;
    time_t startTime;
    time_t endTime;
    int studyDuration;
};

void initStudent(struct Student *student, const char *username, const char *studentID);

void initLearningRecord(struct LearningRecord *record, const char *username, const char *studentID, const char *fileName);

void displayMenu(struct Book *book);

void displayFileContent(char *filePath);

bool registerStudent(struct Student students[], int *numStudents);

void saveStudentInfoToFile(struct Student students[], int numStudents);

void loadStudentInfoFromFile(struct Student students[], int *numStudents);

struct Book *loadTextFiles(const char *directory);

int getStudyDuration(const struct LearningRecord *records, int numRecords, const char *username);

void recordLearningTime(struct LearningRecord *records, int *numRecords, const char *username, const char *studentID, const char *fileName, int studyDuration);

void loadLearningRecordsFromFile(struct LearningRecord learningRecords[], int *numRecords);

void saveLearningRecordToFile(const struct LearningRecord *record);

int main(void) {
    struct Student students[MAX_STUDENTS];
    int numStudents = 0;
    loadStudentInfoFromFile(students, &numStudents);
    struct Book *book = loadTextFiles("book");
    struct LearningRecord learningRecords[MAX_STUDENTS * MAX_CHAPTER_COUNT * MAX_SECTION_COUNT];
    int numRecords = 0;
    loadLearningRecordsFromFile(learningRecords, &numRecords);
    int choice;

    while (true) {
        printf("==== Book Learn System ====\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 0:
                printf("Exiting program.\n");
                exit(0);
            case 1:
                registerStudent(students, &numStudents);
                break;
            case 2: {
                char username[USERNAME_MAX_LENGTH];
                printf("Enter your username: ");
                scanf("%s", username);
                int studentIndex = -1;
                for (int i = 0; i < numStudents; ++i) {
                    if (strcmp(students[i].username, username) == 0) {
                        studentIndex = i;
                        break;
                    }
                }
                if (studentIndex != -1) {
                    printf("Login successful. Welcome, %s!\n", students[studentIndex].username);
                    int secondMenuChoice;
                    while (true) {
                        printf("==== Book Learn System ====\n");
                        printf("1. Go To Study\n");
                        printf("2. Query Study Duration\n");
                        printf("0. Return to Main Menu\n");
                        printf("Enter your choice: ");
                        scanf("%d", &secondMenuChoice);
                        switch (secondMenuChoice) {
                            case 0:
                                printf("Returning to Main Menu.\n");
                                FILE *file = fopen("logs/log.txt", "a");
                                if (file != NULL) {
                                    for (int i = 0; i < numRecords; ++i) {
                                        fprintf(file, "%s %s %s %ld %ld %d\n", learningRecords[i].username, learningRecords[i].studentID, learningRecords[i].fileName, learningRecords[i].startTime, learningRecords[i].endTime, learningRecords[i].studyDuration);
                                    }
                                    fclose(file);
                                } else {
                                    printf("Error opening log file for writing.\n");
                                }
                                break;
                            case 1: {
                                do {
                                    displayMenu(book);
                                    printf("Enter your choice (0 to exit, -1 to go back): ");
                                    scanf("%d", &choice);
                                    switch (choice) {
                                        case 0:
                                            printf("Logging out.\n");
                                            break;
                                        case -1:
                                            break;
                                        case 1 ... 1000: {
                                            int initialChoice = choice;
                                            printf("You selected %s:\n", book->chapters[initialChoice / 100 - 1]->sections[initialChoice % 100 - 1]->name);
                                            time_t startTime;
                                            time(&startTime);
                                            displayFileContent(book->chapters[initialChoice / 100 - 1]->sections[initialChoice % 100 - 1]->fileName);
                                            do {
                                                printf("Enter -1 to return to the directory menu: ");
                                                scanf("%d", &choice);
                                            } while (choice != -1);
                                            time_t endTime;
                                            time(&endTime);
                                            int studyDuration = difftime(endTime, startTime);
                                            recordLearningTime(learningRecords, &numRecords, username, students[studentIndex].studentID, book->chapters[initialChoice / 100 - 1]->sections[initialChoice % 100 - 1]->fileName, studyDuration);
                                            break;
                                        }
                                        default:
                                            printf("Invalid choice. Please enter a number.\n");
                                    }
                                } while (choice != 0);
                            }
                                break;
                            case 2: {
                                int duration = getStudyDuration(learningRecords, numRecords, username);
                                time_t currentTime;
                                time(&currentTime);
                                printf("Student ID: %s, Your total study duration is %d seconds.\n", students[studentIndex].studentID, duration);
                                for (int i = 0; i < numRecords; ++i) {
                                    if (strcmp(learningRecords[i].username, username) == 0) {
                                        printf("Record %d:\n", i + 1);
                                        printf("  Start Time: %s", ctime(&learningRecords[i].startTime));
                                        if (learningRecords[i].endTime == 0) {
                                            printf("  End Time: %s", ctime(&currentTime));
                                        } else {
                                            printf("  End Time: %s", ctime(&learningRecords[i].endTime));
                                        }
                                        printf("  Duration: %d seconds\n", learningRecords[i].studyDuration);
                                        printf("\n");
                                    }
                                }
                                saveLearningRecordToFile(&learningRecords[numRecords - 1]);
                            }
                                break;
                            default:
                                printf("Invalid choice. Please enter a number.\n");
                        }
                        if (secondMenuChoice == 0) {
                            break;
                        }
                    }
                    break;
                } else {
                    printf("Login failed. Please check your username.\n");
                }
            }
                break;
            default:
                printf("Invalid choice. Please enter a number between 0 and 2.\n");
        }
    }
}

void initStudent(struct Student *student, const char *username, const char *studentID) {
    strncpy(student->username, username, sizeof(student->username) - 1);
    student->username[sizeof(student->username) - 1] = '\0';  // Ensure null-terminated
    strncpy(student->studentID, studentID, sizeof(student->studentID) - 1);
    student->studentID[sizeof(student->studentID) - 1] = '\0';  // Ensure null-terminated
}


void initLearningRecord(struct LearningRecord *record, const char *username, const char *studentID, const char *fileName) {
    record->username = strdup(username);
    strncpy(record->studentID, studentID, sizeof(record->studentID) - 1);
    record->studentID[sizeof(record->studentID) - 1] = '\0';  // Ensure null-terminated
    record->fileName = strdup(fileName);
    time(&record->startTime);
    record->endTime = 0;
    record->studyDuration = 0;
}


void freeNodes(struct Node *node) {
    for (int i = 0; i < node->sectionCount; ++i) {
        freeNodes(node->sections[i]);
    }
    free(node);
}

void displayMenu(struct Book *book) {
    printf("==== Text File Viewer ====\n");
    for (int i = 0; i < book->chapterCount; ++i) {
        struct Node *chapter = book->chapters[i];
        printf("%d. %s\n", i + 1, chapter->name);
        for (int j = 0; j < chapter->sectionCount; j++) {
            struct Node *section = chapter->sections[j];
            printf("  "); // Indent to display directory hierarchy
            printf("%d. %s\n", (i + 1) * 100 + j + 1, section->name);
        }
    }
    printf("0. Exit\n");
}

void displayFileContent(char *filePath) {
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    char buffer[1000];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        fwrite(buffer, 1, bytesRead, stdout);
    }
    fclose(file);
}

bool registerStudent(struct Student students[], int *numStudents) {
    if (*numStudents < MAX_STUDENTS) {
        struct Student newStudent;
        char username[USERNAME_MAX_LENGTH];
        printf("Enter your student ID: ");
        scanf("%s", newStudent.studentID);
        printf("Enter your username: ");
        scanf("%s", username);
        initStudent(&newStudent, username, newStudent.studentID);
        for (int i = 0; i < *numStudents; ++i) {
            if (strcmp(newStudent.studentID, students[i].studentID) == 0) {
                printf("Student ID already exists. Registration failed.\n");
                return false;
            }
        }
        students[*numStudents] = newStudent;
        (*numStudents)++;
        saveStudentInfoToFile(students, *numStudents);
        printf("Registration successful. You can now log in.\n");
        return true;
    } else {
        printf("Maximum number of students reached. Registration failed.\n");
        return false;
    }
}

void saveStudentInfoToFile(struct Student students[], int numStudents) {
    FILE *file = fopen("account-info/info.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < numStudents; ++i) {
        fprintf(file, "%s %s\n", students[i].username, students[i].studentID);
    }

    fclose(file);
}

void loadStudentInfoFromFile(struct Student students[], int *numStudents) {
    FILE *file = fopen("account-info/info.txt", "r");
    if (file == NULL) {
        printf("No existing user data found.\n");
        return;
    }
    while (fscanf(file, "%s %s", students[*numStudents].username, students[*numStudents].studentID) == 2) {
        (*numStudents)++;
        if (*numStudents >= MAX_STUDENTS) {
            break;
        }
    }
    fclose(file);
}

struct Book *loadTextFiles(const char *directory) {
    struct Book *book = malloc(sizeof(struct Book));
    book->chapterCount = 0;
    DIR *bookDir = opendir(directory);
    if (bookDir == NULL) {
        printf("Error opening directory.\n");
        return NULL;
    }
    struct dirent *bookChapter;
    while ((bookChapter = readdir(bookDir))) {
        if (strcmp(".", bookChapter->d_name) != 0 && strcmp("..", bookChapter->d_name) != 0) {
            struct Node *chapter = malloc(sizeof(struct Node));
            strcpy(chapter->name, bookChapter->d_name);
            snprintf(chapter->fileName, MAX_FILE_NAME_LENGTH, "%s/%s/", directory, bookChapter->d_name);
            chapter->sectionCount = 0;
            book->chapters[book->chapterCount] = chapter;
            book->chapterCount++;
            DIR *chapterDir = opendir(chapter->fileName);
            struct dirent *chapterSection;
            while ((chapterSection = readdir(chapterDir))) {
                if (strcmp(".", chapterSection->d_name) != 0 && strcmp("..", chapterSection->d_name) != 0) {
                    struct Node *section = malloc(sizeof(struct Node));
                    strcpy(section->name, chapterSection->d_name);
                    snprintf(section->fileName, MAX_FILE_NAME_LENGTH, "%s%s", chapter->fileName, chapterSection->
                            d_name);
                    chapter->sections[chapter->sectionCount] = section;
                    chapter->sections[chapter->sectionCount] = section;
                    chapter->sectionCount++;
                }
            }
            closedir(chapterDir);
        }
    }
    closedir(bookDir);
    return book;
}

void recordLearningTime(struct LearningRecord *records, int *numRecords, const char *username, const char *studentID, const char *fileName, int studyDuration) {
    time_t currentTime;
    time(&currentTime);

    // Add learning record to the array
    initLearningRecord(&records[*numRecords], username, studentID, fileName);
    records[*numRecords].timestamp = currentTime;
    records[*numRecords].startTime = currentTime;  // Set start time
    records[*numRecords].endTime = currentTime + studyDuration;  // Set end time
    records[*numRecords].studyDuration = studyDuration;

    // Increment the number of learning records
    (*numRecords)++;

    saveLearningRecordToFile(&records[*numRecords - 1]);
}

void loadLearningRecordsFromFile(struct LearningRecord learningRecords[], int *numRecords) {
    FILE *logFile = fopen("logs/log.txt", "r");
    if (logFile != NULL) {
        while (fscanf(logFile, "%s %s %s %ld %ld %d", learningRecords[*numRecords].username, learningRecords[*numRecords].studentID, learningRecords[*numRecords].fileName, &learningRecords[*numRecords].startTime, &learningRecords[*numRecords].endTime, &learningRecords[*numRecords].studyDuration) == 6) {
            (*numRecords)++;
            if (*numRecords >= MAX_STUDENTS * MAX_CHAPTER_COUNT * MAX_SECTION_COUNT) {
                // Prevent array out-of-bounds
                break;
            }
        }
        fclose(logFile);
    } else {
        printf("No existing learning records found.\n");
    }
}

void saveLearningRecordToFile(const struct LearningRecord *record) {
    char filename[50];
    snprintf(filename, sizeof(filename), "%s/%ld.dat", "learn-time", record->timestamp);

    FILE *file = fopen(filename, "w");
    if (file != NULL) {
        fprintf(file, "%s %s %s %ld %ld %d\n", record->username, record->studentID, record->fileName, record->startTime, record->endTime, record->studyDuration);
        fclose(file);
    } else {
        printf("Error opening file for writing.\n");
    }
}

int getStudyDuration(const struct LearningRecord *records, int numRecords, const char *username) {
    int totalDuration = 0;
    time_t currentTime;
    time(&currentTime);
    for (int i = 0; i < numRecords; ++i) {
        if (strcmp(records[i].username, username) == 0) {
            if (records[i].endTime == 0) {
                totalDuration += difftime(currentTime, records[i].startTime);
            } else {
                totalDuration += records[i].studyDuration;
            }
        }
    }
    return totalDuration;
}

// Thank you for your reading.
// Copyright reserved by ECM2209270.