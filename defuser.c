#include "defuser.h"

#include <inttypes.h>
#include <stdio.h>

#define MAX_FIELDS 8

/*
 * 把 values 中的 count 个整数写成一行“空格分隔的无符号十进制整数”，存入 out。
 * 成功返回 0；count 为 0（还没有实现）、snprintf 出错或结果被截断时返回非零。
 */
static int write_answer(char *out, size_t cap, const uint32_t *values, size_t count) {
    size_t used = 0;
    for (size_t i = 0; i < count; ++i) {
        int size = snprintf(out + used, cap - used, "%s%" PRIu32, i ? " " : "", values[i]);
        if (size < 0 || (size_t)size >= cap - used) return 1;
        used += (size_t)size;
    }
    return count == 0;
}

int solve_phase_1(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    uint32_t lo = (uint32_t)challenge;
    uint32_t hi = (uint32_t)(challenge >> 32);
    uint32_t mid = (uint32_t)(challenge >> 16);

    uint32_t t1 = (mid ^ UINT32_C(0x86ee)) + UINT32_C(4) * hi;
    uint32_t t0 = (UINT32_C(3) * lo + hi) ^ UINT32_C(0x40e7);

    values[0] = (t1 + UINT32_C(0x3750)) ^ t0;
    count = 1;
    return write_answer(out, cap, values, count);
}

int solve_phase_2(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    values[0] = 3u * (uint32_t)((challenge >> 38) & 0xfu);
    for (size_t i = 1; i < 6; ++i) {
        values[i] = ((2u * values[i - 1]) ^ (uint32_t)((challenge >> (3 * i + 1)) & 0x1fu)) + 11u * i;
    }
    count = 6;
    return write_answer(out, cap, values, count);
}

int solve_phase_3(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    uint32_t x = (uint32_t)(challenge >> 32) ^ (uint32_t)(challenge >> 3);
    values[0] = (uint32_t)(challenge & 7u);

    switch (values[0]) {
        case 0: values[1] = 5u * x + 0xaeb8u; break;
        case 1: values[1] = (4u * x) ^ 0xa0e7u; break;
        case 2: values[1] = 3u * x + 0x9c31u; break;
        case 3: values[1] = 5u * x + 0x914bu; break;
        case 4: values[1] = (x ^ 0x9feau) + (x >> 2); break;
        case 5: values[1] = (x ^ 0x681au) + (x >> 1); break;
        case 6: values[1] = (2u * x) ^ 0x1d21u; break;
        case 7: values[1] = 5u * x + 0x164aeu; break;
    }

    count = 2;
    return write_answer(out, cap, values, count);
}

int solve_phase_4(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    uint32_t target = (uint32_t)(challenge & 0x7fu);
    uint32_t salt = (uint32_t)(challenge >> 7) ^ (uint32_t)(challenge >> 32);
    uint32_t low = 0, high = 127;
    uint32_t path[8];
    size_t depth = 0;

    while (1) {
        uint32_t mid = low + ((high - low) >> 1);
        if (target == mid) {
            values[0] = (mid + 0xdbu) ^ salt;
            break;
        }
        if (target < mid) {
            path[depth++] = (mid + salt) ^ 0x1fu;
            high = mid - 1;
        } else {
            path[depth++] = (mid + salt) ^ 0x6u;
            low = mid + 1;
        }
        }

while (depth > 0) {
    values[0] = (3u * values[0]) ^ path[--depth];
}
count = 1;
    return write_answer(out, cap, values, count);
}

int solve_phase_5(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;

    const uint32_t tags[] = {
    0x056cu, 0x0415u, 0x087fu, 0x0347u, 0x0946u, 0x0724u,
    0x0b86u, 0x0a40u, 0x027au, 0x015au, 0x0676u
    };
    const uint32_t weights[] = {
    0x482du, 0xbfc0u, 0xbacau, 0x3fbeu, 0x75a5u, 0x3dd7u,
    0x8b44u, 0x9112u, 0xd7feu, 0x5079u, 0xfac0u
    };
    uint32_t index = (uint32_t)(challenge & 7u);
    uint32_t hops = (uint32_t)((challenge >> 3) & 3u);
    uint32_t acc = (uint32_t)(challenge >> 6) ^ (uint32_t)(challenge >> 32) ^ 0xcd6cu;

    for (uint32_t i = 0; i <= hops; ++i) {
        acc = (3u * acc + weights[index + i]) ^ tags[index + i];
    }
    values[0] = acc;
    values[1] = tags[index + hops];
    count = 2;
    
    return write_answer(out, cap, values, count);
}

int solve_phase_6(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[MAX_FIELDS] = {0};
    size_t count = 0;
    (void)challenge;
    uint32_t x = (uint32_t)(challenge >> 6) ^ (uint32_t)(challenge >> 32);

    for (uint32_t k = 0; k < 2; ++k) {
    uint32_t index = (uint32_t)((challenge >> (3u * k)) & 7u);

    switch (index) {
        case 0:
            x = (x ^ (x >> 3)) + 0xc05bu;
            break;
        case 1:
        case 2:
        case 4:
            x = (x + 0x9384u) ^ (x >> 7);
            break;
        case 3:
            x = (x ^ 0x3827u) + 8u * x;
            break;
        case 5:
            x = (4u * x + 0x4112u) ^ (5u * x);
            break;
        case 6:
            x = (x >> 2) | (x << 30);
            break;
        case 7:
            x = ((x << 7) | (x >> 25)) ^ 0x1135u;
            break;
    }

    if (k == 0) {
        x ^= 0xfe90u;
    }
}

values[0] = x + 0x35d0u;
count = 1;

    return write_answer(out, cap, values, count);
}

int solve_secret(uint64_t challenge, uint64_t context, char *out, size_t cap) {
    (void)challenge;
    (void)context;
    (void)out;
    (void)cap;
    // TODO（选做）: 根据 secret_gate、secret_copy 和 secret_phase 的汇编构造完整的第七行，写入 out 后返回 0。
    // 第七行不一定只有整数，可以直接用 snprintf 写入；还没完成时保持返回 1，测试会显示 not attempted。
    return 1;
}
