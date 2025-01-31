#ifndef LD_H
# define LD_H

# include <stdio.h>

typedef struct {

} LDState;

int coff_output(LDState *state, FILE *fp);
int coff_load(LDState *state, FILE *fp);

#endif /* !LD_H */
