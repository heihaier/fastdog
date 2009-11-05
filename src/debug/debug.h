/* debug.h
 * Heihaier - admin@heihaier.org
 */

#ifndef __H_DEBUG_H__
#define __H_DEBUG_H__

#include <stdio.h>

#ifdef __H_DEBUG__
#define DBG		printf
#else
#define	DBG
#endif /* __H_DEBUG__ */

#endif /* __H_DEBUG_H__ */
