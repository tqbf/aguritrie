#include "ruby.h"
#include "aguri.h"
#include <stdio.h>

static VALUE rb_mAguri;
static VALUE rb_cTree;

typedef struct aguri_data {
  size_t walked;
  size_t kb;
  aguri_t *tree;
} aguri_data_t;

static void
_free(aguri_data_t *p) {
  if(p) {
    aguri_release(&(p->tree));
    free(p);
  }
}

static VALUE
_alloc(VALUE klass) {
  return Data_Wrap_Struct(klass, 0, _free, 0);
}

static VALUE 
_m_initialize(int argc, VALUE *argv, VALUE obj)
{
  VALUE *vmaxnodes = NULL;
  VALUE *vkeybits = NULL;
  size_t maxnodes = 0;
  size_t keybits = 0;
  aguri_data_t *adt = NULL;

  rb_scan_args(argc, argv, "11", &vmaxnodes, &vkeybits);

  maxnodes = NUM2INT(vmaxnodes);
  if(RTEST(keybits)) 
    keybits = NUM2INT(vkeybits);
  else
    keybits = 32;

  adt = ALLOC(aguri_data_t);
  adt->tree = aguri_new(keybits, maxnodes);
  adt->kb = keybits;

  DATA_PTR(obj) = adt;

  return obj;
}

static VALUE
_m_add(int argc, VALUE *argv, VALUE *obj) {
  VALUE vl = 0, vkey = 0, vcount = 0;
  unsigned long key = 0, count = 0, kl = 0;
  aguri_data_t *adt = NULL;
  Data_Get_Struct(obj, aguri_data_t, adt);

  rb_scan_args(argc, argv, "21", &vkey, &vcount, &vl);

  key = NUM2ULONG(vkey);
  count = NUM2ULONG(vcount);

  if(RTEST(vl)) {
    unsigned long kl = NUM2ULONG(vl);
    aguri_addprefix(adt->tree, &key, kl, count);    
  } else {
    aguri_add(adt->tree, &key, count);
  }
  return Qnil;
}

static VALUE
_m_threshold(VALUE obj, VALUE vcount) {
  unsigned long count = 0;
  aguri_data_t *adt = NULL;
  Data_Get_Struct(obj, aguri_data_t, adt);

  count = NUM2ULONG(vcount);

  aguri_aggregate(adt->tree, count);

  return Qnil;
}

static VALUE
_m_reset(VALUE obj) {
  aguri_data_t *adt = NULL;
  Data_Get_Struct(obj, aguri_data_t, adt);

  aguri_reset(adt->tree);

  return Qnil;
}

static VALUE
_m_dump(VALUE obj) {
  aguri_data_t *adt = NULL;
  Data_Get_Struct(obj, aguri_data_t, adt);

  aguri_print(adt->tree);

  return Qnil;
}

static VALUE
_m_size(VALUE obj) {
  aguri_data_t *adt = NULL;
  Data_Get_Struct(obj, aguri_data_t, adt);

  return(ULONG2NUM(aguri_count(adt->tree)));
}

static int
_walker(size_t prefix, u_char *key, u_int64_t ticks, int depth, void *cl) {
  aguri_data_t *adt = cl;
  adt->walked += 1;
  rb_yield_values(4, rb_tainted_str_new(key, (adt->kb / 8)), ULONG2NUM(prefix), ULONG2NUM(ticks), ULONG2NUM(depth));
  return(0);
}

static VALUE
_m_each(VALUE obj) {
  aguri_data_t *adt = NULL;
  Data_Get_Struct(obj, aguri_data_t, adt);
  
  adt->walked = 0;
  aguri_walk(adt->tree, _walker, adt);

  return(ULONG2NUM(adt->walked));
}


void Init_aguritrie()
{
  rb_mAguri = rb_define_module("Aguri");
  rb_cTree = rb_define_class_under(rb_mAguri, "Tree", rb_cObject);
  rb_define_alloc_func(rb_cTree, _alloc);
  rb_define_method(rb_cTree, "initialize", _m_initialize, -1);
  rb_define_method(rb_cTree, "add", _m_add, -1);
  rb_define_method(rb_cTree, "threshold", _m_threshold, 1);
  rb_define_method(rb_cTree, "size", _m_size, 0);
  rb_define_method(rb_cTree, "dump", _m_dump, 0);
  rb_define_method(rb_cTree, "each", _m_each, 0);

}

