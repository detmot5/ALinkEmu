//
// Created by Norbert Bielak at 07.12.2021
//
#pragma once

#include <cstdint>
#include <cstring>
#include <exception>
#include <iterator>

namespace ALinkEmu {

struct ArrayBufferException : public std::exception {
  const char* what() const noexcept override {
    return "Unknown error occured while working with ALinkEmu ArrayBuffer";
  }
};

struct UnallocatedArrayBufferException : public ArrayBufferException {
  const char* what() const noexcept override { return "Tried to access unallocated ArrayBuffer!"; }
};

struct ArrayBufferOutOfRangeException : public ArrayBufferException {
  const char* what() const noexcept override { return "ArrayBuffer overflow!"; }
};

template <typename ArrayBuffer>
class ArrayBufferIterator {
 public:
  using ValueType = typename ArrayBuffer::ValueType;
  using PointerType = ValueType*;
  using Referencetype = ValueType&;

 public:
  explicit ArrayBufferIterator(PointerType pointer) : pointer(pointer) {}

  ArrayBufferIterator& operator++() {
    this->pointer++;
    return *this;
  }

  ArrayBufferIterator& operator++(int) {
    ArrayBufferIterator iterator = *this;
    ++(*this);
    return iterator;
  }

  ArrayBufferIterator& operator--() {
    this->pointer--;
    return *this;
  }

  ArrayBufferIterator& operator--(int) {
    ArrayBufferIterator iterator = *this;
    ++(*this);
    return iterator;
  }

  Referencetype operator[](size_t index) { return this->pointer[index]; }

  PointerType operator->() { return this->pointer; }
  Referencetype operator*() { return *this->pointer; }
  bool operator==(const ArrayBufferIterator& other) { return this->pointer == other.pointer; }
  bool operator!=(const ArrayBufferIterator& other) { return !(*this == other); }

 private:
  PointerType pointer;
};

/*
 *  Not automatically resizable heap allocated buffer
 *  Useful when dealing with chip's memory.
 */
template <typename T>
class ArrayBuffer {
 public:
  using ValueType = T;
  using Iterator = ArrayBufferIterator<ArrayBuffer<T>>;

 public:
  ArrayBuffer() = default;

  explicit ArrayBuffer(size_t size) {
    this->size = size;
    this->data = new T[this->size];
  }

  ArrayBuffer(const ArrayBuffer& other) {
    this->size = other.size;
    if (other.IsValid()) {
      this->Allocate(other.size);
      this->size = other.size;
      this->Memcpy(other.GetDataPointer(), other.size);
    }
  }

  ArrayBuffer(ArrayBuffer&& other) noexcept {
    this->size = other.size;
    this->data = other.data;
    other.data = nullptr;
  }

  ~ArrayBuffer() {
    if (this->data != nullptr) delete[] data;
  }

  inline void Allocate(size_t size) {
    if (this->data != nullptr) delete[] this->data;
    this->size = size;
    this->data = new T[size];
  }

  inline bool IsValid() const { return this->data != nullptr; }

  inline size_t GetSize() { return this->size; }

  inline const T& GetElement(size_t index) const {
    if (this->data == nullptr) throw UnallocatedArrayBufferException();
    if (index >= this->size) throw ArrayBufferOutOfRangeException();
    return this->data[index];
  }

  inline T& GetElement(size_t index) {
    if (this->data == nullptr) throw UnallocatedArrayBufferException();
    if (index >= this->size) throw ArrayBufferOutOfRangeException();
    return this->data[index];
  }

  void Memset(const T& value, size_t limit) {
    if (this->data == nullptr) throw UnallocatedArrayBufferException();
    if (limit > this->size) limit = this->size;
    std::memset(this->data, value, limit);
  }

  void Memset(const T& value) { this->Memset(value, this->size); }

  void Memcpy(const T* rawData, size_t limit) {
    if (this->data == nullptr) throw UnallocatedArrayBufferException();
    if (limit > this->size) limit = this->size;
    std::memcpy(this->data, rawData, limit);
  }

  inline const T& operator[](size_t index) const { return this->GetElement(index); }
  inline T& operator[](size_t index) { return this->GetElement(index); }

  T* GetDataPointer() const { return this->data; }

  Iterator begin() { return Iterator(this->data); }
  Iterator end() { return Iterator(this->data + size); }

  Iterator begin() const { return Iterator(this->data); }
  Iterator end() const { return Iterator(this->data + size); }

 private:
  T* data = nullptr;
  size_t size = 0;
};

// Frequently used buffers
using ByteBuffer = ArrayBuffer<uint8_t>;

}  // namespace ALinkEmu
