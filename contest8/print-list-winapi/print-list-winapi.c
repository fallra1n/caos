#include "windows.h"
#include "tchar.h"
#include <inttypes.h>
#include <stdio.h>

struct Item {
  int value;
  uint32_t next_pointer;
};


int main(int argc, char* argv[]) {
    if (argc < 2) {
        return 1;
    } 

    // LPSTR - char*
    LPSTR first = argv[1];

    // DWORD - uint32
    // INVALID_HANDLE_VALUE - ret val of CreateFile if error
    // HANDLE - type of ret val of CreateFile
    DWORD return_value = 0;
    HANDLE read_fd = INVALID_HANDLE_VALUE;

    read_fd = CreateFile(first, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (read_fd == INVALID_HANDLE_VALUE) {
        return_value = 1;
        goto Exit;
    }
    
    struct Item item;
    

    do {
        DWORD read_count = 0;

        // 0 - !FILE_FLAG_OVERLAPPED
        BOOL ok = ReadFile(read_fd, &item, sizeof(item), &read_count, 0);
        if (!ok) {
            return_value = 1;
            goto Exit;
        }

        if (read_count == 0) {
            goto Exit;
        }

        printf("%d\n", item.value);

        // cast int -> large_int
        LARGE_INTEGER offset;
        offset.QuadPart = item.next_pointer;

        // NULL - var for new ptr of file
        // 0 - SEEK_SET(file begin)
        BOOL offset_ok = SetFilePointerEx(read_fd, offset, NULL, 0);
        if (!offset_ok) {
            return_value = 1;
            goto Exit;
        }

    } while (item.next_pointer != 0);

Exit:
    CloseHandle(read_fd);

    return return_value;
}