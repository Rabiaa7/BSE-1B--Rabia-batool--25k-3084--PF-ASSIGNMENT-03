//25K-3084 (book shelf management)
#include <stdio.h>
#include <string.h>

typedef struct {
    int id;
    int popularity;
    int lastAccess; 
} Book;

int main() {
    int capacity, Q;
    scanf("%d %d", &capacity, &Q);

    Book shelf[capacity]; 
    int size = 0;         
    int timestamp = 0;    

    for (int i = 0; i < capacity; i++) {
        shelf[i].id = -1; 
    }

    for (int q = 0; q < Q; q++) {
        char op[10];
        scanf("%s", op);

        if (strcmp(op, "ADD") == 0) {
            int x, y;
            scanf("%d %d", &x, &y);
            timestamp++;

            int found = 0;
            for (int i = 0; i < size; i++) {
                if (shelf[i].id == x) {
                    shelf[i].popularity = y;
                    shelf[i].lastAccess = timestamp;
                    found = 1;
                    break;
                }
            }

            if (!found) {
                if (size < capacity) {
                    shelf[size].id = x;
                    shelf[size].popularity = y;
                    shelf[size].lastAccess = timestamp;
                    size++;
                } else {
                    int lraIndex = 0;
                    for (int i = 1; i < size; i++) {
                        if (shelf[i].lastAccess < shelf[lraIndex].lastAccess) {
                            lraIndex = i;
                        }
                    }
                    shelf[lraIndex].id = x;
                    shelf[lraIndex].popularity = y;
                    shelf[lraIndex].lastAccess = timestamp;
                }
            }

        } else if (strcmp(op, "ACCESS") == 0) {
            int x;
            scanf("%d", &x);
            timestamp++;

            int found = 0;
            for (int i = 0; i < size; i++) {
                if (shelf[i].id == x) {
                    printf("%d\n", shelf[i].popularity);
                    shelf[i].lastAccess = timestamp;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                printf("-1\n");
            }
        }
    }

    return 0;
}
