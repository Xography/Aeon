// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: EntityDB.proto

#ifndef PROTOBUF_EntityDB_2eproto__INCLUDED
#define PROTOBUF_EntityDB_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2004001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
#include "Datatypes.pb.h"
#include "Entity.pb.h"
#include "Attribute.pb.h"
#include "Component.pb.h"
#include "UUID.pb.h"
// @@protoc_insertion_point(includes)

namespace Aeon {
namespace Serialize {

// Internal implementation detail -- do not call these.
void AEON_EXPORT protobuf_AddDesc_EntityDB_2eproto();
void protobuf_AssignDesc_EntityDB_2eproto();
void protobuf_ShutdownFile_EntityDB_2eproto();

class CategoryDescriptor;
class DBDescriptor;
class EntityDB;

// ===================================================================

class AEON_EXPORT CategoryDescriptor : public ::google::protobuf::Message {
 public:
  CategoryDescriptor();
  virtual ~CategoryDescriptor();
  
  CategoryDescriptor(const CategoryDescriptor& from);
  
  inline CategoryDescriptor& operator=(const CategoryDescriptor& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const CategoryDescriptor& default_instance();
  
  void Swap(CategoryDescriptor* other);
  
  // implements Message ----------------------------------------------
  
  CategoryDescriptor* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CategoryDescriptor& from);
  void MergeFrom(const CategoryDescriptor& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required .Aeon.Serialize.UUID id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline const ::Aeon::Serialize::UUID& id() const;
  inline ::Aeon::Serialize::UUID* mutable_id();
  inline ::Aeon::Serialize::UUID* release_id();
  
  // required string name = 2;
  inline bool has_name() const;
  inline void clear_name();
  static const int kNameFieldNumber = 2;
  inline const ::std::string& name() const;
  inline void set_name(const ::std::string& value);
  inline void set_name(const char* value);
  inline void set_name(const char* value, size_t size);
  inline ::std::string* mutable_name();
  inline ::std::string* release_name();
  
  // @@protoc_insertion_point(class_scope:Aeon.Serialize.CategoryDescriptor)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_name();
  inline void clear_has_name();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::Aeon::Serialize::UUID* id_;
  ::std::string* name_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void AEON_EXPORT protobuf_AddDesc_EntityDB_2eproto();
  friend void protobuf_AssignDesc_EntityDB_2eproto();
  friend void protobuf_ShutdownFile_EntityDB_2eproto();
  
  void InitAsDefaultInstance();
  static CategoryDescriptor* default_instance_;
};
// -------------------------------------------------------------------

class AEON_EXPORT DBDescriptor : public ::google::protobuf::Message {
 public:
  DBDescriptor();
  virtual ~DBDescriptor();
  
  DBDescriptor(const DBDescriptor& from);
  
  inline DBDescriptor& operator=(const DBDescriptor& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const DBDescriptor& default_instance();
  
  void Swap(DBDescriptor* other);
  
  // implements Message ----------------------------------------------
  
  DBDescriptor* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const DBDescriptor& from);
  void MergeFrom(const DBDescriptor& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required string name = 1;
  inline bool has_name() const;
  inline void clear_name();
  static const int kNameFieldNumber = 1;
  inline const ::std::string& name() const;
  inline void set_name(const ::std::string& value);
  inline void set_name(const char* value);
  inline void set_name(const char* value, size_t size);
  inline ::std::string* mutable_name();
  inline ::std::string* release_name();
  
  // required string version = 2;
  inline bool has_version() const;
  inline void clear_version();
  static const int kVersionFieldNumber = 2;
  inline const ::std::string& version() const;
  inline void set_version(const ::std::string& value);
  inline void set_version(const char* value);
  inline void set_version(const char* value, size_t size);
  inline ::std::string* mutable_version();
  inline ::std::string* release_version();
  
  // required string description = 3;
  inline bool has_description() const;
  inline void clear_description();
  static const int kDescriptionFieldNumber = 3;
  inline const ::std::string& description() const;
  inline void set_description(const ::std::string& value);
  inline void set_description(const char* value);
  inline void set_description(const char* value, size_t size);
  inline ::std::string* mutable_description();
  inline ::std::string* release_description();
  
  // required string modified = 4;
  inline bool has_modified() const;
  inline void clear_modified();
  static const int kModifiedFieldNumber = 4;
  inline const ::std::string& modified() const;
  inline void set_modified(const ::std::string& value);
  inline void set_modified(const char* value);
  inline void set_modified(const char* value, size_t size);
  inline ::std::string* mutable_modified();
  inline ::std::string* release_modified();
  
  // @@protoc_insertion_point(class_scope:Aeon.Serialize.DBDescriptor)
 private:
  inline void set_has_name();
  inline void clear_has_name();
  inline void set_has_version();
  inline void clear_has_version();
  inline void set_has_description();
  inline void clear_has_description();
  inline void set_has_modified();
  inline void clear_has_modified();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* name_;
  ::std::string* version_;
  ::std::string* description_;
  ::std::string* modified_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];
  
  friend void AEON_EXPORT protobuf_AddDesc_EntityDB_2eproto();
  friend void protobuf_AssignDesc_EntityDB_2eproto();
  friend void protobuf_ShutdownFile_EntityDB_2eproto();
  
  void InitAsDefaultInstance();
  static DBDescriptor* default_instance_;
};
// -------------------------------------------------------------------

class AEON_EXPORT EntityDB : public ::google::protobuf::Message {
 public:
  EntityDB();
  virtual ~EntityDB();
  
  EntityDB(const EntityDB& from);
  
  inline EntityDB& operator=(const EntityDB& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const EntityDB& default_instance();
  
  void Swap(EntityDB* other);
  
  // implements Message ----------------------------------------------
  
  EntityDB* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const EntityDB& from);
  void MergeFrom(const EntityDB& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required .Aeon.Serialize.DBDescriptor dbDescriptor = 1;
  inline bool has_dbdescriptor() const;
  inline void clear_dbdescriptor();
  static const int kDbDescriptorFieldNumber = 1;
  inline const ::Aeon::Serialize::DBDescriptor& dbdescriptor() const;
  inline ::Aeon::Serialize::DBDescriptor* mutable_dbdescriptor();
  inline ::Aeon::Serialize::DBDescriptor* release_dbdescriptor();
  
  // repeated .Aeon.Serialize.EntityDescriptor entities = 2;
  inline int entities_size() const;
  inline void clear_entities();
  static const int kEntitiesFieldNumber = 2;
  inline const ::Aeon::Serialize::EntityDescriptor& entities(int index) const;
  inline ::Aeon::Serialize::EntityDescriptor* mutable_entities(int index);
  inline ::Aeon::Serialize::EntityDescriptor* add_entities();
  inline const ::google::protobuf::RepeatedPtrField< ::Aeon::Serialize::EntityDescriptor >&
      entities() const;
  inline ::google::protobuf::RepeatedPtrField< ::Aeon::Serialize::EntityDescriptor >*
      mutable_entities();
  
  // repeated .Aeon.Serialize.ComponentDescriptor components = 3;
  inline int components_size() const;
  inline void clear_components();
  static const int kComponentsFieldNumber = 3;
  inline const ::Aeon::Serialize::ComponentDescriptor& components(int index) const;
  inline ::Aeon::Serialize::ComponentDescriptor* mutable_components(int index);
  inline ::Aeon::Serialize::ComponentDescriptor* add_components();
  inline const ::google::protobuf::RepeatedPtrField< ::Aeon::Serialize::ComponentDescriptor >&
      components() const;
  inline ::google::protobuf::RepeatedPtrField< ::Aeon::Serialize::ComponentDescriptor >*
      mutable_components();
  
  // repeated .Aeon.Serialize.CategoryDescriptor categories = 4;
  inline int categories_size() const;
  inline void clear_categories();
  static const int kCategoriesFieldNumber = 4;
  inline const ::Aeon::Serialize::CategoryDescriptor& categories(int index) const;
  inline ::Aeon::Serialize::CategoryDescriptor* mutable_categories(int index);
  inline ::Aeon::Serialize::CategoryDescriptor* add_categories();
  inline const ::google::protobuf::RepeatedPtrField< ::Aeon::Serialize::CategoryDescriptor >&
      categories() const;
  inline ::google::protobuf::RepeatedPtrField< ::Aeon::Serialize::CategoryDescriptor >*
      mutable_categories();
  
  // @@protoc_insertion_point(class_scope:Aeon.Serialize.EntityDB)
 private:
  inline void set_has_dbdescriptor();
  inline void clear_has_dbdescriptor();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::Aeon::Serialize::DBDescriptor* dbdescriptor_;
  ::google::protobuf::RepeatedPtrField< ::Aeon::Serialize::EntityDescriptor > entities_;
  ::google::protobuf::RepeatedPtrField< ::Aeon::Serialize::ComponentDescriptor > components_;
  ::google::protobuf::RepeatedPtrField< ::Aeon::Serialize::CategoryDescriptor > categories_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];
  
  friend void AEON_EXPORT protobuf_AddDesc_EntityDB_2eproto();
  friend void protobuf_AssignDesc_EntityDB_2eproto();
  friend void protobuf_ShutdownFile_EntityDB_2eproto();
  
  void InitAsDefaultInstance();
  static EntityDB* default_instance_;
};
// ===================================================================


// ===================================================================

// CategoryDescriptor

// required .Aeon.Serialize.UUID id = 1;
inline bool CategoryDescriptor::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void CategoryDescriptor::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void CategoryDescriptor::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void CategoryDescriptor::clear_id() {
  if (id_ != NULL) id_->::Aeon::Serialize::UUID::Clear();
  clear_has_id();
}
inline const ::Aeon::Serialize::UUID& CategoryDescriptor::id() const {
  return id_ != NULL ? *id_ : *default_instance_->id_;
}
inline ::Aeon::Serialize::UUID* CategoryDescriptor::mutable_id() {
  set_has_id();
  if (id_ == NULL) id_ = new ::Aeon::Serialize::UUID;
  return id_;
}
inline ::Aeon::Serialize::UUID* CategoryDescriptor::release_id() {
  clear_has_id();
  ::Aeon::Serialize::UUID* temp = id_;
  id_ = NULL;
  return temp;
}

// required string name = 2;
inline bool CategoryDescriptor::has_name() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void CategoryDescriptor::set_has_name() {
  _has_bits_[0] |= 0x00000002u;
}
inline void CategoryDescriptor::clear_has_name() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void CategoryDescriptor::clear_name() {
  if (name_ != &::google::protobuf::internal::kEmptyString) {
    name_->clear();
  }
  clear_has_name();
}
inline const ::std::string& CategoryDescriptor::name() const {
  return *name_;
}
inline void CategoryDescriptor::set_name(const ::std::string& value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void CategoryDescriptor::set_name(const char* value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void CategoryDescriptor::set_name(const char* value, size_t size) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* CategoryDescriptor::mutable_name() {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  return name_;
}
inline ::std::string* CategoryDescriptor::release_name() {
  clear_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = name_;
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// -------------------------------------------------------------------

// DBDescriptor

// required string name = 1;
inline bool DBDescriptor::has_name() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void DBDescriptor::set_has_name() {
  _has_bits_[0] |= 0x00000001u;
}
inline void DBDescriptor::clear_has_name() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void DBDescriptor::clear_name() {
  if (name_ != &::google::protobuf::internal::kEmptyString) {
    name_->clear();
  }
  clear_has_name();
}
inline const ::std::string& DBDescriptor::name() const {
  return *name_;
}
inline void DBDescriptor::set_name(const ::std::string& value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void DBDescriptor::set_name(const char* value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void DBDescriptor::set_name(const char* value, size_t size) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* DBDescriptor::mutable_name() {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  return name_;
}
inline ::std::string* DBDescriptor::release_name() {
  clear_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = name_;
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// required string version = 2;
inline bool DBDescriptor::has_version() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void DBDescriptor::set_has_version() {
  _has_bits_[0] |= 0x00000002u;
}
inline void DBDescriptor::clear_has_version() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void DBDescriptor::clear_version() {
  if (version_ != &::google::protobuf::internal::kEmptyString) {
    version_->clear();
  }
  clear_has_version();
}
inline const ::std::string& DBDescriptor::version() const {
  return *version_;
}
inline void DBDescriptor::set_version(const ::std::string& value) {
  set_has_version();
  if (version_ == &::google::protobuf::internal::kEmptyString) {
    version_ = new ::std::string;
  }
  version_->assign(value);
}
inline void DBDescriptor::set_version(const char* value) {
  set_has_version();
  if (version_ == &::google::protobuf::internal::kEmptyString) {
    version_ = new ::std::string;
  }
  version_->assign(value);
}
inline void DBDescriptor::set_version(const char* value, size_t size) {
  set_has_version();
  if (version_ == &::google::protobuf::internal::kEmptyString) {
    version_ = new ::std::string;
  }
  version_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* DBDescriptor::mutable_version() {
  set_has_version();
  if (version_ == &::google::protobuf::internal::kEmptyString) {
    version_ = new ::std::string;
  }
  return version_;
}
inline ::std::string* DBDescriptor::release_version() {
  clear_has_version();
  if (version_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = version_;
    version_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// required string description = 3;
inline bool DBDescriptor::has_description() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void DBDescriptor::set_has_description() {
  _has_bits_[0] |= 0x00000004u;
}
inline void DBDescriptor::clear_has_description() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void DBDescriptor::clear_description() {
  if (description_ != &::google::protobuf::internal::kEmptyString) {
    description_->clear();
  }
  clear_has_description();
}
inline const ::std::string& DBDescriptor::description() const {
  return *description_;
}
inline void DBDescriptor::set_description(const ::std::string& value) {
  set_has_description();
  if (description_ == &::google::protobuf::internal::kEmptyString) {
    description_ = new ::std::string;
  }
  description_->assign(value);
}
inline void DBDescriptor::set_description(const char* value) {
  set_has_description();
  if (description_ == &::google::protobuf::internal::kEmptyString) {
    description_ = new ::std::string;
  }
  description_->assign(value);
}
inline void DBDescriptor::set_description(const char* value, size_t size) {
  set_has_description();
  if (description_ == &::google::protobuf::internal::kEmptyString) {
    description_ = new ::std::string;
  }
  description_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* DBDescriptor::mutable_description() {
  set_has_description();
  if (description_ == &::google::protobuf::internal::kEmptyString) {
    description_ = new ::std::string;
  }
  return description_;
}
inline ::std::string* DBDescriptor::release_description() {
  clear_has_description();
  if (description_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = description_;
    description_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// required string modified = 4;
inline bool DBDescriptor::has_modified() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void DBDescriptor::set_has_modified() {
  _has_bits_[0] |= 0x00000008u;
}
inline void DBDescriptor::clear_has_modified() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void DBDescriptor::clear_modified() {
  if (modified_ != &::google::protobuf::internal::kEmptyString) {
    modified_->clear();
  }
  clear_has_modified();
}
inline const ::std::string& DBDescriptor::modified() const {
  return *modified_;
}
inline void DBDescriptor::set_modified(const ::std::string& value) {
  set_has_modified();
  if (modified_ == &::google::protobuf::internal::kEmptyString) {
    modified_ = new ::std::string;
  }
  modified_->assign(value);
}
inline void DBDescriptor::set_modified(const char* value) {
  set_has_modified();
  if (modified_ == &::google::protobuf::internal::kEmptyString) {
    modified_ = new ::std::string;
  }
  modified_->assign(value);
}
inline void DBDescriptor::set_modified(const char* value, size_t size) {
  set_has_modified();
  if (modified_ == &::google::protobuf::internal::kEmptyString) {
    modified_ = new ::std::string;
  }
  modified_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* DBDescriptor::mutable_modified() {
  set_has_modified();
  if (modified_ == &::google::protobuf::internal::kEmptyString) {
    modified_ = new ::std::string;
  }
  return modified_;
}
inline ::std::string* DBDescriptor::release_modified() {
  clear_has_modified();
  if (modified_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = modified_;
    modified_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// -------------------------------------------------------------------

// EntityDB

// required .Aeon.Serialize.DBDescriptor dbDescriptor = 1;
inline bool EntityDB::has_dbdescriptor() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void EntityDB::set_has_dbdescriptor() {
  _has_bits_[0] |= 0x00000001u;
}
inline void EntityDB::clear_has_dbdescriptor() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void EntityDB::clear_dbdescriptor() {
  if (dbdescriptor_ != NULL) dbdescriptor_->::Aeon::Serialize::DBDescriptor::Clear();
  clear_has_dbdescriptor();
}
inline const ::Aeon::Serialize::DBDescriptor& EntityDB::dbdescriptor() const {
  return dbdescriptor_ != NULL ? *dbdescriptor_ : *default_instance_->dbdescriptor_;
}
inline ::Aeon::Serialize::DBDescriptor* EntityDB::mutable_dbdescriptor() {
  set_has_dbdescriptor();
  if (dbdescriptor_ == NULL) dbdescriptor_ = new ::Aeon::Serialize::DBDescriptor;
  return dbdescriptor_;
}
inline ::Aeon::Serialize::DBDescriptor* EntityDB::release_dbdescriptor() {
  clear_has_dbdescriptor();
  ::Aeon::Serialize::DBDescriptor* temp = dbdescriptor_;
  dbdescriptor_ = NULL;
  return temp;
}

// repeated .Aeon.Serialize.EntityDescriptor entities = 2;
inline int EntityDB::entities_size() const {
  return entities_.size();
}
inline void EntityDB::clear_entities() {
  entities_.Clear();
}
inline const ::Aeon::Serialize::EntityDescriptor& EntityDB::entities(int index) const {
  return entities_.Get(index);
}
inline ::Aeon::Serialize::EntityDescriptor* EntityDB::mutable_entities(int index) {
  return entities_.Mutable(index);
}
inline ::Aeon::Serialize::EntityDescriptor* EntityDB::add_entities() {
  return entities_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::Aeon::Serialize::EntityDescriptor >&
EntityDB::entities() const {
  return entities_;
}
inline ::google::protobuf::RepeatedPtrField< ::Aeon::Serialize::EntityDescriptor >*
EntityDB::mutable_entities() {
  return &entities_;
}

// repeated .Aeon.Serialize.ComponentDescriptor components = 3;
inline int EntityDB::components_size() const {
  return components_.size();
}
inline void EntityDB::clear_components() {
  components_.Clear();
}
inline const ::Aeon::Serialize::ComponentDescriptor& EntityDB::components(int index) const {
  return components_.Get(index);
}
inline ::Aeon::Serialize::ComponentDescriptor* EntityDB::mutable_components(int index) {
  return components_.Mutable(index);
}
inline ::Aeon::Serialize::ComponentDescriptor* EntityDB::add_components() {
  return components_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::Aeon::Serialize::ComponentDescriptor >&
EntityDB::components() const {
  return components_;
}
inline ::google::protobuf::RepeatedPtrField< ::Aeon::Serialize::ComponentDescriptor >*
EntityDB::mutable_components() {
  return &components_;
}

// repeated .Aeon.Serialize.CategoryDescriptor categories = 4;
inline int EntityDB::categories_size() const {
  return categories_.size();
}
inline void EntityDB::clear_categories() {
  categories_.Clear();
}
inline const ::Aeon::Serialize::CategoryDescriptor& EntityDB::categories(int index) const {
  return categories_.Get(index);
}
inline ::Aeon::Serialize::CategoryDescriptor* EntityDB::mutable_categories(int index) {
  return categories_.Mutable(index);
}
inline ::Aeon::Serialize::CategoryDescriptor* EntityDB::add_categories() {
  return categories_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::Aeon::Serialize::CategoryDescriptor >&
EntityDB::categories() const {
  return categories_;
}
inline ::google::protobuf::RepeatedPtrField< ::Aeon::Serialize::CategoryDescriptor >*
EntityDB::mutable_categories() {
  return &categories_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace Serialize
}  // namespace Aeon

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_EntityDB_2eproto__INCLUDED
