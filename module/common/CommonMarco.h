#ifndef __COMMONMARCO_H
#define __COMMONMARCO_H

#define CONCAT_2(p1, p2)      CONCAT_2_(p1, p2)
/** Auxiliary macro used by @ref CONCAT_2 */
#define CONCAT_2_(p1, p2)     p1##p2

#define CONCAT_3_(__A, __B, __C)    __A##__B##__C
#define CONCAT_3(__A, __B, __C)      CONCAT_3_(__A, __B, __C)

#define UNUSED_VARIABLE(X)  ((void)(X))
#define UNUSED_PARAMETER(X) UNUSED_VARIABLE(X)
#define UNUSED_RETURN_VALUE(X) UNUSED_VARIABLE(X)
#endif
