#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

// Simple IPC Client Example
// Connects to the compositor's UNIX socket and sends a command.

int main() {
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "/run/user/1000/halo-ipc.sock", sizeof(addr.sun_path) - 1);

    // This will fail in this example because the compositor isn't running,
    // but demonstrates the architecture.
    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect");
        close(sock);
        return EXIT_FAILURE;
    }

    // TLV encoded message: Type 0x01 (COMMAND), Length 16, Value "workspace next"
    char buffer[256] = { 0x01, 16, 0, 0 }; 
    strcpy(buffer + 4, "workspace next");

    if (send(sock, buffer, 4 + 16, 0) == -1) {
        perror("send");
    }

    printf("Command sent to compositor.\n");

    close(sock);
    return EXIT_SUCCESS;
}
