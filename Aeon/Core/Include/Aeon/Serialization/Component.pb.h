// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Component.proto

#ifndef PROTOBUF_Component_2eproto__INCLUDED
#define PROTOBUF_Component_2eproto__INCLUDED

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
#include "Attribute.pb.h"
#include "UUID.pb.h"
// @@protoc_insertion_point(includes)

namespace Aeon {
namespace Serialize {

// Internal implementation detail -- do not call these.
void AEON_EXPORT protobuf_AddDesc_Component_2eproto();
void protobuf_AssignDesc_Component_2eproto();
void protobuf_ShutdownFile_Component_2eproto();

class ComponentDescriptor;
class ComponentDescriptor_Parameter;
class ComponentInstance;

enum ComponentDescriptor_ComponentType {
  ComponentDescriptor_ComponentType_NATIVE = 0,
  ComponentDescriptor_ComponentType_SCRIPT = 1
};
AEON_EXPORT bool ComponentDescriptor_ComponentType_IsValid(int value);
const ComponentDescriptor_ComponentType ComponentDescriptor_ComponentType_ComponentType_MIN = ComponentDescriptor_ComponentType_NATIVE;
const ComponentDescriptor_ComponentType ComponentDescriptor_ComponentType_ComponentType_MAX = ComponentDescriptor_ComponentType_SCRIPT;
const int ComponentDescriptor_ComponentType_ComponentType_ARRAYSIZE = ComponentDescriptor_ComponentType_ComponentType_MAX + 1;

AEON_EXPORT const ::google::protobuf::EnumDescriptor* ComponentDescriptor_ComponentType_descriptor();
inline const ::std::string& ComponentDescriptor_ComponentType_Name(ComponentDescriptor_ComponentType value) {
  return ::google::protobuf::internal::NameOfEnum(
    ComponentDescriptor_ComponentType_descriptor(), value);
}
inline bool ComponentDescriptor_ComponentType_Parse(
    const ::std::string& name, ComponentDescriptor_ComponentType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<ComponentDescriptor_ComponentType>(
    ComponentDescriptor_ComponentType_descriptor(), name, value);
}
// ===================================================================

class AEON_EXPORT ComponentDescriptor_Parameter : public ::google::protobuf::Message {
 public:
  ComponentDescriptor_Parameter();
  virtual ~ComponentDescriptor_Parameter();
  
  ComponentDescriptor_Parameter(const ComponentDescriptor_Parameter& from);
  
  inline ComponentDescriptor_Parameter& operator=(const ComponentDescriptor_Parameter& from) {
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
  static const ComponentDescriptor_Parameter& default_instance();
  
  void Swap(ComponentDescriptor_Parameter* other);
  
  // implements Message ----------------------------------------------
  
  ComponentDescriptor_Parameter* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ComponentDescriptor_Parameter& from);
  void MergeFrom(const ComponentDescriptor_Parameter& from);
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
  
  // required string key = 1;
  inline bool has_key() const;
  inline void clear_key();
  static const int kKeyFieldNumber = 1;
  inline const ::std::string& key() const;
  inline void set_key(const ::std::string& value);
  inline void set_key(const char* value);
  inline void set_key(const char* value, size_t size);
  inline ::std::string* mutable_key();
  inline ::std::string* release_key();
  
  // required .Aeon.Serialize.AttributeType type = 2;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 2;
  inline Aeon::Serialize::AttributeType type() const;
  inline void set_type(Aeon::Serialize::AttributeType value);
  
  // @@protoc_insertion_point(class_scope:Aeon.Serialize.ComponentDescriptor.Parameter)
 private:
  inline void set_has_key();
  inline void clear_has_key();
  inline void set_has_type();
  inline void clear_has_type();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* key_;
  int type_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void AEON_EXPORT protobuf_AddDesc_Component_2eproto();
  friend void protobuf_AssignDesc_Component_2eproto();
  friend void protobuf_ShutdownFile_Component_2eproto();
  
  void InitAsDefaultInstance();
  static ComponentDescriptor_Parameter* default_instance_;
};
// -------------------------------------------------------------------

class AEON_EXPORT ComponentDescriptor : public ::google::protobuf::Message {
 public:
  ComponentDescriptor();
  virtual ~ComponentDescriptor();
  
  ComponentDescriptor(const ComponentDescriptor& from);
  
  inline ComponentDescriptor& operator=(const ComponentDescriptor& from) {
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
  static const ComponentDescriptor& default_instance();
  
  void Swap(ComponentDescriptor* other);
  
  // implements Message ----------------------------------------------
  
  ComponentDescriptor* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ComponentDescriptor& from);
  void MergeFrom(const ComponentDescriptor& from);
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
  
  typedef ComponentDescriptor_Parameter Parameter;
  
  typedef ComponentDescriptor_ComponentType ComponentType;
  static const ComponentType NATIVE = ComponentDescriptor_ComponentType_NATIVE;
  static const ComponentType SCRIPT = ComponentDescriptor_ComponentType_SCRIPT;
  static inline bool ComponentType_IsValid(int value) {
    return ComponentDescriptor_ComponentType_IsValid(value);
  }
  static const ComponentType ComponentType_MIN =
    ComponentDescriptor_ComponentType_ComponentType_MIN;
  static const ComponentType ComponentType_MAX =
    ComponentDescriptor_ComponentType_ComponentType_MAX;
  static const int ComponentType_ARRAYSIZE =
    ComponentDescriptor_ComponentType_ComponentType_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  ComponentType_descriptor() {
    return ComponentDescriptor_ComponentType_descriptor();
  }
  static inline const ::std::string& ComponentType_Name(ComponentType value) {
    return ComponentDescriptor_ComponentType_Name(value);
  }
  static inline bool ComponentType_Parse(const ::std::string& name,
      ComponentType* value) {
    return ComponentDescriptor_ComponentType_Parse(name, value);
  }
  
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
  
  // optional string description = 3;
  inline bool has_description() const;
  inline void clear_description();
  static const int kDescriptionFieldNumber = 3;
  inline const ::std::string& description() const;
  inline void set_description(const ::std::string& value);
  inline void set_description(const char* value);
  inline void set_description(const char* value, size_t size);
  inline ::std::string* mutable_description();
  inline ::std::string* release_description();
  
  // required .Aeon.Serialize.ComponentDescriptor.ComponentType type = 4;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 4;
  inline ::Aeon::Serialize::ComponentDescriptor_ComponentType type() const;
  inline void set_type(::Aeon::Serialize::ComponentDescriptor_ComponentType value);
  
  // repeated .Aeon.Serialize.ComponentDescriptor.Parameter parameters = 5;
  inline int parameters_size() const;
  inline void clear_parameters();
  static const int kParametersFieldNumber = 5;
  inline const ::Aeon::Serialize::ComponentDescriptor_Parameter& parameters(int index) const;
  inline ::Aeon::Serialize::ComponentDescriptor_Parameter* mutable_parameters(int index);
  inline ::Aeon::Serialize::ComponentDescriptor_Parameter* add_parameters();
  inline const ::google::protobuf::RepeatedPtrField< ::Aeon::Serialize::ComponentDescriptor_Parameter >&
      parameters() const;
  inline ::google::protobuf::RepeatedPtrField< ::Aeon::Serialize::ComponentDescriptor_Parameter >*
      mutable_parameters();
  
  // @@protoc_insertion_point(class_scope:Aeon.Serialize.ComponentDescriptor)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_name();
  inline void clear_has_name();
  inline void set_has_description();
  inline void clear_has_description();
  inline void set_has_type();
  inline void clear_has_type();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::Aeon::Serialize::UUID* id_;
  ::std::string* name_;
  ::std::string* description_;
  ::google::protobuf::RepeatedPtrField< ::Aeon::Serialize::ComponentDescriptor_Parameter > parameters_;
  int type_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(5 + 31) / 32];
  
  friend void AEON_EXPORT protobuf_AddDesc_Component_2eproto();
  friend void protobuf_AssignDesc_Component_2eproto();
  friend void protobuf_ShutdownFile_Component_2eproto();
  
  void InitAsDefaultInstance();
  static ComponentDescriptor* default_instance_;
};
// -------------------------------------------------------------------

class AEON_EXPORT ComponentInstance : public ::google::protobuf::Message {
 public:
  ComponentInstance();
  virtual ~ComponentInstance();
  
  ComponentInstance(const ComponentInstance& from);
  
  inline ComponentInstance& operator=(const ComponentInstance& from) {
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
  static const ComponentInstance& default_instance();
  
  void Swap(ComponentInstance* other);
  
  // implements Message ----------------------------------------------
  
  ComponentInstance* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ComponentInstance& from);
  void MergeFrom(const ComponentInstance& from);
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
  
  // required .Aeon.Serialize.UUID this_id = 1;
  inline bool has_this_id() const;
  inline void clear_this_id();
  static const int kThisIdFieldNumber = 1;
  inline const ::Aeon::Serialize::UUID& this_id() const;
  inline ::Aeon::Serialize::UUID* mutable_this_id();
  inline ::Aeon::Serialize::UUID* release_this_id();
  
  // required .Aeon.Serialize.UUID descriptor_id = 2;
  inline bool has_descriptor_id() const;
  inline void clear_descriptor_id();
  static const int kDescriptorIdFieldNumber = 2;
  inline const ::Aeon::Serialize::UUID& descriptor_id() const;
  inline ::Aeon::Serialize::UUID* mutable_descriptor_id();
  inline ::Aeon::Serialize::UUID* release_descriptor_id();
  
  // @@protoc_insertion_point(class_scope:Aeon.Serialize.ComponentInstance)
 private:
  inline void set_has_this_id();
  inline void clear_has_this_id();
  inline void set_has_descriptor_id();
  inline void clear_has_descriptor_id();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::Aeon::Serialize::UUID* this_id_;
  ::Aeon::Serialize::UUID* descriptor_id_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void AEON_EXPORT protobuf_AddDesc_Component_2eproto();
  friend void protobuf_AssignDesc_Component_2eproto();
  friend void protobuf_ShutdownFile_Component_2eproto();
  
  void InitAsDefaultInstance();
  static ComponentInstance* default_instance_;
};
// ===================================================================


// ===================================================================

// ComponentDescriptor_Parameter

// required string key = 1;
inline bool ComponentDescriptor_Parameter::has_key() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ComponentDescriptor_Parameter::set_has_key() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ComponentDescriptor_Parameter::clear_has_key() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ComponentDescriptor_Parameter::clear_key() {
  if (key_ != &::google::protobuf::internal::kEmptyString) {
    key_->clear();
  }
  clear_has_key();
}
inline const ::std::string& ComponentDescriptor_Parameter::key() const {
  return *key_;
}
inline void ComponentDescriptor_Parameter::set_key(const ::std::string& value) {
  set_has_key();
  if (key_ == &::google::protobuf::internal::kEmptyString) {
    key_ = new ::std::string;
  }
  key_->assign(value);
}
inline void ComponentDescriptor_Parameter::set_key(const char* value) {
  set_has_key();
  if (key_ == &::google::protobuf::internal::kEmptyString) {
    key_ = new ::std::string;
  }
  key_->assign(value);
}
inline void ComponentDescriptor_Parameter::set_key(const char* value, size_t size) {
  set_has_key();
  if (key_ == &::google::protobuf::internal::kEmptyString) {
    key_ = new ::std::string;
  }
  key_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ComponentDescriptor_Parameter::mutable_key() {
  set_has_key();
  if (key_ == &::google::protobuf::internal::kEmptyString) {
    key_ = new ::std::string;
  }
  return key_;
}
inline ::std::string* ComponentDescriptor_Parameter::release_key() {
  clear_has_key();
  if (key_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = key_;
    key_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// required .Aeon.Serialize.AttributeType type = 2;
inline bool ComponentDescriptor_Parameter::has_type() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ComponentDescriptor_Parameter::set_has_type() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ComponentDescriptor_Parameter::clear_has_type() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ComponentDescriptor_Parameter::clear_type() {
  type_ = 1;
  clear_has_type();
}
inline Aeon::Serialize::AttributeType ComponentDescriptor_Parameter::type() const {
  return static_cast< Aeon::Serialize::AttributeType >(type_);
}
inline void ComponentDescriptor_Parameter::set_type(Aeon::Serialize::AttributeType value) {
  GOOGLE_DCHECK(Aeon::Serialize::AttributeType_IsValid(value));
  set_has_type();
  type_ = value;
}

// -------------------------------------------------------------------

// ComponentDescriptor

// required .Aeon.Serialize.UUID id = 1;
inline bool ComponentDescriptor::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ComponentDescriptor::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ComponentDescriptor::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ComponentDescriptor::clear_id() {
  if (id_ != NULL) id_->::Aeon::Serialize::UUID::Clear();
  clear_has_id();
}
inline const ::Aeon::Serialize::UUID& ComponentDescriptor::id() const {
  return id_ != NULL ? *id_ : *default_instance_->id_;
}
inline ::Aeon::Serialize::UUID* ComponentDescriptor::mutable_id() {
  set_has_id();
  if (id_ == NULL) id_ = new ::Aeon::Serialize::UUID;
  return id_;
}
inline ::Aeon::Serialize::UUID* ComponentDescriptor::release_id() {
  clear_has_id();
  ::Aeon::Serialize::UUID* temp = id_;
  id_ = NULL;
  return temp;
}

// required string name = 2;
inline bool ComponentDescriptor::has_name() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ComponentDescriptor::set_has_name() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ComponentDescriptor::clear_has_name() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ComponentDescriptor::clear_name() {
  if (name_ != &::google::protobuf::internal::kEmptyString) {
    name_->clear();
  }
  clear_has_name();
}
inline const ::std::string& ComponentDescriptor::name() const {
  return *name_;
}
inline void ComponentDescriptor::set_name(const ::std::string& value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void ComponentDescriptor::set_name(const char* value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void ComponentDescriptor::set_name(const char* value, size_t size) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ComponentDescriptor::mutable_name() {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  return name_;
}
inline ::std::string* ComponentDescriptor::release_name() {
  clear_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = name_;
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional string description = 3;
inline bool ComponentDescriptor::has_description() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ComponentDescriptor::set_has_description() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ComponentDescriptor::clear_has_description() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void ComponentDescriptor::clear_description() {
  if (description_ != &::google::protobuf::internal::kEmptyString) {
    description_->clear();
  }
  clear_has_description();
}
inline const ::std::string& ComponentDescriptor::description() const {
  return *description_;
}
inline void ComponentDescriptor::set_description(const ::std::string& value) {
  set_has_description();
  if (description_ == &::google::protobuf::internal::kEmptyString) {
    description_ = new ::std::string;
  }
  description_->assign(value);
}
inline void ComponentDescriptor::set_description(const char* value) {
  set_has_description();
  if (description_ == &::google::protobuf::internal::kEmptyString) {
    description_ = new ::std::string;
  }
  description_->assign(value);
}
inline void ComponentDescriptor::set_description(const char* value, size_t size) {
  set_has_description();
  if (description_ == &::google::protobuf::internal::kEmptyString) {
    description_ = new ::std::string;
  }
  description_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ComponentDescriptor::mutable_description() {
  set_has_description();
  if (description_ == &::google::protobuf::internal::kEmptyString) {
    description_ = new ::std::string;
  }
  return description_;
}
inline ::std::string* ComponentDescriptor::release_description() {
  clear_has_description();
  if (description_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = description_;
    description_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// required .Aeon.Serialize.ComponentDescriptor.ComponentType type = 4;
inline bool ComponentDescriptor::has_type() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void ComponentDescriptor::set_has_type() {
  _has_bits_[0] |= 0x00000008u;
}
inline void ComponentDescriptor::clear_has_type() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void ComponentDescriptor::clear_type() {
  type_ = 0;
  clear_has_type();
}
inline ::Aeon::Serialize::ComponentDescriptor_ComponentType ComponentDescriptor::type() const {
  return static_cast< ::Aeon::Serialize::ComponentDescriptor_ComponentType >(type_);
}
inline void ComponentDescriptor::set_type(::Aeon::Serialize::ComponentDescriptor_ComponentType value) {
  GOOGLE_DCHECK(::Aeon::Serialize::ComponentDescriptor_ComponentType_IsValid(value));
  set_has_type();
  type_ = value;
}

// repeated .Aeon.Serialize.ComponentDescriptor.Parameter parameters = 5;
inline int ComponentDescriptor::parameters_size() const {
  return parameters_.size();
}
inline void ComponentDescriptor::clear_parameters() {
  parameters_.Clear();
}
inline const ::Aeon::Serialize::ComponentDescriptor_Parameter& ComponentDescriptor::parameters(int index) const {
  return parameters_.Get(index);
}
inline ::Aeon::Serialize::ComponentDescriptor_Parameter* ComponentDescriptor::mutable_parameters(int index) {
  return parameters_.Mutable(index);
}
inline ::Aeon::Serialize::ComponentDescriptor_Parameter* ComponentDescriptor::add_parameters() {
  return parameters_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::Aeon::Serialize::ComponentDescriptor_Parameter >&
ComponentDescriptor::parameters() const {
  return parameters_;
}
inline ::google::protobuf::RepeatedPtrField< ::Aeon::Serialize::ComponentDescriptor_Parameter >*
ComponentDescriptor::mutable_parameters() {
  return &parameters_;
}

// -------------------------------------------------------------------

// ComponentInstance

// required .Aeon.Serialize.UUID this_id = 1;
inline bool ComponentInstance::has_this_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ComponentInstance::set_has_this_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ComponentInstance::clear_has_this_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ComponentInstance::clear_this_id() {
  if (this_id_ != NULL) this_id_->::Aeon::Serialize::UUID::Clear();
  clear_has_this_id();
}
inline const ::Aeon::Serialize::UUID& ComponentInstance::this_id() const {
  return this_id_ != NULL ? *this_id_ : *default_instance_->this_id_;
}
inline ::Aeon::Serialize::UUID* ComponentInstance::mutable_this_id() {
  set_has_this_id();
  if (this_id_ == NULL) this_id_ = new ::Aeon::Serialize::UUID;
  return this_id_;
}
inline ::Aeon::Serialize::UUID* ComponentInstance::release_this_id() {
  clear_has_this_id();
  ::Aeon::Serialize::UUID* temp = this_id_;
  this_id_ = NULL;
  return temp;
}

// required .Aeon.Serialize.UUID descriptor_id = 2;
inline bool ComponentInstance::has_descriptor_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ComponentInstance::set_has_descriptor_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ComponentInstance::clear_has_descriptor_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ComponentInstance::clear_descriptor_id() {
  if (descriptor_id_ != NULL) descriptor_id_->::Aeon::Serialize::UUID::Clear();
  clear_has_descriptor_id();
}
inline const ::Aeon::Serialize::UUID& ComponentInstance::descriptor_id() const {
  return descriptor_id_ != NULL ? *descriptor_id_ : *default_instance_->descriptor_id_;
}
inline ::Aeon::Serialize::UUID* ComponentInstance::mutable_descriptor_id() {
  set_has_descriptor_id();
  if (descriptor_id_ == NULL) descriptor_id_ = new ::Aeon::Serialize::UUID;
  return descriptor_id_;
}
inline ::Aeon::Serialize::UUID* ComponentInstance::release_descriptor_id() {
  clear_has_descriptor_id();
  ::Aeon::Serialize::UUID* temp = descriptor_id_;
  descriptor_id_ = NULL;
  return temp;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace Serialize
}  // namespace Aeon

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Aeon::Serialize::ComponentDescriptor_ComponentType>() {
  return ::Aeon::Serialize::ComponentDescriptor_ComponentType_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Component_2eproto__INCLUDED
