#include "parameters.h"

parameters::parameters( void ) {
  return;
}

parameters::parameters( string filename ) {
  readFile(filename);
  return;
}

void parameters::readFile( string filename ) {
  FILE *pFile;
  char inStream[2048];

  /*-- Now, try to read in the parameter file --*/
  errno = 0;
  if ( (pFile=fopen(filename.c_str(),"r")) == NULL) {
    perror(filename.c_str());
    exit(errno);
  }

  char key[512], value[1471], type[64];

  memset( (void *)key,   0, 512*sizeof(char));
  memset( (void *)value, 0, 1471*sizeof(char));
  memset( (void *)type,  0,   64*sizeof(char));

  while ( fgets(inStream, 2047, pFile) != NULL ) {

    if ( inStream[0] == '#' )
      continue;

    if ( !strncmp(inStream, "EOF", 3) )
      break;

    sscanf(inStream, "%s %s = %s %*s", type, key, value);

    if ( !strncmp(type, "EOF", 3) )
      break;

    if ( !type[0] || !key[0] || !value[0] || type[0] == '#' || type[0] == ' ' )
      continue;

    if ( !strcmp(type, "int") ) {
      if ( strstr( (const char *)inStream, "inf" ) || strstr( (const char *)inStream, "INF" ) )
	pMap[key] = MAX_INT;
      else
	pMap[key] = atoi(value);
    }

    if ( !strcmp(type, "long") ) {
      if ( strstr( (const char *)inStream, "inf" ) || strstr( (const char *)inStream, "INF" ) )
	pMap[key] = MAX_INT;
      else
	pMap[key] = atol(value);
    }

    if ( !strcmp(type, "ulong") ) {
      if ( strstr( (const char *)inStream, "inf" ) || strstr( (const char *)inStream, "INF" ) )
	pMap[key] = MAX_INT;
      else
	pMap[key] = abs(atol(value));
    }

    else if ( !strcmp(type, "uint") ) {
      if ( strstr( (const char *)inStream, "inf" ) || strstr( (const char *)inStream, "INF" ) )
	pMap[key] = MAX_INT;
      else
	pMap[key] = abs(atoi(value));
    }

    else if ( !strcmp(type, "string") ) {
      pMap[key] = new char [strlen(value)+1];
      strcpy(pMap[key], value);

      this->allocated_arrays.push_back((string)key);
    }

    else if ( !strcmp(type, "float") )
      pMap[key] = strtof(value, (char **)NULL);

    else if ( !strcmp(type, "double") )
      pMap[key] = strtod(value, (char **)NULL);

    else if ( !strcmp(type, "bool") )
      pMap[key] = (!strcmp(value, "true")) ? true : false;

    else if ( !strcmp(type, "uint_array") ) {

      vector<string> elements;
      char *p = strtok(value, (const char *)",");
      while ( p != NULL ) {
	elements.push_back(p);
	p = strtok(NULL, (const char *)",");
      }
      uint *temp = new uint [elements.size()];
      for ( uint i=0; i<elements.size(); i++ ) {
	temp[i] = atoi((const char *)elements[i].c_str());
      }
      pMap[key] = temp;

      string new_key = key + std::string("_SIZE");
      pMap[new_key] = (uint)elements.size();

      this->allocated_arrays.push_back((string)key);

    } else if ( !strcmp(type, "int_array") ) {

      vector<string> elements;
      char *p = strtok(value, (const char *)",");
      while ( p != NULL ) {
	elements.push_back(p);
	p = strtok(NULL, (const char *)",");
      }
      int *temp = new int [elements.size()];
      for ( uint i=0; i<elements.size(); i++ ) {
	temp[i] = atoi((const char *)elements[i].c_str());
      }
      pMap[key] = temp;

      string new_key = key + std::string("_SIZE");
      pMap[new_key] = (int)elements.size();

      this->allocated_arrays.push_back((string)key);

    } else if ( !strcmp(type, "float_array") ) {
      vector<string> elements;
      char *p = strtok(value, (const char *)",");
      while ( p != NULL ) {
	elements.push_back(p);
	p = strtok(NULL, (const char *)",");
      }
      float *temp = new float [elements.size()];
      for ( uint i=0; i<elements.size(); i++ ) {
	temp[i] = atof((const char *)elements[i].c_str());
      }
      pMap[key] = temp;

      string new_key = key + std::string("_SIZE");
      pMap[new_key] = (int)elements.size();

      this->allocated_arrays.push_back((string)key);

    } else if ( !strcmp(type, "double_array") ) {
      vector<string> elements;
      char *p = strtok(value, (const char *)",");
      while ( p != NULL ) {
	cout << p << "\n";
	elements.push_back(p);
	p = strtok(NULL, (const char *)",");
      }
      double *temp = new double [elements.size()];
      for ( uint i=0; i<elements.size(); i++ ) {
	temp[i] = atof((const char *)elements[i].c_str());
      }
      pMap[key] = temp;

      string new_key = key + std::string("_SIZE");
      pMap[new_key] = (int)elements.size();

      this->allocated_arrays.push_back((string)key);

    } else if ( !strcmp( type, "char" ) || !strcmp( type, "uchar" ) ) {
      pMap[key] = value;

    } else if ( !strcmp( type, "short" ) ) {
      pMap[key] = (short)atoi(value);

    } else if ( !strcmp( type, "ushort" ) ) {
      pMap[key] = (unsigned short)atoi(value);

    }

    string new_key = key + string("_TYPE");
    pMap[new_key] = type;

    memset( (void *)key,   0, 128*sizeof(char));
    memset( (void *)value, 0, 128*sizeof(char));
    memset( (void *)type,  0,   16*sizeof(char));
  }

  return;
}

parameters::~parameters( void ) {

  if ( this->allocated_arrays.size() ) {
    for ( uint i=0; i<this->allocated_arrays.size(); i++ ) {
      if ( pMap[this->allocated_arrays[i]] )
	delete [] pMap[this->allocated_arrays[i]];
    }
  }

  return;
}

void parameters::save( string outfile ) {
  FILE * oFile;
  if ( !outfile.length() )
    oFile = stdout;
  else {
    errno = 0;
    if ( (oFile=fopen(outfile.c_str(),"w")) == NULL) {
      perror(outfile.c_str());
      oFile = stdout;
    }
  }

  for(auto kv : pMap) {
    string key = kv.first;

    if ( key.find("_SIZE") > key.length() && key.find("_TYPE") > key.length())
      fprintf(oFile, "%s %s = %s\n", getType(key).c_str(), key.c_str(), toString(key).c_str() );

  }
  return;
}

void parameters::flush( string input, string output ) {
  FILE *pFile, *oFile;
  char inStream[2048];

  if ( input == "" )
    input = inFile;

  /*-- Now, try to read in the parameter file --*/
  errno = 0;
  if ( (pFile=fopen(input.c_str(),"r")) == NULL) {
    perror(input.c_str());
    return;
  }

  if ( output == "" )
    oFile = stdout;
  else if ( output == input ) {
    errno = 0;
    if ( (oFile=fopen("temp.rcp","w")) == NULL) {
      perror("temp.rcp");
      oFile = stdout;
    }
  } else {
    errno = 0;
    if ( (oFile=fopen(output.c_str(),"w")) == NULL) {
      perror(output.c_str());
      oFile = stdout;
    }
  }
  char key[512], value[1471], type[64];

  memset( (void *)key,   0, 512*sizeof(char));
  memset( (void *)value, 0, 1471*sizeof(char));
  memset( (void *)type,  0,   64*sizeof(char));

  while ( fgets(inStream, 2047, pFile) != NULL ) {

    sscanf(inStream, "%s %s = %s %*s", type, key, value);

    int c = (int)inStream[0];
    if ( c == 10 || c == 35 || c == 0 || !key[0] || !value[0] ) {
      fprintf(oFile, "%s", inStream);
      continue;
    }

    if ( this->pMap.count(key) ) {
      string newValue = toString( key, type );
      if ( newValue == "NULL" )
	cout << key << " returned NULL\n";

      if ( newValue != value ) {
	bool same = false;
	if ( !strcmp(type, "bool") )
	  same = ( (!strcmp(value, "false") && newValue == "0") || 
		   (!strcmp(value, "true")  && newValue == "1") );

	if ( !same ) {

	  if ( !strcmp(type, "bool") ) {
	    if (!strcmp(value, "false"))
	      newValue = "true";
	    else if (!strcmp(value, "true"))
	      newValue = "false";
	  }
	  string str(inStream);

	  /* Locate the substring to replace. */
	  size_t index = str.find(value, 0);

	  /* Make the replacement. */
	  if ( index < str.size() && newValue != "NULL" )
	    str.replace(index, strlen(value), newValue);

	  strcpy(inStream, str.c_str());

	} // End if ( !same )

      } // End if ( newValue != value )

    } // End if ( this->pMap.count(key) )

    fprintf(oFile, "%s", inStream);

  } // End while ( fgets(inStream, 2047, pFile) != NULL )

  fclose(pFile);
  fclose(oFile);

  if ( output == input )
    //cout << "rename ( \"temp.rcp\", " << input.c_str() << " );\n";
    rename ( "temp.rcp", input.c_str() );

  return;
}

void parameters::flushOne( string which ) {

  FILE *pFile, *oFile;

  errno = 0;
  if ( (pFile=fopen(inFile.c_str(),"r")) == NULL) {
    perror(inFile.c_str());
    return;
  }
  errno = 0;
  if ( (oFile=fopen("temp.rcp","w")) == NULL) {
    perror("temp.rcp");
    return;
  }

  char key[512], value[1471], type[64], inStream[2048];
  memset( (void *)inStream, 0, 2048*sizeof(char) );
  memset( (void *)key,      0,  512*sizeof(char) );
  memset( (void *)value,    0, 1471*sizeof(char) );
  memset( (void *)type,     0,   64*sizeof(char) );

  while ( fgets(inStream, 2047, pFile) != NULL ) {

    sscanf(inStream, "%s %s = %s %*s", type, key, value);
    string str(inStream);

    if ( !strcmp(key, which.c_str()) ) {
      string newValue = toString( which, type );
      if ( newValue == "NULL" )
	cout << "toString( " << which << " ) returned NULL\n";

      size_t index = str.find(value, 0);
      if ( index < str.size() && newValue != "NULL" ) {
	str.replace( index, str.size(), newValue );
	str += "\n";
      }
    }

    fprintf(oFile, "%s", str.c_str());

  }

  fclose(pFile);
  fclose(oFile);

  //cout << "rename(\"temp.rcp\", " << inFile.c_str() << ");\n";
  rename("temp.rcp", inFile.c_str());

  return;
}

void parameters::dumpKeys() {

  for(auto kv : pMap) {
    string key = kv.first;
    cout << key << "\n";
  }
  return;
}

bool parameters::keyExists( string search ) {

  for(auto kv : pMap) {
    string key = kv.first;
    if ( key == search )
      return true;
  }
  return false;
}

string parameters::toString( string key, string type ) {
  string thisType = type;

  if ( thisType == "" ) {
    string typeKey = string(key) + string("_TYPE");

    if ( this->pMap.count(typeKey) )
      thisType = (string)pMap[typeKey];
    else
      return (string)"NULL";
  }

  if ( thisType == "int" ) {
    int x = getInt(key);
    return SSTR(x);
  }

  else if ( thisType == "uint" ) {
    uint x = getUInt(key);
    return SSTR(x);
  }

  else if ( thisType == "bool" ) {
    bool x = getBool(key);
    return SSTR(x);
  }

  else if ( thisType == "short" ) {
    short x = getShort(key);
    return SSTR(x);
  }
  
  else if ( thisType == "ushort" ) {
    ushort x = getUShort(key);
    return SSTR(x);
  }
  
  else if ( thisType == "long" ) {
    long x = getLong(key);
    return SSTR(x);
  }
  
  else if ( thisType == "ulong" ) {
    ulong x = getULong(key);
    return SSTR(x);
  }

  else if ( thisType == "string" ) {
    return getString(key);
  }

  else if ( thisType == "char" ) {
    char x = getChar(key);
    return SSTR(x);
  }
  
  else if ( thisType == "uchar" ) {
    char x = getChar(key);
    return SSTR(x);
  }
  
  else if ( thisType == "float" ) {
    float x = getFloat(key);
    if ( x == (int)x )
      return FSTR(x);
    else
      return SSTR(x);
  }
  
  else if ( thisType == "double" ) {
    double x = getDouble(key);
    return SSTR(x);
  }

  else if ( thisType == "float_array" ) {
    float * x = getFloatArray(key);

    string size_key = key + "_SIZE";
    uint size = getUInt(size_key);

    string array_string = "";

    for ( uint i=0; i<size; i++ ) {
      if ( x[i] == (int)x[i] )
	array_string += FSTR(x[i]);
      else
	array_string += SSTR(x[i]);
      if ( i < size-1 )
	array_string += ",";
    }
    return array_string;
  }

  else if ( thisType == "double_array" ) {
    double * x = getDoubleArray(key);

    string size_key = key + "_SIZE";
    uint size = getUInt(size_key);

    string array_string = "";

    for ( uint i=0; i<size-1; i++ )
      array_string += SSTR(x[i]) + ",";
    array_string += SSTR(x[size-1]);

    return array_string;
  }

  else if ( thisType == "int_array" ) {
    int * x = getIntArray(key);

    string size_key = key + "_SIZE";
    uint size = getUInt(size_key);

    string array_string = "";

    for ( uint i=0; i<size-1; i++ )
      array_string += SSTR(x[i]) + ",";
    array_string += SSTR(x[size-1]);

    return array_string;
  }

  else if ( thisType == "uint_array" ) {
    uint * x = getUIntArray(key);

    string size_key = key + "_SIZE";
    uint size = getUInt(size_key);

    string array_string = "";

    for ( uint i=0; i<size-1; i++ )
      array_string += SSTR(x[i]) + ",";
    array_string += SSTR(x[size-1]);

    return array_string;
  }

  return (string)"NULL";
}

unsigned long parameters::getULong( string key ) {
  if ( this->pMap.count(key) )
    return this->pMap[key].asULong;
  else {
    cerr << "No such key " << key << endl;
    return 0;
  }
}

bool parameters::getBool( string key ) {
  if ( this->pMap.count(key) )
    return this->pMap[key].asBool;
  else {
    cerr << "No such key " << key << endl;
    return false;
  }
}

char parameters::getChar( string key ) {
  if ( this->pMap.count(key) )
    return this->pMap[key].asChar;
  else {
    cerr << "No such key " << key << endl;
    return '\0';
  }
}

short parameters::getShort( string key ) {
  if ( this->pMap.count(key) )
    return this->pMap[key].asShort;
  else {
    cerr << "No such key " << key << endl;
    return 0;
  }
}

ushort parameters::getUShort( string key ) {
  if ( this->pMap.count(key) )
    return this->pMap[key].asUShort;
  else {
    cerr << "No such key " << key << endl;
    return 0;
  }
}

int parameters::getInt( string key ) {
  if ( this->pMap.count(key) )
    return this->pMap[key].asInt;
  else {
    cerr << "No such key " << key << endl;
    return 0;
  }
}

int *parameters::getIntArray( string key ) {
  if ( this->pMap.count(key) )
    return (int *)this->pMap[key].asIntArray;
  else {
    cerr << "No such key " << key << endl;
    return (int *)0x0;
  }
}

float parameters::getFloat( string key ) {
  if ( this->pMap.count(key) )
    return (float)this->pMap[key].asFloat;
  else {
    cerr << "No such key " << key << endl;
    return 0.0;
  }
}

float *parameters::getFloatArray( string key ) {
  if ( this->pMap.count(key) )
    return (float *)this->pMap[key].asFloatArray;
  else {
    cerr << "No such key " << key << endl;
    return (float *)0x0;
  }
}

double parameters::getDouble( string key ) {
  if ( this->pMap.count(key) )
    return this->pMap[key].asDouble;
  else {
    cerr << "No such key " << key << endl;
    return 0.0;
  }
}

double * parameters::getDoubleArray( string key ) {
  if ( this->pMap.count(key) )
    return this->pMap[key].asDoubleArray;
  else {
    cerr << "No such key " << key << endl;
    return (double *)0x0;
  }
}

char * parameters::getCString( string key ) {
  if ( this->pMap.count(key) )
    return this->pMap[key].asCString;
  else {
    cerr << "No such key " << key << endl;
    return '\0';
  }
}

string parameters::getString( string key ) {
  if ( this->pMap.count(key) )
    return this->pMap[key].asCString;
  else {
    cerr << "No such key " << key << endl;
    return (string)"NULL";
  }
}

unsigned int parameters::getLong( string key ) {
    if ( this->pMap.count(key) )
    return this->pMap[key].asLong;
  else {
    cerr << "No such key " << key << endl;
    return 0;
  }
}

unsigned int parameters::getUInt( string key ) {
  if ( this->pMap.count(key) )
    return this->pMap[key].asUInt;
  else {
    cerr << "No such key " << key << endl;
    return 0;
  }
}

unsigned int *parameters::getUIntArray( string key ) {
  if ( this->pMap.count(key) )
    return (uint *)this->pMap[key].asUIntArray;
  else {
    cerr << "No such key " << key << endl;
    return (uint *)0x0;
  }
}

string parameters::getType( string key ) {
  if ( !this->pMap.count(key) ) {
    cerr << "No such key " << key << endl;
    return (string)NULL;
  }
  string type_key = key + (string)"_TYPE";
  return string((char *)pMap[type_key.c_str()]);
}

void parameters::setType( string key, string type ) {
  if ( !keyExists(key) ) {
    cerr << "No such key " << key << "\n";
    return;
  }
  string new_key = key + string("_TYPE");
  char *t = new char [type.length()+2];
  strcpy( t, type.c_str() );
  pMap[new_key] = t;
  //delete [] t;

  return;
}

void parameters::setULong( string key, ulong value ) {
  this->pMap[key] = value;
  this->setType(key, (string)"ulong");
  return;
}

void parameters::setLong( string key, long value ) {
  this->pMap[key] = value;
  this->setType(key, (string)"long");
  return;
}

void parameters::setInt( string key, int value ) {
  this->pMap[key] = value;
  this->setType(key, (string)"int");
  return;
}

void parameters::setUInt( string key, uint value ) {
  this->pMap[key] = value;
  this->setType(key, (string)"uint");
  return;
}

void parameters::setChar( string key, char value ) {
  this->pMap[key] = value;
  this->setType(key, (string)"char");
  return;
}

void parameters::setShort( string key, short value ) {
  this->pMap[key] = value;
  this->setType(key, (string)"short");
  return;
}

void parameters::setUShort( string key, ushort value ) {
  this->pMap[key] = value;
  this->setType(key, (string)"ushort");
  return;
}

void parameters::setBool( string key, bool value ) {
  this->pMap[key] = value;
  this->setType(key, (string)"bool");
  return;
}

void parameters::setFloat( string key, float value ) {
  this->pMap[key] = value;
  this->setType(key, (string)"float");
  return;
}

void parameters::setDouble( string key, double value ) {
  this->pMap[key] = value;
  this->setType(key, (string)"double");
  return;
}

void parameters::setString( string key, char *value ) {
  if ( this->pMap[key] )
    delete(this->pMap[key]);
  this->pMap[key] = new char[ strlen(value) + 1 ];

  this->pMap[key] = value;
  this->setType(key, (string)"string");
  return;
}

void parameters::setString( string key, string value ) {
  if ( this->pMap[key] )
   delete(this->pMap[key]);

  this->pMap[key] = new char [ value.size() + 1 ];
  strcpy(this->pMap[key], value.c_str());
  this->setType(key, (string)"string");

  return;
}

void parameters::setCString( string key, string value ) {
  if ( this->pMap[key] )
    delete(this->pMap[key]);
  this->pMap[key] = new char [ value.size() + 1 ];
  this->pMap[key] = value.c_str();
  this->setType(key, (string)"cstring");
  return;
}

void parameters::setIntArray( string key, int *values, uint size ) {
  if ( !size ) {
    cerr << "Invalid array size 0 in parameters::setIntArray(key,values,size)\n";
    return;
  }

  int *temp = new int[size];

  for ( uint i=0; i<size; i++ )
    temp[i] = values[i];

  pMap[key] = temp;

  string new_key = key + std::string("_SIZE");
  pMap[new_key] = size;

  this->allocated_arrays.push_back((string)key);
  this->setType(key, (string)"int_array");

  return;
}

void parameters::setIntArray( string key, vector<int>values ) {

  int *temp = new int[values.size()];

  for ( uint i=0; i<values.size(); i++ )
    temp[i] = values[i];

  pMap[key] = temp;

  string new_key = key + std::string("_SIZE");
  pMap[new_key] = values.size();

  this->allocated_arrays.push_back((string)key);
  this->setType(key, (string)"int_array");

  return;
}

void parameters::setUIntArray( string key, uint * values, uint size ) {
  if ( !size ) {
    cerr << "Invalid array size 0 in parameters::setUIntArray(key,values,size)\n";
    return;
  }

  uint *temp = new uint[size];

  for ( uint i=0; i<size; i++ )
    temp[i] = values[i];

  pMap[key] = temp;

  string new_key = key + std::string("_SIZE");
  pMap[new_key] = size;

  this->allocated_arrays.push_back((string)key);
  this->setType(key, (string)"uint_array");

  return;
}

void parameters::setUIntArray( string key, vector<uint>values ) {

  uint *temp = new uint[values.size()];

  for ( uint i=0; i<values.size(); i++ )
    temp[i] = values[i];

  pMap[key] = temp;

  string new_key = key + std::string("_SIZE");
  pMap[new_key] = values.size();

  this->allocated_arrays.push_back((string)key);
  this->setType(key, (string)"uint_array");

  return;
}

void parameters::setFloatArray( string key, float * values, uint size ) {
  if ( !size ) {
    cerr << "Invalid array size 0 in parameters::setFloatArray(key,values,size)\n";
    return;
  }
  float *temp = new float[size];
  for ( uint i=0; i<size; i++ )
    temp[i] = values[i];

  pMap[key] = temp;

  string new_key = key + std::string("_SIZE");
  pMap[new_key] = size;

  this->allocated_arrays.push_back((string)key);
  this->setType(key, (string)"float_array");

  return;

  return;
}

void parameters::setFloatArray( string key, vector<float>values ) {

  float *temp = new float[values.size()];

  for ( uint i=0; i<values.size(); i++ )
    temp[i] = values[i];

  pMap[key] = temp;

  string new_key = key + std::string("_SIZE");
  pMap[new_key] = values.size();

  this->allocated_arrays.push_back((string)key);
  this->setType(key, (string)"float_array");

  return;
}

void parameters::setDoubleArray( string key, double * values, uint size ) {
  if ( !size ) {
    cerr << "Invalid array size 0 in parameters::setDoubleArray(key,values,size)\n";
    return;
  }

  double *temp = new double[size];

  for ( uint i=0; i<size; i++ )
    temp[i] = values[i];

  pMap[key] = temp;

  string new_key = key + std::string("_SIZE");
  pMap[new_key] = size;

  this->allocated_arrays.push_back((string)key);
  this->setType(key, (string)"double_array");

  return;
}

void parameters::setDoubleArray( string key, vector<double>values ) {

  double *temp = new double[values.size()];

  for ( uint i=0; i<values.size(); i++ )
    temp[i] = values[i];

  pMap[key] = temp;

  string new_key = key + std::string("_SIZE");
  pMap[new_key] = values.size();

  this->allocated_arrays.push_back((string)key);
  this->setType(key, (string)"double_array");

  return;
}
