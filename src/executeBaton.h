
#ifndef _excute_baton_h_
#define _excute_baton_h_

class Connection;

#include <v8.h>
#include <node.h>
#ifndef WIN32
  #include <unistd.h>
#endif
#include <occi.h>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

enum {
  VALUE_TYPE_NULL = 1,
  VALUE_TYPE_OUTPUT = 2,
  VALUE_TYPE_STRING = 3,
  VALUE_TYPE_NUMBER = 4,
  VALUE_TYPE_DATE = 5,
  VALUE_TYPE_TIMESTAMP = 6,
  VALUE_TYPE_CLOB = 7,
  VALUE_TYPE_BLOB = 8,
  VALUE_TYPE_ARRAY = 9
};

struct column_t {
  int type;
  int charForm;
  std::string name;
};

struct row_t {
  std::vector<void*> values;
};

struct value_t {
  int type;
  void* value;
};

struct arrayParam_t {
  // This will hold the info needed for binding array values
  void* value;  
  ub4 collectionLength;
  sb4 elementsSize; // The size of each element in the array
  ub2* elementLength; //  An array that holds the actual length of each element in the array (in case of strings)
  oracle::occi::Type elementsType;
};

struct output_t {
  int type;
  int index;
  std::string strVal; 
  int intVal;
  double doubleVal;
  float floatVal;
  std::vector<row_t*>* rows;
  std::vector<column_t*> columns;
  oracle::occi::Clob clobVal;
  oracle::occi::Date dateVal;
  oracle::occi::Timestamp timestampVal;
  oracle::occi::Number numberVal;
  oracle::occi::Blob blobVal;
};

class ExecuteBaton {
public:
  ExecuteBaton(Connection* connection, const char* sql, v8::Local<v8::Array>* values, v8::Handle<v8::Function>* callback);
  ~ExecuteBaton();
  void ResetValues();
  void ResetRows();
  void ResetOutputs();
  void ResetError();

  Connection *connection;
  v8::Persistent<v8::Function> callback;
  std::vector<value_t*> values;
  std::string sql;
  std::vector<column_t*> columns;
  std::vector<row_t*>* rows;
  std::vector<output_t*>* outputs;
  std::string* error;
  int updateCount;

  static void CopyValuesToBaton(ExecuteBaton* baton, v8::Local<v8::Array>* values);
  static void GetVectorParam(ExecuteBaton* baton, arrayParam_t *value, v8::Local<v8::Array> arr);
};

#endif
