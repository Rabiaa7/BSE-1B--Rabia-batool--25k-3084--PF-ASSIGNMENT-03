//25k-3084 (ACM/IEEE membership)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int studentID;         
    char fullName[100];
    char batch[50];         
    char membershipType[10]; 
    char registrationDate[11]; 
    char dateOfBirth[11];     
    char interest[10];       
} Student;

typedef struct {
    Student *students;
    size_t size;     
    size_t capacity; 
} StudentDB;

#define INITIAL_CAPACITY 10
#define GROWTH_FACTOR 2

void initDB(StudentDB *db) {
    db->students = malloc(INITIAL_CAPACITY * sizeof(Student));
    if (!db->students) {
        perror("Failed to allocate memory for student database");
        exit(EXIT_FAILURE);
    }
    db->size = 0;
    db->capacity = INITIAL_CAPACITY;
}

void freeDB(StudentDB *db) {
    free(db->students);
    db->students = NULL;
    db->size = 0;
    db->capacity = 0;
}

void ensureCapacity(StudentDB *db) {
    if (db->size < db->capacity) return;
    size_t newCap = db->capacity * GROWTH_FACTOR;
    Student *tmp = realloc(db->students, newCap * sizeof(Student));
    if (!tmp) {
        perror("Failed to reallocate memory for student database");
        exit(EXIT_FAILURE);
    }
    db->students = tmp;
    db->capacity = newCap;
}

int loadDatabase(const char *filename, StudentDB *db) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        if (errno == ENOENT) return 0; 
        perror("Failed to open file for reading");
        return -1;
    }

    Student s;
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        ensureCapacity(db);
        db->students[db->size++] = s;
    }

    if (ferror(fp)) {
        perror("Error reading file");
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return 0;
}

int saveDatabase(const char *filename, StudentDB *db) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("Failed to open file for writing");
        return -1;
    }

    if (fwrite(db->students, sizeof(Student), db->size, fp) != db->size) {
        perror("Failed to write all records");
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return 0;
}

int addStudent(StudentDB *db, Student s) {
    // check duplicate ID
    for (size_t i = 0; i < db->size; i++) {
        if (db->students[i].studentID == s.studentID) {
            printf("Student ID %d already exists!\n", s.studentID);
            return -1;
        }
    }
    ensureCapacity(db);
    db->students[db->size++] = s;
    return 0;
}

int updateStudent(StudentDB *db, int studentID) {
    for (size_t i = 0; i < db->size; i++) {
        if (db->students[i].studentID == studentID) {
            printf("Current Batch: %s, Membership: %s\n", db->students[i].batch, db->students[i].membershipType);
            printf("Enter new Batch (CS/SE/Cyber Security/AI): ");
            scanf(" %[^\n]", db->students[i].batch);
            printf("Enter new Membership Type (IEEE/ACM): ");
            scanf(" %[^\n]", db->students[i].membershipType);
            printf("Updated successfully.\n");
            return 0;
        }
    }
    printf("Student ID %d not found.\n", studentID);
    return -1;
}

int deleteStudent(StudentDB *db, int studentID) {
    size_t i;
    for (i = 0; i < db->size; i++) {
        if (db->students[i].studentID == studentID) break;
    }
    if (i == db->size) {
        printf("Student ID %d not found.\n", studentID);
        return -1;
    }
    // shift remaining
    for (size_t j = i; j < db->size - 1; j++) {
        db->students[j] = db->students[j + 1];
    }
    db->size--;
    printf("Student ID %d deleted.\n", studentID);
    return 0;
}

void displayAllStudents(StudentDB *db) {
    printf("\n--- All Students (%zu) ---\n", db->size);
    for (size_t i = 0; i < db->size; i++) {
        Student s = db->students[i];
        printf("ID: %d, Name: %s, Batch: %s, Membership: %s, Reg: %s, DOB: %s, Interest: %s\n",
               s.studentID, s.fullName, s.batch, s.membershipType, s.registrationDate, s.dateOfBirth, s.interest);
    }
}

void batchReport(StudentDB *db) {
    char batch[50], membership[10];
    printf("Enter Batch (CS/SE/Cyber Security/AI): ");
    scanf(" %[^\n]", batch);
    printf("Enter Membership Filter (IEEE/ACM/Both): ");
    scanf(" %[^\n]", membership);

    printf("\n--- Batch Report ---\n");
    for (size_t i = 0; i < db->size; i++) {
        Student s = db->students[i];
        if (strcmp(s.batch, batch) == 0 && (strcmp(s.interest, membership) == 0 || strcmp(membership, "Both") == 0)) {
            printf("ID: %d, Name: %s, Membership: %s, Interest: %s\n", s.studentID, s.fullName, s.membershipType, s.interest);
        }
    }
}

/* ------------------ Menu ------------------ */
void menu(StudentDB *db, const char *filename) {
    int choice;
    while (1) {
        printf("\n1. Register Student\n2. Update Student\n3. Delete Student\n4. View All\n5. Batch Report\n6. Exit\nChoice: ");
        if (scanf("%d", &choice) != 1) { while (getchar()!='\n'); continue; }
        if (choice == 1) {
            Student s;
            printf("Enter Student ID: "); scanf("%d", &s.studentID); while(getchar()!='\n');
            printf("Full Name: "); scanf(" %[^\n]", s.fullName);
            printf("Batch (CS/SE/Cyber Security/AI): "); scanf(" %[^\n]", s.batch);
            printf("Membership Type (IEEE/ACM): "); scanf(" %[^\n]", s.membershipType);
            printf("Registration Date (YYYY-MM-DD): "); scanf(" %[^\n]", s.registrationDate);
            printf("Date of Birth (YYYY-MM-DD): "); scanf(" %[^\n]", s.dateOfBirth);
            printf("Interest (IEEE/ACM/Both): "); scanf(" %[^\n]", s.interest);
            if (addStudent(db, s) == 0) saveDatabase(filename, db);
        } else if (choice == 2) {
            int id; printf("Enter Student ID to update: "); scanf("%d", &id); while(getchar()!='\n');
            if (updateStudent(db, id) == 0) saveDatabase(filename, db);
        } else if (choice == 3) {
            int id; printf("Enter Student ID to delete: "); scanf("%d", &id); while(getchar()!='\n');
            if (deleteStudent(db, id) == 0) saveDatabase(filename, db);
        } else if (choice == 4) {
            displayAllStudents(db);
        } else if (choice == 5) {
            batchReport(db);
        } else if (choice == 6) {
            saveDatabase(filename, db);
            printf("Exiting...\n");
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }
}

int main() {
    const char *filename = "members.dat";
    StudentDB db;
    initDB(&db);
    
    loadDatabase(filename, &db);

    menu(&db, filename);

    freeDB(&db);
    return 0;
}
