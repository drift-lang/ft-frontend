/* Drift
 *
 * 	- https://drift-lang.fun/
 *
 * GPL 3.0 License - bingxio <3106740988@qq.com> */
#ifndef FT_OBJECT_H
#define FT_OBJECT_H

#include <stdio.h>
#include <stdbool.h>

#include "list.h"
#include "type.h"
#include "code.h"
#include "opcode.h"

/* Object type */
typedef enum {
    OBJ_INT,  OBJ_FLOAT, OBJ_STRING, OBJ_CHAR, OBJ_BOOL,
    OBJ_ENUM, OBJ_FUNC,  OBJ_WHOLE,  OBJ_FACE, OBJ_ARR,
    OBJ_TUP,  OBJ_MAP
} obj_kind;

/* Object system */
typedef struct {
    u_int8_t kind; /* Kind */
    union {
        int integer; /* int */
        double floating; /* float */
        char *string; /* string */
        char ch; /* char */
        bool boolean; /* bool */
        struct {
            char *name;
            list *element;
        } enumeration; /* enum */
        struct {
            char *name;
            list *k;
            list *v;
            type *ret;
            code_object *code;
        } func; /* function */
        struct {
            char *name;
            list *element;
        } face; /* interface */
        struct {
            char *name;
            code_object *code;
            struct frame *fr;
        } whole; /* whole */
        struct {
            list *element;
            type *T;
        } arr; /* array */
        struct {
            list *element;
            type *T;
        } tup; /* tuple */
        struct {
            list *k;
            list *v;
            type *T1;
            type *T2;
        } map; /* map */
    } value; /* Inner value */
} object;

/* Method structure of interface */
typedef struct {
    char *name; /* Face name */
    list *T; /* Types of arguments */
    type *ret; /* Return type */
} method;

/* Output object */
const char *obj_string(object *);

/* Object's raw data */
const char *obj_raw_string(object *);

/* Binary operation */
object *binary_op(u_int8_t, object *, object *);

/* The judgement type is the same */
bool type_checker(type *, object *);

/* Are the two objects equal */
bool obj_eq(object *, object *);

#endif