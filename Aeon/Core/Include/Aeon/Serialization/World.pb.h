// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: World.proto

#ifndef PROTOBUF_World_2eproto__INCLUDED
#define PROTOBUF_World_2eproto__INCLUDED

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
#include "Entity.pb.h"
// @@protoc_insertion_point(includes)

namespace Aeon {
namespace Serialize {

// Internal implementation detail -- do not call these.
void AEON_EXPORT protobuf_AddDesc_World_2eproto();
void protobuf_AssignDesc_World_2eproto();
void protobuf_ShutdownFile_World_2eproto();

class World;

// ===================================================================

class AEON_EXPORT World : public ::google::protobuf::Message {
 public:
  World();
  virtual ~World();
  
  World(const World& from);
  
  inline World& operator=(const World& from) {
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
  static const World& default_instance();
  
  void Swap(World* other);
  
  // implements Message ----------------------------------------------
  
  World* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const World& from);
  void MergeFrom(const World& from);
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
  
  // required string world_name = 1;
  inline bool has_world_name() const;
  inline void clear_world_name();
  static const int kWorldNameFieldNumber = 1;
  inline const ::std::string& world_name() const;
  inline void set_world_name(const ::std::string& value);
  inline void set_world_name(const char* value);
  inline void set_world_name(const char* value, size_t size);
  inline ::std::string* mutable_world_name();
  inline ::std::string* release_world_name();
  
  // required fixed32 timestamp = 2;
  inline bool has_timestamp() const;
  inline void clear_timestamp();
  static const int kTimestampFieldNumber = 2;
  inline ::google::protobuf::uint32 timestamp() const;
  inline void set_timestamp(::google::protobuf::uint32 value);
  
  // required .Aeon.Serialize.EntityInstance settings = 3;
  inline bool has_settings() const;
  inline void clear_settings();
  static const int kSettingsFieldNumber = 3;
  inline const ::Aeon::Serialize::EntityInstance& settings() const;
  inline ::Aeon::Serialize::EntityInstance* mutable_settings();
  inline ::Aeon::Serialize::EntityInstance* release_settings();
  
  // repeated .Aeon.Serialize.EntityInstance entity_table = 4;
  inline int entity_table_size() const;
  inline void clear_entity_table();
  static const int kEntityTableFieldNumber = 4;
  inline const ::Aeon::Serialize::EntityInstance& entity_table(int index) const;
  inline ::Aeon::Serialize::EntityInstance* mutable_entity_table(int index);
  inline ::Aeon::Serialize::EntityInstance* add_entity_table();
  inline const ::google::protobuf::RepeatedPtrField< ::Aeon::Serialize::EntityInstance >&
      entity_table() const;
  inline ::google::protobuf::RepeatedPtrField< ::Aeon::Serialize::EntityInstance >*
      mutable_entity_table();
  
  // @@protoc_insertion_point(class_scope:Aeon.Serialize.World)
 private:
  inline void set_has_world_name();
  inline void clear_has_world_name();
  inline void set_has_timestamp();
  inline void clear_has_timestamp();
  inline void set_has_settings();
  inline void clear_has_settings();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::std::string* world_name_;
  ::Aeon::Serialize::EntityInstance* settings_;
  ::google::protobuf::RepeatedPtrField< ::Aeon::Serialize::EntityInstance > entity_table_;
  ::google::protobuf::uint32 timestamp_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];
  
  friend void AEON_EXPORT protobuf_AddDesc_World_2eproto();
  friend void protobuf_AssignDesc_World_2eproto();
  friend void protobuf_ShutdownFile_World_2eproto();
  
  void InitAsDefaultInstance();
  static World* default_instance_;
};
// ===================================================================


// ===================================================================

// World

// required string world_name = 1;
inline bool World::has_world_name() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void World::set_has_world_name() {
  _has_bits_[0] |= 0x00000001u;
}
inline void World::clear_has_world_name() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void World::clear_world_name() {
  if (world_name_ != &::google::protobuf::internal::kEmptyString) {
    world_name_->clear();
  }
  clear_has_world_name();
}
inline const ::std::string& World::world_name() const {
  return *world_name_;
}
inline void World::set_world_name(const ::std::string& value) {
  set_has_world_name();
  if (world_name_ == &::google::protobuf::internal::kEmptyString) {
    world_name_ = new ::std::string;
  }
  world_name_->assign(value);
}
inline void World::set_world_name(const char* value) {
  set_has_world_name();
  if (world_name_ == &::google::protobuf::internal::kEmptyString) {
    world_name_ = new ::std::string;
  }
  world_name_->assign(value);
}
inline void World::set_world_name(const char* value, size_t size) {
  set_has_world_name();
  if (world_name_ == &::google::protobuf::internal::kEmptyString) {
    world_name_ = new ::std::string;
  }
  world_name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* World::mutable_world_name() {
  set_has_world_name();
  if (world_name_ == &::google::protobuf::internal::kEmptyString) {
    world_name_ = new ::std::string;
  }
  return world_name_;
}
inline ::std::string* World::release_world_name() {
  clear_has_world_name();
  if (world_name_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = world_name_;
    world_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// required fixed32 timestamp = 2;
inline bool World::has_timestamp() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void World::set_has_timestamp() {
  _has_bits_[0] |= 0x00000002u;
}
inline void World::clear_has_timestamp() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void World::clear_timestamp() {
  timestamp_ = 0u;
  clear_has_timestamp();
}
inline ::google::protobuf::uint32 World::timestamp() const {
  return timestamp_;
}
inline void World::set_timestamp(::google::protobuf::uint32 value) {
  set_has_timestamp();
  timestamp_ = value;
}

// required .Aeon.Serialize.EntityInstance settings = 3;
inline bool World::has_settings() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void World::set_has_settings() {
  _has_bits_[0] |= 0x00000004u;
}
inline void World::clear_has_settings() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void World::clear_settings() {
  if (settings_ != NULL) settings_->::Aeon::Serialize::EntityInstance::Clear();
  clear_has_settings();
}
inline const ::Aeon::Serialize::EntityInstance& World::settings() const {
  return settings_ != NULL ? *settings_ : *default_instance_->settings_;
}
inline ::Aeon::Serialize::EntityInstance* World::mutable_settings() {
  set_has_settings();
  if (settings_ == NULL) settings_ = new ::Aeon::Serialize::EntityInstance;
  return settings_;
}
inline ::Aeon::Serialize::EntityInstance* World::release_settings() {
  clear_has_settings();
  ::Aeon::Serialize::EntityInstance* temp = settings_;
  settings_ = NULL;
  return temp;
}

// repeated .Aeon.Serialize.EntityInstance entity_table = 4;
inline int World::entity_table_size() const {
  return entity_table_.size();
}
inline void World::clear_entity_table() {
  entity_table_.Clear();
}
inline const ::Aeon::Serialize::EntityInstance& World::entity_table(int index) const {
  return entity_table_.Get(index);
}
inline ::Aeon::Serialize::EntityInstance* World::mutable_entity_table(int index) {
  return entity_table_.Mutable(index);
}
inline ::Aeon::Serialize::EntityInstance* World::add_entity_table() {
  return entity_table_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::Aeon::Serialize::EntityInstance >&
World::entity_table() const {
  return entity_table_;
}
inline ::google::protobuf::RepeatedPtrField< ::Aeon::Serialize::EntityInstance >*
World::mutable_entity_table() {
  return &entity_table_;
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

#endif  // PROTOBUF_World_2eproto__INCLUDED
