# easy-table

A smart table for read and search in memory. You can convert it between `csv string format` and `memory format`.

Example project: https://github.com/easy-csv/easy-table-example

## Table Format

Table format in memory.

    FieldHead   :  field_1 field_2 field_3 ... field_N
    Record_1    :   cell    cell    cell   ...  cell
    Record_2    :   cell    cell    cell   ...  cell
    Record_3    :   cell    cell    cell   ...  cell
    ..................................................
    Record_N    :   cell    cell    cell   ...  cell

When the table is generated, the system will automatically generate an index map for field_1, we can find a record based on the key of field_1.

## Data Format
In memory, the table content has three data formats, but each field can only be one of them.

Suported format:

1. number: integer format
2. float: floating-point format
3. text: string format

## Field Declaration

The type of field can be specified in csv string.

    	key,fv1(type=float),fv2(type=float),num1(type=num),num2(type=num),txt1(type=text),txt2

If no field type is specified, the default field type will be 'text'