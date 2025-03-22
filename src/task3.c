#include "task3.h"
char** split_into_blocks_and_pad(
    char* data, size_t blocks_size, size_t size_student
) {
    // 14bytes 14b 14b 11b
    char** blocks = calloc(4, sizeof(char*));

    // printf("%d", blocks_size);
    for (int i = 0; i < 3; i++) {
        blocks[i] = calloc(blocks_size, sizeof(char));
        memcpy(blocks[i], data + i * blocks_size, blocks_size);
    }
    blocks[3] = calloc(blocks_size, sizeof(char));
    if ((size_student) % blocks_size)
        memcpy(blocks[3], data + 3 * blocks_size, (size_student) % blocks_size);
    else

        memcpy(blocks[3], data + 3 * blocks_size, blocks_size);
    // printf("cv");
    // for (int i = 0; i < blocks_size; i++)
    // S  printf("%x ", blocks[0][i]);
    return blocks;
}
char* XOR(char* block1, char* block2, size_t block1_size, size_t block2_size) {
    for (size_t i = 0; i < block1_size; i++)
        block1[i] = (char) (block1[i] ^ block2[i % block2_size]);
    return block1;
}
char* P_BOX(char* block, size_t blocks_size) {
    char* copy = calloc(blocks_size, sizeof(char));
    for (size_t i = 0; i < blocks_size; i++)
        copy[(i * (blocks_size - 1) + 2) % blocks_size] = block[i];
    free(block);
    return copy;
}
// 00000000 00000000 00000000 00000000
void cripteaza_studenti(
    secretariat* secretariat, void* key, size_t key_len, void* iv,
    size_t iv_len, char* cale_output
) {
    // TODO(student): Cripteaza vectorul secretariat->studenti si scrie
    // rezultatul in fisierul cale_output
    size_t blocks_size = (secretariat->nr_studenti * sizeof(student)) / 4;
    if (secretariat->nr_studenti * sizeof(student) % 4)
        blocks_size++;
    char* data = (char*) secretariat->studenti;
    size_t size_student = sizeof(student) * secretariat->nr_studenti;
    char** blocks = split_into_blocks_and_pad(data, blocks_size, size_student);
    blocks[0] = XOR(blocks[0], iv, blocks_size, iv_len);
    blocks[0] = XOR(blocks[0], key, blocks_size, key_len);
    blocks[0] = P_BOX(blocks[0], blocks_size);
    for (int i = 1; i <= 3; i++) {
        blocks[i] = XOR(blocks[i], blocks[i - 1], blocks_size, blocks_size);
        blocks[i] = XOR(blocks[i], key, blocks_size, key_len);
        blocks[i] = P_BOX(blocks[i], blocks_size);
    }
    FILE* f = fopen(cale_output, "wb");
    for (int i = 0; i < 4; i++)
        for (size_t j = 0; j < blocks_size; j++)
            fputc(blocks[i][j], f);
}
