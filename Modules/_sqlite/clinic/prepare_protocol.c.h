/*[clinic input]
preserve
[clinic start generated code]*/

static int
pysqlite_prepare_protocol_init_impl(pysqlite_PrepareProtocol *self);

static int
pysqlite_prepare_protocol_init(PyObject *self, PyObject *args, PyObject *kwargs)
{
    int return_value = -1;

    if (Py_IS_TYPE(self, pysqlite_PrepareProtocolType) &&
        !_PyArg_NoPositional("PrepareProtocol", args)) {
        goto exit;
    }
    if (Py_IS_TYPE(self, pysqlite_PrepareProtocolType) &&
        !_PyArg_NoKeywords("PrepareProtocol", kwargs)) {
        goto exit;
    }
    return_value = pysqlite_prepare_protocol_init_impl((pysqlite_PrepareProtocol *)self);

exit:
    return return_value;
}
/*[clinic end generated code: output=45e0d674f1c77f8c input=a9049054013a1b77]*/
