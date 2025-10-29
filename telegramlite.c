#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define MAX_MSG 100
#define MAX_LEN 200

struct Message {
    char sender[50];
    char text[MAX_LEN];
    char time[30];
};

void encrypt(char *text) {
    for (int i = 0; text[i]; i++) text[i] ^= 15;
}

void decrypt(char *text) {
    for (int i = 0; text[i]; i++) text[i] ^= 15;
}

void getTime(char *buffer) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, 30, "%I:%M %p", t);
}

void saveMessage(struct Message msg) {
    FILE *f = fopen("messages.db", "ab");
    encrypt(msg.text);
    fwrite(&msg, sizeof(struct Message), 1, f);
    fclose(f);
}

void showMessages() {
    struct Message msg;
    FILE *f = fopen("messages.db", "rb");
    if (!f) {
        printf("\nNo messages yet!\n");
        return;
    }
    printf("\n--- Chat History ---\n");
    while (fread(&msg, sizeof(struct Message), 1, f)) {
        decrypt(msg.text);
        printf("[%s] %s: %s\n", msg.time, msg.sender, msg.text);
    }
    fclose(f);
    printf("---------------------\n");
}

int main() {
    struct Message msg;
    char choice[5];
    printf("Welcome to Telegram Lite!\nEnter your name: ");
    fgets(msg.sender, sizeof(msg.sender), stdin);
    msg.sender[strcspn(msg.sender, "\n")] = 0;

    while (1) {
        printf("\n1. Send a message\n2. View chat history\n3. Exit\n> ");
        fgets(choice, sizeof(choice), stdin);
        if (choice[0] == '1') {
            printf("Enter message: ");
            fgets(msg.text, sizeof(msg.text), stdin);
            msg.text[strcspn(msg.text, "\n")] = 0;
            getTime(msg.time);
            saveMessage(msg);
            printf("[Message encrypted and saved]\n");
        } else if (choice[0] == '2') {
            showMessages();
        } else if (choice[0] == '3') {
            printf("Goodbye!\n");
            break;
        } else {
            printf("Invalid option!\n");
        }
    }
    return 0;
}
