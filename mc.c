#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

FILE *ifp;
FILE *ofp;
FILE *openingFP;
FILE *closingFP;

// AddOpeningBrackets
void detectColonAddBracket();
void addOpeningBrackets();
void addClosingBrackets();
int getIndentLevel();
int getIndentDifference();
int closeCurlyBracket();
int detectBlankLine();
void printIndentIndexes();
// TerminateLines
void terminateLines(void);
int getLastCharacter(void);

char * line = NULL;
char * nLine[500];
size_t len = 0;
ssize_t read;
int inLevel;
int colonDetected;
int lineLength;
int *indentIndexes[100] = {-1};
int indentLevel = 0;
int indentDifference;
int curIndentLevel = 0;
int prevIndentLevel = 0;
int lineLength;
int indexFlag = 0;
int nextIndentLevel = 0;
int curCount = 0;
int preCount = 0;


int main(int argc, char *argv[]) {
    addOpeningBrackets(argv);
    addClosingBrackets();

    terminateLines();
    if (line)
        free(line);
    return 0;
}

/////////////////////////////////////////////////////
void detectColonAddBracket() {
    int lineLength = strlen(line);
    for (int i = lineLength; i > 0; --i) {
        char ch = line[i];
        if (ch == '\n' || ch == ' ' || ch == '\0')
            continue;
        // LOOKINTO
        // For some reason when converting to metaC "cd == ':'" doesn't work
        // only "ch == 58" works :(
        else if (ch == 58){
            line[i] = '{';
        }
    }
}

/////////////////////////////////////////////////////
void addOpeningBrackets(char *argv[]) {
    printf("argv[1]: %s\n", argv[1]);
    openingFP = fopen(argv[1], "r");
    if (openingFP == NULL) {
        printf("Could not open file for reading\n");
        exit(EXIT_FAILURE);
    }
    closingFP = fopen("test.mmc", "w");
    if (closingFP == NULL) {
        printf("Could not open file for writing\n");
        exit(EXIT_FAILURE);
    }

    int nextIndentLevel = 0;
    while ((read = getline(&line, &len, openingFP)) != -1) {
        curIndentLevel = getIndentLevel();
        if (detectBlankLine()) {
            continue;
        }
        detectColonAddBracket();
        fputs(line, closingFP);
        prevIndentLevel = curIndentLevel;
    }
    fclose(openingFP);
    fclose(closingFP);
}

/////////////////////////////////////////////////////
void addClosingBrackets() {
    memset(indentIndexes, 0, sizeof indentIndexes);
    curIndentLevel = 0;
    prevIndentLevel = 0;
    nextIndentLevel = 0;
    ifp = fopen("test.mmc", "r");
    if (ifp == NULL)
        exit(EXIT_FAILURE);
    ofp = fopen("test.mc", "w");
    if (ofp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, ifp)) != -1) {
        curIndentLevel = getIndentLevel();
        if (curIndentLevel < prevIndentLevel) {
            indentDifference = getIndentDifference();
            for (int i=0; i < indentDifference; i++)
                fputs("}\n", ofp);
            fputs(line, ofp);
        }
        else if (curIndentLevel > prevIndentLevel) {
            indentIndexes[nextIndentLevel] = curIndentLevel;
            nextIndentLevel++;
            fputs(line, ofp);
        }
        else
            fputs(line, ofp);
        prevIndentLevel = curIndentLevel;
    }

    if (prevIndentLevel != 0) {
        curIndentLevel = 0;
        indentDifference = getIndentDifference();
        for (int i=0; i < indentDifference; i++)
            fputs("}\n", ofp);
    }

    fclose(ofp);
    fclose(ifp);
}
/////////////////////////////////////////////////////
// For how many loops we're closing
int getIndentDifference() {
    int indexing_size = sizeof indentIndexes / sizeof *indentIndexes;
    curCount = 0;
    preCount = 0;
    if (curIndentLevel > 1) {
        for (int i = 0; i < indexing_size; i++) {
            curCount++;
            if (indentIndexes[i] == curIndentLevel) 
                break;
        }
    }
    nextIndentLevel = (curCount );
    for (int i = curCount; i < indexing_size; i++) {
        if (indentIndexes[i] == 0) {
            break;
        }
        preCount++;
        indentIndexes[i] = 0;      // Erases the now unneeded indexes
    }
    
    return preCount;
}

/////////////////////////////////////////////////////
int getIndentLevel() {
    int length = strlen(line);
    int indentLength = 0;
    int count = 0;
    for (int i = 0; i < length; i++) {
        if (line[i] == ' ') {
            indentLength++;
            count++;
        }
        else
            break;
    }
    return indentLength;
}

/////////////////////////////////////////////////////
int detectBlankLine() {
    for (int i = 0; i < strlen(line) ; i++) {
        if (!(isspace(line[i])) && !(line[i] == '\n')) {
                return 0;
        }
    }
    return 1;
}
/////////////////////////////////////////////////////
void printIndentIndexes() {
    int array_size = sizeof indentIndexes / sizeof *indentIndexes;
    for (int i = 0; i < array_size; i++) {
        if (indentIndexes[i] != 0)
            printf("indexIndexs[%d] = %d\n", i, indentIndexes[i]);
    }
}

/////////////////////////////////////////////////////
void terminateLines(void) {
    /////////////////////////////////////////////////////
    // Terminate lines with semicolon
    /////////////////////////////////////////////////////
    ifp = fopen("test.mc", "r");
    if (ifp == NULL)
        exit(EXIT_FAILURE);
    ofp = fopen("test.c", "w");
    if (ifp == NULL)
        exit(EXIT_FAILURE);
    int lineLength;
    int i;
    char ch;
    char lastCharacter;
    while ((read = getline(&line, &len, ifp)) != -1) {
        printf("%s", line);
        lastCharacter = getLastCharacter();
        /* printf("BEFORE lastCharacter: %c\n", lastCharacter); */
        /* if ((lastCharacter == '}') || (lastCharacter == '>') || (lastCharacter == '{')) { */
        if (lastCharacter == '{') {
            printf("lastCharacter: %c\n", lastCharacter);
            fputs(line, ofp);
            continue;
        } 
        else if (lastCharacter == '>') {
            printf("lastCharacter: %c\n", lastCharacter);
            fputs(line, ofp);
            continue;
        }
        else if (lastCharacter == '}') {
            printf("lastCharacter: %c\n", lastCharacter);
            fputs(line, ofp);
            continue;
        }
        ////////////////////////////////////////////////////////////
        /* printf("if ((lastCharacter == '}') || (lastCharacter == '>') || (lastCharacter == '{')) {\n"); */
        /* printf("lastCharacter: %c\n", lastCharacter); */
        /* printf("continue;\n"); */
        /* printf("}\n"); */
        ////////////////////////////////////////////////////////////
        /* lineLength = sizeof indentIndexes / sizeof *indentIndexes; */
        lineLength = strlen(line);
        line[lineLength-1] = ';';
        line[lineLength] = '\n';
        line[lineLength+1] = '\0';
        fputs(line, ofp);
    }
}


/////////////////////////////////////////////////////
int getLastCharacter() {
    int array_size = sizeof indentIndexes / sizeof *indentIndexes;
    char lastCharacter;
    char prevLastCharacter = line[0];
    for (int i = 0; i < array_size; i++) {
        lastCharacter = line[i];
        if (lastCharacter == '\n') {
            return prevLastCharacter;
        }
        prevLastCharacter = lastCharacter;
    }
    return prevLastCharacter;
}
/////////////////////////////////////////////////////
