#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readFile(int dept_prefs[5][5], int prog_prefs[5][5]) {
    char filename[] = "matching-data.txt";
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error: could not open file %s", filename);
        return;
    }

    int n = 0;
    char line[20];
    while (fgets(line, sizeof(line), file)) {
        char* token;
        int line_list[5];
        token = strtok(line, " ");
        for (int j = 0; j < 5; j++) {
            line_list[j] = atoi(token);
            token = strtok(NULL, " ");
        }
        if (n < 5) {
            for (int j = 0; j < 5; j++) {
                dept_prefs[j][n] = line_list[j];
            }
        } else {
            for (int j = 0; j < 5; j++) {
                prog_prefs[j][n-5] = line_list[j];
            }
        }
        n++;
    }

    fclose(file);
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void permute(int arr[], int start, int end, FILE *fp) {
    int results[120][5];
    if (start == end) {
        for (int i = 0; i <= end; i++) {
            fprintf(fp, "%d ", arr[i]);
        }
        fprintf(fp, "\n");
        return;
    }

    for (int i = start; i <= end; i++) {
        swap(&arr[start], &arr[i]);
        permute(arr, start+1, end, fp);
        swap(&arr[start], &arr[i]);
    }
}

void find_best_match(int prefA[5][5], int prefB[5][5], int perms[120][5], int results[5]) {
    //loop through each possible match set and score preferences both ways
    int min_score = 99999;
    for (int n=0; n < 120; n++) {
        int poss_match[5];
        for (int i = 0; i < 5; i++) {
            poss_match[i] = perms[n][i];
        }
        int score = 0;
        for (int i=0; i < 5; i++) {
            int a = i+1;
            int b = poss_match[i];
            //score prefA's rank
            int pref_a[5];
            for (int k = 0; k < 5; k++) {
                pref_a[k] = prefA[a-1][k];
            }
            for (int j=0; j < 5; j++) {
                if (pref_a[j] == b) {
                    score += j+1;
                }
            }
            //score prefB's rank
            int pref_b[5];
            for (int k = 0; k < 5; k++) {
                pref_b[k] = prefB[b-1][k];
            }
            for (int j=0; j < 5; j++) {
                if (pref_b[j] == a) {
                    score += j+1;
                }
            }
        }
        if (score < min_score) {
            min_score = score;
            for(int k=0;k<5;k++) {
                results[k] = poss_match[k];
            }
        }
    }
}


int main() {
    int dept_prefs[5][5];
    int prog_prefs[5][5];
    // read in preferences matrices
    readFile(dept_prefs, prog_prefs);

    //write out all permutations of possible matches
    FILE *fp;
    fp = fopen("perms.txt", "w");
    int arr[5] = {1,2,3,4,5};
    permute(arr, 0, 4, fp);
    fclose(fp);

    //read in permutations into array (couldn't figure out how to do this without writing/reading)
    FILE *fn;
    fn = fopen("perms.txt", "r");   
    int match_list[120][5];
    char line[20];
    int i = 0;
    while (fgets(line, sizeof(line), fn)) {
        char *token;
        token = strtok(line, " ");
        for (int j = 0; j < 5; j++) {
            match_list[i][j] = atoi(token);
            token = strtok(NULL, " ");
        }
        i++;
    }
    fclose(fn);
    
    //score all of the possible matches
    int best_match[5];
    find_best_match(dept_prefs, prog_prefs, match_list, best_match);

    //print out results

    printf("Programmers and their preferences\n");
    printf("%15s %15s %15s %15s %15s\n","Programmer #1","Programmer #2","Programmer #3","Programmer #4","Programmer #5");
    for (int i =0; i < 5; i++) {
        int l;
        for (int j =0; j < 5; j++) {
            l = prog_prefs[j][i];
            printf("%15i ",l);
        }
        printf("\n");
    }
    printf("\n");

    printf("Departments and their preferences\n");
    printf("%15s %15s %15s %15s %15s\n","Department #1","Department #2","Department #3","Department #4","Department #5");
    for (int i =0; i < 5; i++) {
        int l;
        for (int j =0; j < 5; j++) {
            l = dept_prefs[j][i];
            printf("%15i ",l);
        }
        printf("\n");
    }
    printf("\n");

    printf("RESULTS:\n");
    for (int i =0; i < 5; i++) {
        printf("Department #%i will get Programmer #%i\n", i+1, best_match[i]);
    }

    return 0;
}