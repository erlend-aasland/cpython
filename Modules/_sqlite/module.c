/* module.c - the module itself
 *
 * Copyright (C) 2004-2010 Gerhard Häring <gh@ghaering.de>
 *
 * This file is part of pysqlite.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include "connection.h"
#include "statement.h"
#include "cursor.h"
#include "cache.h"
#include "prepare_protocol.h"
#include "microprotocols.h"
#include "row.h"

#if SQLITE_VERSION_NUMBER < 3007003
#error "SQLite 3.7.3 or higher required"
#endif

#include "clinic/module.c.h"
/*[clinic input]
module _sqlite3
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=81e330492d57488e]*/

pysqlite_state *pysqlite_get_state(PyObject *module) {
    pysqlite_state *state =  ((pysqlite_state *)PyModule_GetState(module));
    assert(state != NULL);
    return state;
}

/* Python seems to have no way of extracting a single keyword-arg at
 * C-level, so this code is redundant with the one in connection_init in
 * connection.c and must always be copied from there ... */
/*[clinic input]
_sqlite3.connect as pysqlite_connect

    database: object(converter='PyUnicode_FSConverter')
    timeout: double = 5.0
    detect_types: int = 0
    isolation_level: object = NULL
    check_same_thread: int = 1
    factory: object(c_default='(PyObject *)pysqlite_get_state(module)->ConnectionType') = ConnectionType
    cached_statements: int = 100
    uri: bool = False

Opens a connection to the SQLite database file database.

You can use ":memory:" to open a database connection to a database that resides
in RAM instead of on disk.
[clinic start generated code]*/

static PyObject *
pysqlite_connect_impl(PyObject *module, PyObject *database, double timeout,
                      int detect_types, PyObject *isolation_level,
                      int check_same_thread, PyObject *factory,
                      int cached_statements, int uri)
/*[clinic end generated code: output=450ac9078b4868bb input=f821fd43a7afe4c3]*/
{
    PyObject* obj_timeout;
    PyObject* obj_detect_types;
    PyObject* obj_check_same_thread;
    PyObject* obj_cached_statements;
    PyObject* obj_uri;
    PyObject* result;
    int decref_isolation_level = 0;

    if (PySys_Audit("sqlite3.connect", "O", database) < 0) {
        return NULL;
    }

    if (!isolation_level) {
        isolation_level = PyUnicode_FromString("");
        if (!isolation_level) {
            return NULL;
        }
        decref_isolation_level = 1;
    }

    obj_timeout = PyFloat_FromDouble(timeout);
    obj_detect_types = PyLong_FromLong(detect_types);
    obj_check_same_thread = PyLong_FromLong(check_same_thread);
    obj_cached_statements = PyLong_FromLong(cached_statements);
    obj_uri = PyBool_FromLong(uri);

    result = PyObject_CallFunctionObjArgs(factory,
                                          database,
                                          obj_timeout,
                                          obj_detect_types,
                                          isolation_level,
                                          obj_check_same_thread,
                                          factory,
                                          obj_cached_statements,
                                          obj_uri,
                                          NULL);
    if (decref_isolation_level) {
        Py_DECREF(isolation_level);
    }
    Py_XDECREF(database);
    Py_XDECREF(obj_timeout);
    Py_XDECREF(obj_detect_types);
    Py_XDECREF(obj_check_same_thread);
    Py_XDECREF(obj_cached_statements);
    Py_XDECREF(obj_uri);

    return result;
}

/*[clinic input]
_sqlite3.complete_statement as pysqlite_complete_statement

    statement: str

Checks if a string contains a complete SQL statement. Non-standard.
[clinic start generated code]*/

static PyObject *
pysqlite_complete_statement_impl(PyObject *module, const char *statement)
/*[clinic end generated code: output=e55f1ff1952df558 input=f6b24996b31c5c33]*/
{
    PyObject* result;

    if (sqlite3_complete(statement)) {
        result = Py_True;
    } else {
        result = Py_False;
    }

    Py_INCREF(result);

    return result;
}

/*[clinic input]
_sqlite3.enable_shared_cache as pysqlite_enable_shared_cache

    do_enable: int

Enable or disable shared cache mode for the calling thread.

Experimental/Non-standard.
[clinic start generated code]*/

static PyObject *
pysqlite_enable_shared_cache_impl(PyObject *module, int do_enable)
/*[clinic end generated code: output=259c74eedee1516b input=8400e41bc58b6b24]*/
{
    pysqlite_state *state = pysqlite_get_state(module);
    int rc;

    rc = sqlite3_enable_shared_cache(do_enable);

    if (rc != SQLITE_OK) {
        PyErr_SetString(state->OperationalError, "Changing the shared_cache flag failed");
        return NULL;
    } else {
        Py_RETURN_NONE;
    }
}

/*[clinic input]
_sqlite3.register_adapter as pysqlite_register_adapter

    type: object(type='PyTypeObject *')
    caster: object
    /

Registers an adapter with pysqlite's adapter registry. Non-standard.
[clinic start generated code]*/

static PyObject *
pysqlite_register_adapter_impl(PyObject *module, PyTypeObject *type,
                               PyObject *caster)
/*[clinic end generated code: output=a287e8db18e8af23 input=839dad90e2492725]*/
{
    pysqlite_state *state = pysqlite_get_state(module);
    int rc;

    /* a basic type is adapted; there's a performance optimization if that's not the case
     * (99 % of all usages) */
    if (type == &PyLong_Type || type == &PyFloat_Type
            || type == &PyUnicode_Type || type == &PyByteArray_Type) {
        state->BaseTypeAdapted = 1;
    }

    rc = pysqlite_microprotocols_add(type, (PyObject*)state->PrepareProtocolType, caster);
    if (rc == -1)
        return NULL;

    Py_RETURN_NONE;
}

/*[clinic input]
_sqlite3.register_converter as pysqlite_register_converter

    name as orig_name: unicode
    converter as callable: object
    /

Registers a converter with pysqlite. Non-standard.
[clinic start generated code]*/

static PyObject *
pysqlite_register_converter_impl(PyObject *module, PyObject *orig_name,
                                 PyObject *callable)
/*[clinic end generated code: output=a2f2bfeed7230062 input=e074cf7f4890544f]*/
{
    pysqlite_state *state = pysqlite_get_state(module);
    PyObject* name = NULL;
    PyObject* retval = NULL;
    _Py_IDENTIFIER(upper);

    /* convert the name to upper case */
    name = _PyObject_CallMethodIdNoArgs(orig_name, &PyId_upper);
    if (!name) {
        goto error;
    }

    if (PyDict_SetItem(state->converters, name, callable) != 0) {
        goto error;
    }

    Py_INCREF(Py_None);
    retval = Py_None;
error:
    Py_XDECREF(name);
    return retval;
}

/*[clinic input]
_sqlite3.enable_callback_tracebacks as pysqlite_enable_callback_trace

    enable: int
    /

Enable or disable callback functions throwing errors to stderr.
[clinic start generated code]*/

static PyObject *
pysqlite_enable_callback_trace_impl(PyObject *module, int enable)
/*[clinic end generated code: output=4ff1d051c698f194 input=cb79d3581eb77c40]*/
{
    pysqlite_state *state = pysqlite_get_state(module);

    state->enable_callback_tracebacks = enable;

    Py_RETURN_NONE;
}

/*[clinic input]
_sqlite3.adapt as pysqlite_adapt

    obj: object
    proto: object(c_default='(PyObject*)pysqlite_get_state(module)->PrepareProtocolType') = PrepareProtocolType
    alt: object = NULL
    /

Adapt given object to given protocol. Non-standard.
[clinic start generated code]*/

static PyObject *
pysqlite_adapt_impl(PyObject *module, PyObject *obj, PyObject *proto,
                    PyObject *alt)
/*[clinic end generated code: output=0c3927c5fcd23dd9 input=0d05fd95e8241643]*/
{
    return pysqlite_microprotocols_adapt(pysqlite_get_state(module), obj, proto, alt);
}

static int converters_init(PyObject* module)
{
    pysqlite_state *state = pysqlite_get_state(module);

    state->converters = PyDict_New();
    if (!state->converters) {
        return -1;
    }

    if (PyModule_AddObject(module, "converters", state->converters) < 0) {
        Py_DECREF(state->converters);
        return -1;
    }
    return 0;
}

static PyMethodDef module_methods[] = {
    PYSQLITE_ADAPT_METHODDEF
    PYSQLITE_COMPLETE_STATEMENT_METHODDEF
    PYSQLITE_CONNECT_METHODDEF
    PYSQLITE_ENABLE_CALLBACK_TRACE_METHODDEF
    PYSQLITE_ENABLE_SHARED_CACHE_METHODDEF
    PYSQLITE_REGISTER_ADAPTER_METHODDEF
    PYSQLITE_REGISTER_CONVERTER_METHODDEF
    {NULL, NULL}
};

static int add_integer_constants(PyObject *module) {
    int ret = 0;

    ret += PyModule_AddIntMacro(module, PARSE_DECLTYPES);
    ret += PyModule_AddIntMacro(module, PARSE_COLNAMES);
    ret += PyModule_AddIntMacro(module, SQLITE_OK);
    ret += PyModule_AddIntMacro(module, SQLITE_DENY);
    ret += PyModule_AddIntMacro(module, SQLITE_IGNORE);
    ret += PyModule_AddIntMacro(module, SQLITE_CREATE_INDEX);
    ret += PyModule_AddIntMacro(module, SQLITE_CREATE_TABLE);
    ret += PyModule_AddIntMacro(module, SQLITE_CREATE_TEMP_INDEX);
    ret += PyModule_AddIntMacro(module, SQLITE_CREATE_TEMP_TABLE);
    ret += PyModule_AddIntMacro(module, SQLITE_CREATE_TEMP_TRIGGER);
    ret += PyModule_AddIntMacro(module, SQLITE_CREATE_TEMP_VIEW);
    ret += PyModule_AddIntMacro(module, SQLITE_CREATE_TRIGGER);
    ret += PyModule_AddIntMacro(module, SQLITE_CREATE_VIEW);
    ret += PyModule_AddIntMacro(module, SQLITE_DELETE);
    ret += PyModule_AddIntMacro(module, SQLITE_DROP_INDEX);
    ret += PyModule_AddIntMacro(module, SQLITE_DROP_TABLE);
    ret += PyModule_AddIntMacro(module, SQLITE_DROP_TEMP_INDEX);
    ret += PyModule_AddIntMacro(module, SQLITE_DROP_TEMP_TABLE);
    ret += PyModule_AddIntMacro(module, SQLITE_DROP_TEMP_TRIGGER);
    ret += PyModule_AddIntMacro(module, SQLITE_DROP_TEMP_VIEW);
    ret += PyModule_AddIntMacro(module, SQLITE_DROP_TRIGGER);
    ret += PyModule_AddIntMacro(module, SQLITE_DROP_VIEW);
    ret += PyModule_AddIntMacro(module, SQLITE_INSERT);
    ret += PyModule_AddIntMacro(module, SQLITE_PRAGMA);
    ret += PyModule_AddIntMacro(module, SQLITE_READ);
    ret += PyModule_AddIntMacro(module, SQLITE_SELECT);
    ret += PyModule_AddIntMacro(module, SQLITE_TRANSACTION);
    ret += PyModule_AddIntMacro(module, SQLITE_UPDATE);
    ret += PyModule_AddIntMacro(module, SQLITE_ATTACH);
    ret += PyModule_AddIntMacro(module, SQLITE_DETACH);
    ret += PyModule_AddIntMacro(module, SQLITE_ALTER_TABLE);
    ret += PyModule_AddIntMacro(module, SQLITE_REINDEX);
    ret += PyModule_AddIntMacro(module, SQLITE_ANALYZE);
    ret += PyModule_AddIntMacro(module, SQLITE_CREATE_VTABLE);
    ret += PyModule_AddIntMacro(module, SQLITE_DROP_VTABLE);
    ret += PyModule_AddIntMacro(module, SQLITE_FUNCTION);
    ret += PyModule_AddIntMacro(module, SQLITE_SAVEPOINT);
#if SQLITE_VERSION_NUMBER >= 3008003
    ret += PyModule_AddIntMacro(module, SQLITE_RECURSIVE);
#endif
    ret += PyModule_AddIntMacro(module, SQLITE_DONE);
    return ret;
}

static int pysqlite_traverse(PyObject *module, visitproc visit, void *arg)
{
    pysqlite_state *state = pysqlite_get_state(module);

    // Exceptions
    Py_VISIT(state->DataError);
    Py_VISIT(state->DatabaseError);
    Py_VISIT(state->Error);
    Py_VISIT(state->IntegrityError);
    Py_VISIT(state->InterfaceError);
    Py_VISIT(state->InternalError);
    Py_VISIT(state->NotSupportedError);
    Py_VISIT(state->OperationalError);
    Py_VISIT(state->ProgrammingError);
    Py_VISIT(state->Warning);

    // Types
    Py_VISIT(state->CacheType);
    Py_VISIT(state->ConnectionType);
    Py_VISIT(state->CursorType);
    Py_VISIT(state->NodeType);
    Py_VISIT(state->PrepareProtocolType);
    Py_VISIT(state->RowType);
    Py_VISIT(state->StatementType);

    // Misc
    Py_VISIT(state->converters);

    return 0;
}

static int pysqlite_clear(PyObject *module)
{
    pysqlite_state *state = pysqlite_get_state(module);

    // Exceptions
    Py_CLEAR(state->DataError);
    Py_CLEAR(state->DatabaseError);
    Py_CLEAR(state->Error);
    Py_CLEAR(state->IntegrityError);
    Py_CLEAR(state->InterfaceError);
    Py_CLEAR(state->InternalError);
    Py_CLEAR(state->NotSupportedError);
    Py_CLEAR(state->OperationalError);
    Py_CLEAR(state->ProgrammingError);
    Py_CLEAR(state->Warning);

    // Types
    Py_CLEAR(state->CacheType);
    Py_CLEAR(state->ConnectionType);
    Py_CLEAR(state->CursorType);
    Py_CLEAR(state->NodeType);
    Py_CLEAR(state->PrepareProtocolType);
    Py_CLEAR(state->RowType);
    Py_CLEAR(state->StatementType);

    // Misc
    Py_CLEAR(state->converters);

    return 0;
}

#define ADD_TYPE(module, type)                 \
do {                                           \
    if (PyModule_AddType(module, type) < 0) {  \
        goto error;                            \
    }                                          \
} while (0)

#define ADD_EXCEPTION(module, name, exc, base)                  \
do {                                                            \
    exc = PyErr_NewException(MODULE_NAME "." name, base, NULL); \
    if (!exc) {                                                 \
        goto error;                                             \
    }                                                           \
    if (PyModule_AddObject(module, name, exc) < 0) {            \
        Py_DECREF(exc);                                         \
        goto error;                                             \
    }                                                           \
} while (0)

static int pysqlite_exec(PyObject *module)
{
    pysqlite_state *state = pysqlite_get_state(module);

    if (sqlite3_libversion_number() < 3007003) {
        PyErr_SetString(PyExc_ImportError, MODULE_NAME ": SQLite 3.7.3 or higher required");
        goto error;
    }

    if ((pysqlite_row_setup_types(module) < 0) ||
        (pysqlite_cursor_setup_types(module) < 0) ||
        (pysqlite_connection_setup_types(module) < 0) ||
        (pysqlite_cache_setup_types(module) < 0) ||
        (pysqlite_statement_setup_types(module) < 0) ||
        (pysqlite_prepare_protocol_setup_types(module) < 0)
       ) {
        goto error;
    }

    ADD_TYPE(module, state->ConnectionType);
    ADD_TYPE(module, state->CursorType);
    ADD_TYPE(module, state->PrepareProtocolType);
    ADD_TYPE(module, state->RowType);

    /*** Create DB-API Exception hierarchy */
    ADD_EXCEPTION(module, "Error", state->Error, PyExc_Exception);
    ADD_EXCEPTION(module, "Warning", state->Warning, PyExc_Exception);

    /* Error subclasses */
    ADD_EXCEPTION(module, "InterfaceError", state->InterfaceError, state->Error);
    ADD_EXCEPTION(module, "DatabaseError", state->DatabaseError, state->Error);

    /* state->DatabaseError subclasses */
    ADD_EXCEPTION(module, "InternalError", state->InternalError, state->DatabaseError);
    ADD_EXCEPTION(module, "OperationalError", state->OperationalError, state->DatabaseError);
    ADD_EXCEPTION(module, "ProgrammingError", state->ProgrammingError, state->DatabaseError);
    ADD_EXCEPTION(module, "IntegrityError", state->IntegrityError, state->DatabaseError);
    ADD_EXCEPTION(module, "DataError", state->DataError, state->DatabaseError);
    ADD_EXCEPTION(module, "NotSupportedError", state->NotSupportedError, state->DatabaseError);

    /* In Python 2.x, setting Connection.text_factory to
       OptimizedUnicode caused Unicode objects to be returned for
       non-ASCII data and bytestrings to be returned for ASCII data.
       Now OptimizedUnicode is an alias for str, so it has no
       effect. */
    Py_INCREF((PyObject*)&PyUnicode_Type);
    if (PyModule_AddObject(module, "OptimizedUnicode", (PyObject*)&PyUnicode_Type) < 0) {
        Py_DECREF((PyObject*)&PyUnicode_Type);
        goto error;
    }

    /* Set integer constants */
    if (add_integer_constants(module) < 0) {
        goto error;
    }

    if (PyModule_AddStringConstant(module, "version", PYSQLITE_VERSION) < 0) {
        goto error;
    }

    if (PyModule_AddStringConstant(module, "sqlite_version", sqlite3_libversion())) {
        goto error;
    }

    /* initialize microprotocols layer */
    if (pysqlite_microprotocols_init(module) < 0) {
        goto error;
    }

    /* initialize the default converters */
    if (converters_init(module) < 0) {
        goto error;
    }

    return 0;

error:
    return -1;
}

static struct PyModuleDef_Slot module_slots[] = {
    {Py_mod_exec, pysqlite_exec},
    {0, NULL},
};

struct PyModuleDef _sqlite3module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "_sqlite3",
    .m_size = sizeof(pysqlite_state),
    .m_methods = module_methods,
    .m_slots = module_slots,
    .m_traverse = pysqlite_traverse,
    .m_clear = pysqlite_clear,
};

PyMODINIT_FUNC PyInit__sqlite3(void)
{
    return PyModuleDef_Init(&_sqlite3module);
}
