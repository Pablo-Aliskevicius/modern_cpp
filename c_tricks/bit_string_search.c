#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

/*
Shift-And / Shift-Or Algorithm (Bit-Parallel String Matching)
=============================================================

The Problem: 
------------
Searching for a pattern string inside a larger text usually requires 
complex state machines or algorithms like Knuth-Morris-Pratt (KMP).

The Trick: 
----------
For short patterns (fitting within a machine word, e.g., 32 or 64 bits), 
you can represent the characters of the pattern as bits and perform 
string matching entirely in parallel using bitwise shifts and bitwise 
OR/AND operations across an entire CPU word at once.

There are other string search algorithms, don't use in production before
benchmarking for you rown use case.

Output:
------- 

Correctness test passed: both searches returned index 2851.
Naive Search Time:          5.1860 seconds
Shift-And Search Time:      4.2410 seconds

Conclusion:
-----------
Probably not worth the effort for short texts (less than a book), but a fun 
exercise in bit manipulation and benchmarking.
*/

#define ITERATIONS 2000000

// Naive substring search (strstr might be optimized)
int naive_search(const char *text, int text_len, const char *pattern, int pat_len) {
    for (int i = 0; i <= text_len - pat_len; i++) {
        int j;
        for (j = 0; j < pat_len; j++) {
            if (text[i + j] != pattern[j]) break;
        }
        if (j == pat_len) return i;
    }
    return -1;
}

// Bit-parallel Shift-And algorithm
int shift_and_search(const char *text, int text_len, const char *pattern, int pat_len) {
    if (pat_len > 64 || pat_len == 0) return -1;

    // Build a bitmask for each character in the pattern
    // If a character appears several times, its bitmask will 
    // have multiple bits set.
    uint64_t pattern_mask[256] = {0};
    
    for (int i = 0; i < pat_len; i++) {
        pattern_mask[(unsigned char)pattern[i]] |= (1ULL << i);
    }

    uint64_t state = 0;
    uint64_t match_bit = (1ULL << (pat_len - 1));

    for (int i = 0; i < text_len; i++) {
        state = ((state << 1) | 1ULL) & pattern_mask[(unsigned char)text[i]];
        if (state & match_bit) {
            return i - pat_len + 1;
        }
    }
    return -1;
}

int main() {
    const char *text =
        "A quiet workshop stood beside the river, where the afternoon light moved slowly across the wooden tables. "
        "Every drawer held a collection of small tools, carefully arranged after years of patient work. "
        "Visitors often paused near the window to watch the water carry leaves beneath the old stone bridge. "
        "al The room smelled faintly of cedar, machine oil, and the paper notes pinned along the far wall. "
        "go On rainy days, the steady sound on the roof made it easier to focus on complicated problems. "
        "ri A brass clock marked each hour while the benches remained busy with sketches, books, and unfinished plans. "
        "th Some projects needed only a few careful adjustments, while others required many attempts and fresh ideas. "
        "ms The shelves held jars of screws, coils of wire, pieces of glass, and boxes filled with spare parts. "
        "A notebook on the central desk recorded measurements, questions, and useful observations from each test. "
        "Outside, clouds passed over the hills, and the river reflected a changing pattern of silver and blue. "
        "A visitor once brought a broken music box and described how its tune stopped after the third note. "
        "The mechanic opened the case, examined the gears, and found a tiny spring bent out of alignment. "
        "With a magnifier and a pair of narrow pliers, the spring was returned to its original position. "
        "The box played its full melody again, filling the workshop with a bright and delicate sound. "
        "That small repair reminded everyone that careful observation can matter more than complicated tools. "
        "Near the door, a large map showed footpaths, villages, and the steep ridge beyond the valley. "
        "The map had been updated over many seasons, with new trails drawn in ink beside older routes. "
        "People used it to plan weekend walks and to remember places they hoped to visit another time. "
        "A stack of letters nearby carried questions from students learning how everyday objects are made. "
        "The workers answered each letter with a sketch, a clear explanation, and an invitation to experiment. "
        "Whenever a new idea seemed difficult, they divided it into smaller tasks and tested each part. "
        "Results were written down in plain language so that another person could repeat the same steps. "
        "This habit made the workshop welcoming to beginners as well as people with years of experience. "
        "At the end of the week, everyone gathered to share what had worked and what still needed attention. "
        "The conversation moved from woodworking to electronics, from careful measurements to creative design. "
        "No single method solved every problem, so the group compared approaches and chose the most suitable one. "
        "A fresh page was opened in the notebook, ready for the next question and the next careful experiment. "
        "As evening approached, the workers tidied the benches and put every instrument back in its proper place. "
        "Before leaving, one of them reviewed the latest notes and found the word algorithms near the end.";
    int text_len = (int)strlen(text);
    const char *pattern = "algorithms";
    int pat_len = (int)strlen(pattern);

    int naive_result = naive_search(text, text_len, pattern, pat_len);
    int shift_and_result = shift_and_search(text, text_len, pattern, pat_len);
    if (naive_result != shift_and_result) {
        fprintf(stderr,
                "Correctness test failed: naive search returned %d, Shift-And returned %d\n",
                naive_result, shift_and_result);
        return 1;
    }
    printf("Correctness test passed: both searches returned index %d.\n", naive_result);

    volatile int dummy = 0;
    clock_t start, end;

    start = clock();
    for (int i = 0; i < ITERATIONS; i++) {
        dummy += naive_search(text, text_len, pattern, pat_len);
    }
    end = clock();
    printf("Naive Search Time:          %.4f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    start = clock();
    for (int i = 0; i < ITERATIONS; i++) {
        dummy += shift_and_search(text, text_len, pattern, pat_len);
    }
    end = clock();
    printf("Shift-And Search Time:      %.4f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    if (dummy == 0) printf("");
    return 0;
}