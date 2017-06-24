#ifndef _LINUX_TRACEPOINT_H
#define _LINUX_TRACEPOINT_H
#define TP_PROTO(args...)	args
#define TP_ARGS(args...)	args
#define TP_CONDITION(args...)	args


/* XXX */
#define TRACE_EVENT(name, proto, args, struct, assign, print) static inline void trace_ ## name (int a, int b, ...) { }


#endif
