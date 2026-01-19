#include "engine/score_ladder.h"
#include <stdlib.h>

typedef struct _sl {
    f32 scores[5];
} ScoreLadder;

ScoreLadder ladder = {};

i32 _sl_sort(const void *_a, const void *_b) {
    const f32 *a = (f32 const *)_a;
    const f32 *b = (f32 const *)_b;
    return a > b ? -1 : 1;
}

void ScoreLadderSubmit(f32 score) {
    qsort(ladder.scores, 5, sizeof(ladder.scores[0]), _sl_sort);
    for (i32 i = 0; i < 5; i++) {
		f32 current = ladder.scores[i];
		if (current < score) {

		}
    }
}

void ScoreLadderGetScores(f32 out[], u32 *count) {
	*count = 5;
	for (u32 i = 0; i < *count; i++) {
		out[i] = ladder.scores[i];
	}
}
