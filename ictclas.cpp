#include <node.h>
#include <v8.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ICTCLAS50.h"
using namespace v8;

Handle<Value> initICTCLAS(const Arguments& args) {
	HandleScope scope;
	Handle<Value> arg = args[0];
  String::Utf8Value initDir(arg);
	bool initRst = ICTCLAS_Init(*initDir);
  if(!initRst){
  	ThrowException(Exception::TypeError(String::New("initial failure")));
  }
  return scope.Close(Undefined());
}

Handle<Value> importDictFile(const Arguments& args) {
  HandleScope scope;
  Handle<Value> arg = args[0];
  String::Utf8Value dictFile(arg);
  FILE *fp;
  fp = fopen(*dictFile, "r");
  if(fp == NULL){
    ThrowException(Exception::TypeError(String::New("can not open the dict file")));
    return scope.Close(False());
  }
  unsigned wordCount = ICTCLAS_ImportUserDictFile(*dictFile, CODE_TYPE_UTF8);
  return scope.Close(Number::New(wordCount));
}

Handle<Value> segment(const Arguments& args) {
  HandleScope scope;

  Local<Function> cb = Local<Function>::Cast(args[1]);
  Handle<Value> arg0 = args[0];
  String::Utf8Value txt(arg0);
  int txtLen = strlen(*txt);
  char* sRst=(char *)malloc(txtLen * 6);

  ICTCLAS_ParagraphProcess(*txt, txtLen, sRst, CODE_TYPE_UTF8, 0);
  
  const unsigned argc = 1;
  Local<Value> argv[argc] = { Local<Value>::New(String::New(sRst)) };
  cb->Call(Context::GetCurrent()->Global(), argc, argv);
  free(sRst);
  return scope.Close(Undefined());
}

void Init(Handle<Object> target) {
  target->Set(String::NewSymbol("init"),
      FunctionTemplate::New(initICTCLAS)->GetFunction());

  target->Set(String::NewSymbol("segment"),
      FunctionTemplate::New(segment)->GetFunction());

  target->Set(String::NewSymbol("importDictFile"),
      FunctionTemplate::New(importDictFile)->GetFunction());
}

NODE_MODULE(ictclas, Init)