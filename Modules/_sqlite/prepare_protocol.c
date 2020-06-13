/* prepare_protocol.c - the protocol for preparing values for SQLite
 *
 * Copyright (C) 2005-2010 Gerhard Häring <gh@ghaering.de>
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

#include "prepare_protocol.h"
#include "clinic/prepare_protocol.c.h"

/*[clinic input]
module _sqlite3
class _sqlite3.PrepareProtocol "pysqlite_PrepareProtocol *" "pysqlite_PrepareProtocolType"
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=98bb34eaa1f2e0fa]*/

/*[clinic input]
_sqlite3.PrepareProtocol.__init__ as pysqlite_prepare_protocol_init
[clinic start generated code]*/

static int
pysqlite_prepare_protocol_init_impl(pysqlite_PrepareProtocol *self)
/*[clinic end generated code: output=3b31d0c14febd335 input=39127fc4f4923519]*/
{
    return 0;
}

void pysqlite_prepare_protocol_dealloc(pysqlite_PrepareProtocol* self)
{
    PyTypeObject *tp = Py_TYPE(self);

    tp->tp_free(self);
    Py_DECREF(tp);
}

static PyType_Slot type_slots[] = {
    {Py_tp_dealloc, pysqlite_prepare_protocol_dealloc},
    {Py_tp_new, PyType_GenericNew},
    {Py_tp_init, pysqlite_prepare_protocol_init},
    {0, NULL},
};

static PyType_Spec type_spec = {
    .name = MODULE_NAME ".PrepareProtocol",
    .basicsize = sizeof(pysqlite_PrepareProtocol),
    .flags = Py_TPFLAGS_DEFAULT,
    .slots = type_slots,
};

PyTypeObject *pysqlite_PrepareProtocolType = NULL;

extern int pysqlite_prepare_protocol_setup_types(PyObject *module)
{
    pysqlite_PrepareProtocolType = (PyTypeObject *)PyType_FromModuleAndSpec(module, &type_spec, NULL);
    if (pysqlite_PrepareProtocolType == NULL) {
        return -1;
    }
    return 0;
}
