#ifndef _CLASSES_H_
#define _CLASSES_H_

#define NEWHANDLE(Handle) typedef struct tag ## Handle *Handle
#define CLASS(Handle) struct tag ## Handle

#endif /* _CLASSES_H_ */
