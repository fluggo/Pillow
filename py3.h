#if PY_VERSION_HEX >= 0x03000000
/* #define VERBOSE */
#define PY3
#define staticforward static
#define PyInt_AsLong PyLong_AsLong
#define PyInt_Check PyLong_Check
#define PyInt_FromLong PyLong_FromLong
#define PyInt_AS_LONG PyLong_AS_LONG
#define PyInt_FromSsize_t PyLong_FromSsize_t
#define PyStringObject PyBytesObject
#define PyString_AsString PyBytes_AsString
#define PyString_AS_STRING PyBytes_AS_STRING
#define PyString_Check PyBytes_Check
#define PyString_FromStringAndSize PyBytes_FromStringAndSize
#define PyString_FromString PyBytes_FromString
#define _PyString_Resize _PyBytes_Resize
#define PyString_GET_SIZE PyBytes_GET_SIZE
#endif