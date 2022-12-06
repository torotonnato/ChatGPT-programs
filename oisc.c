/*
write a small C function that accept an array "disp" of integers
and returns an array of bytes. The function will output valid x86
instructions that codify a jump-if-not-zero to each value in "disp"
*/

#include <stdio.h>
#include <stdint.h>

// jnz_to_disp_32 generates a jnz (jump-if-not-zero) instruction that jumps
// to the specified 32-bit displacement.
uint8_t *jnz_to_disp_32(int32_t disp) {
    // The jnz instruction is 0F 85 /disp32 in x86.
    // The displacement value /disp32 is a 32-bit signed integer
    // that specifies the target address of the jump relative to
    // the address of the instruction following the jnz instruction.
    // For example, a displacement value of 0x00000004 means that
    // the target address is 4 bytes after the instruction following
    // the jnz instruction.
    static uint8_t jnz[] = { 0x0F, 0x85, 0x00, 0x00, 0x00, 0x00 };
    *(int32_t *)(jnz + 2) = disp;
    return jnz;
}

// gen_jnz_to_disps generates a sequence of jnz instructions that jump
// to the specified 32-bit displacements. The resulting instructions are
// stored in the array pointed to by "buf". The function returns the number
// of bytes written to "buf".
int gen_jnz_to_disps(int32_t *disps, int num_disps, uint8_t *buf) {
    int i;
    int offset = 0;

    // Generate a jnz instruction for each displacement in "disps"
    // and write it to the output buffer.
    for (i = 0; i < num_disps; i++) {
        int32_t disp = disps[i];
        uint8_t *jnz = jnz_to_disp_32(disp);
        int jnz_size = sizeof(jnz);
        memcpy(buf + offset, jnz, jnz_size);
        offset += jnz_size;
    }

    return offset;
}

int main() {
    int32_t disps[] = { 0x00000004, 0xFFFFFFFC, 0x00000001 };
    int num_disps = sizeof(disps) / sizeof(int32_t);

    // Allocate an output buffer for the generated instructions.
    // The maximum size of the output buffer is 6 bytes per jnz
    // instruction * the number of displacements.
    uint8_t buf[6 * num_disps];

    // Generate the jnz instructions and write them to the output buffer.
    int num_bytes = gen_jnz_to_disps(disps, num_disps, buf);

    // Print the generated instructions.
    int i;
    for (i = 0; i < num_bytes; i++) {
        printf("%02X ", buf[i]);
    }
    printf("\n");

    return 0;
}
