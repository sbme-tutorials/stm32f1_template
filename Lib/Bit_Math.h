#ifndef BIT_OPERATIONS_H
#define BIT_OPERATIONS_H

#define SET_BIT(X, BIT)    ((X) |= (1UL << (BIT)))
#define CLEAR_BIT(X, BIT)  ((X) &= ~(1UL << (BIT)))
#define READ_BIT(X, BIT)   (((X) >> (BIT)) & 1UL)
#define TOGGLE_BIT(X, BIT) ((X) ^= (1UL << (BIT)))

#endif
