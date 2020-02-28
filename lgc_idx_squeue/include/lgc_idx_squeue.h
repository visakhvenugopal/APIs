
#ifndef LGC_IDX_SQUEUE_H
#define LGC_IDX_SQUEUE_H

int lgc_idx_squeue_init(int);
int lgc_idx_squeue_destroy();
int lgc_idx_squeue_insert(int, const void *, const int);
int lgc_idx_squeue_remove(int, void *, const int);

#endif
